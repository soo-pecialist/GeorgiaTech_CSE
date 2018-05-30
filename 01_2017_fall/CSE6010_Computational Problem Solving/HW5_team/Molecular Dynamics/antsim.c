/*
 * Interaction.c
 *
 *  Created on: Nov 13, 2017
 *      Author: Caleb
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "antsim.h"






void interact(Ant* a, double width, double height, ViscekBox** boxMat, double R,int numBoxRows,int numBoxCols){


	ViscekBox* friends=find_neighbors(a,boxMat,R,numBoxRows,numBoxCols);

	int motion = motion_pressure(a, friends, .5, .1, .8);



	if(motion==1){


		gravity(a,friends,10);

		hard_body(a,friends,.5);

		walls(a, width, height);
	}
	free_ant_list(&(friends->antList));
	free(friends);
}

//Returns a box containing a list of all the nearest neighbors

ViscekBox* find_neighbors(Ant* a, ViscekBox** boxMat, double R, int numBoxRows, int numBoxCols){

	//Define a new "Viscek box" called friends that list all the ants within interaction range with ant* a.
	int i,j;
	BoxID boxloc=a->boxID;
	ViscekBox check;
	double x0=a->x;
	double y0=a->y;
	ViscekBox* friends = (ViscekBox*)malloc(sizeof(ViscekBox));
	Ant newFriend;
	int test=0;
	AntList* current_node;

	friends->numAnts=0;
	friends->antList=NULL;

	// loop through nearest 9 "Viscek boxes"

	for(i=-1;i<1;i++){

		if(boxloc.x+i>-1 && boxloc.x+i<=numBoxRows){
			test=boxloc.x+i;
			for(j=-1;j<1;j++){
			if(boxloc.y+j>-1 && boxloc.y+j<=numBoxCols){


				check = boxMat[boxloc.x+i][boxloc.y+j];


				//If the box exists, search through each of the ants in the box to find those within cutoff range
				//Add those found to the box called friends

				current_node = check.antList;

				while(current_node){
					newFriend=(current_node->ant);

					if(pow(newFriend.x-x0,2)+pow(newFriend.y-y0,2)<pow(R,2)){

						friends->numAnts++;
						add_front(&(friends->antList), newFriend);
						test++;

					}

					current_node = current_node->next;
				}
			}
			}

		}
	}

	return friends;

}

// Checks the neighbors to see if Ant* a will move this step. If the fraction of neighbors moving is
//greater than laziness, the ant will move. Laziness must be between 0 and 1. 0 corresponds to always
//moving and 1 corresponds to never moving.
int motion_pressure (Ant* a, ViscekBox* neighbors, double laziness, double waitnoise, double thetanoise){

	AntList* current_node;
	current_node = neighbors->antList;
	int count=0;
	int moving=0;
	double fraction;
	double mean_vx=0;
	double mean_vy=0;
	double test;

	while(current_node){

		count++;
		if(pow(current_node->ant.v_x_old,2)+pow(current_node->ant.v_y_old,2)>.01){

			moving++;
			mean_vx=mean_vx+current_node->ant.v_x_old;
			mean_vy=mean_vy+current_node->ant.v_y_old;
		}


		current_node = current_node->next;
	}
	test=(double) count;

	fraction=(double) moving / (double) count;



	// If enough neighbors were motionless, this particle also stops moving
	if (fraction<laziness){

		if(rand_in_range(0,1)>waitnoise){
			a->v_x=0;
			a->v_y=0;
			return 0;
		}

	}

	else if (rand_in_range(0,1)<waitnoise){
		a->v_x=0;
		a->v_y=0;
		return 0;
	}
	// Else the ant takes the average direction of it's neighbors


	//double theta= atan2(mean_vy,mean_vx)+ normal_rand(0,thetanoise);
	double theta= atan2(mean_vy,mean_vx)+ rand_in_range(-thetanoise/2, thetanoise/2);

	a->v_x=cos(theta);
	a->v_y=sin(theta);


	return 1;

}

void walls (Ant* a, double width, double height){



	double x0=a->x;
	double y0=a->y;

	if(x0<0.5 && a->v_x<0){
            //a->v_x=fabs(a->v_x);
		//a->v_x=1;
		//a->v_x=rand_in_range(0,.25);
		a->v_x=0;
    }

	if(x0>width-0.5 && a->v_x>0){
            //a->v_x=-fabs(a->v_x);
		//a->v_x=rand_in_range(-.25,0);
		a->v_x=0;
    }

	if(y0<0.5 && a->v_y<0){
            //a->v_y=fabs(a->v_y);
		//a->v_y=rand_in_range(0,.25);
		a->v_y=0;
    }

	if(y0>height-0.5 && a->v_y>0){
            //a->v_y=-fabs(a->v_y);
		//a->v_y=rand_in_range(-.25,0);
		a->v_y=0;
    }
}

//Checks for distance with neighbors and performs a hard body collision if they are too close.

void hard_body(Ant*a, ViscekBox* neighbors, double r){
	AntList* b;
	b = neighbors->antList;
	double x0=a->x;
	double y0=a->y;
	double theta;
	double vr,vt,dif;
	double d;

	double r2_min=20;
	while(b){

		d=pow(pow(b->ant.x-x0,2)+pow(b->ant.y-y0,2),.5);

			if(d<r && d>.01 && d<r2_min){


				r2_min=d;
				//Breaks the velocity into radial and tangential components
				theta=atan2(y0-b->ant.y,x0-b->ant.x);
				vr=a->v_x*cos(theta)+a->v_y*sin(theta);

				// Makes sure the particles are moving away from each other. If they would crash, instead they "slide"
				// along the tangential direction
				if (vr<0){

					vt=-a->v_x*sin(theta)+a->v_y*cos(theta);
					dif=r-d;
					vr=5*dif;
					a->v_x=vr*cos(theta)-vt*sin(theta);
					a->v_y=vr*sin(theta)+vt*cos(theta);


				}

			}


			b = b->next;
		}
}


// Looks for a neighbor within a distance of 2*leg length and with an angle between -pi and 0.
// If it find one, nothing happens. Otherwise, it acquires a purely downwards velocity.
void gravity(Ant* a, ViscekBox* neighbors, double leg_length){
	AntList* b;
	b = neighbors->antList;
	double x0=a->x;
	double y0=a->y;
	double theta;

	while(b){

		// If a neighbor is within 2*leg distance, check for angle. If a suitable neighbor
		//is found, gravity has no effect

		if(pow(b->ant.x-x0,2)+pow(b->ant.y-y0,2)<pow(2*leg_length,2)){

			theta=atan2(y0-b->ant.y,x0-b->ant.x);

			if(theta>0){
				return;
			}
		}


		b = b->next;
	}

	//Otherwise, the ant begins falling
	if(y0>leg_length){
		a->v_y=-1;
		a->v_x=0;
	}

}

// Pulls a random number from a normal distribution with mean mu and standard deviation  sigma
double normal_rand (double mu, double sigma){
	double a= ((double) (rand()%1000) +2)/ 1002;
	double b= ((double) (rand()%1000) +2)/ 1002;
	double z;

	z = sqrt(-2.0 * log(a))* cos(6.28318530718 * b);

	z= mu+sigma*z;
	if (z>10||z<-10){
		z=0;
	}
	return z;

}
