/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <math.h>
#include <string.h>
#include "engine.h"
#include <stdbool.h>

void usage(const char *programName) {
    printf("usage: %s N H W R T \n", programName);
    printf("where:\n");
    printf("    N   : Total number of ants.\n");
    printf("    H   : Height of container (in cm).\n");
    printf("    W   : Width of container (in cm).\n");
    printf("    R   : Viscek bounding box (square) width(in cm). Must be a value"
            " smaller than the smaller value of H and W and bigger than the dimensions of the ant.\n");    
    printf("    T   : Time to run the simulation for (in seconds).\n");
    printf("\nExample: %s 1000 20 5 1 10\n", programName);
}

double min(double a, double b){
    return (a<b?a:b); // Return the minimum of the two
}

Ant* malloc_ant_array(int num_ants){
    if(num_ants < 1){
	printf("ERROR: Invalid number of ants. Simulation Aborted!\n\n");
    }

    Ant *ant_arr = NULL;       // pointer to the array
    
    ant_arr = (Ant *)malloc(num_ants * sizeof(Ant)); 
    if (!ant_arr){
        printf("ERROR: Insufficient Memory for Ant Array!"); // If malloc returns NULL, operation failed due to insufficient memory
        exit(1);
    }

    return ant_arr;
}

ViscekBox** malloc_box_matrix(int num_rows, int num_cols){
     if((num_rows < 1) || (num_cols < 1)){ // Check for invalid matrix dimensions
	printf("ERROR: Invalid matrix dimensions. Simulation Aborted!\n\n");
    }

    ViscekBox **box = NULL; // pointer to the matrix
    int r_itr;

    box = (ViscekBox **)malloc(num_rows * sizeof(ViscekBox *));	//initialization using "array of pointers" method
    for(r_itr = 0; r_itr < num_rows; r_itr++){
    	box[r_itr] = (ViscekBox *)malloc(num_cols * sizeof(ViscekBox));
    	if (!box[r_itr]){
            printf("ERROR: Insufficient Memory for Viscek box!"); // If malloc returns NULL, operation failed due to insufficient memory
            exit(1);
    	}
    }

    return box;
}

void init_box_matrix(ViscekBox **box, int num_rows, int num_cols){
           
    for(int i = 0; i < num_rows; i++){
        for(int j = 0; j < num_cols; j++){
            box[i][j].numAnts = 0;
            box[i][j].antList = NULL;        
        }
    }
}

void free_box_matrix(int num_rows, int num_cols, ViscekBox **boxMat){ // free for viscek box matrix
    if((num_rows < 1) || (num_cols < 1)){					// Check for invalid matrix dimensions
        printf("ERROR: Invalid matrix dimensions. Simulation Aborted!\n\n");
        exit(1);
    }
    
    for(int r_itr = 0; r_itr < num_rows; r_itr++){
        for(int c_itr = 0; c_itr < num_cols; c_itr++){
            free_ant_list(&boxMat[r_itr][c_itr].antList);
        }
    }
    
    /*for(int r_itr = 0; r_itr < num_rows; r_itr++){
        free(boxMat[r_itr]);
         printf("row getting freed: %d\n", r_itr);
    }*/
    free(boxMat);
    printf("INFO: Box matrix freed!\n");
}

void init_ant_array(int num_ants, double ant_min_size, double container_width, double container_height, double radius, Ant ant[]){
    double position_x = - ant_min_size/2;
    double position_y = ant_min_size/2;
    for(int i = 0; i < num_ants; i++){
        position_x = position_x + ant_min_size; //spawn the ants uniformly. The ants are spawned with their legs compressed
        if(position_x < container_width){
            ant[i].x = position_x;
            ant[i].y = position_y;
        }
        else{ //The ants have filled the bottom row so start filling them in the next row above
            position_x = ant_min_size/2;
            position_y = position_y + ant_min_size;
            ant[i].x = position_x;
            ant[i].y = position_y;
        }
        ant[i].v_x = rand_in_range(-1.0, 1.0); //random velocities between -1 and 1 cm/s
        ant[i].v_y = rand_in_range(-1.0, 1.0);
        ant[i].v_x_old = ant[i].v_x;
        ant[i].v_y_old = ant[i].v_y;
        ant[i].boxID.x = (int)(ant[i].y/radius); //ants get assigned a box depending on where they are present
        ant[i].boxID.y = (int)(ant[i].x/radius);

    }
}

void populate_box_matrix(ViscekBox **box, Ant ant[],  int num_ants){
    AntList** head;
    //int temp_x, temp_y;
        
    for(int i = 0; i < num_ants; i++){
        //display_list(&box[ant[i].boxID.x][ant[i].boxID.y].antList);
        //temp_x = ant[i].boxID.x;
        //temp_y = ant[i].boxID.y;
        box[ant[i].boxID.x][ant[i].boxID.y].numAnts++;
        //printf("ant iteration = %d, boxID.x = %d, boxID.y = %d, numants till now = %d\n", i, temp_x, temp_y, box[ant[i].boxID.x][ant[i].boxID.y].numAnts);
        head = &box[ant[i].boxID.x][ant[i].boxID.y].antList;
        add_front(head, ant[i]);
    }
}

double rand_in_range(double min, double max){
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

int ant_overflow_check(int num_ants, double ant_min_size, double container_width, double container_height){
    int max_ants = (int)((container_width*container_height)/(ant_min_size*ant_min_size));
    return min(max_ants, num_ants);
}

void print_ant_stats(Ant *ant, int num_ants){
    printf("Printing Stats of all ants\n");
    for(int i = 0; i < num_ants; i++){
        printf("Ant %d:: x: %lf, y: %lf, v_x: %lf, v_y: %lf, BoxID: (%d, %d)\n", i, ant[i].x, ant[i].y, ant[i].v_x, ant[i].v_y, ant[i].boxID.x, ant[i].boxID.y);
    }
    printf("\n");
}

void print_box_stats(ViscekBox **boxMat, int num_rows, int num_cols){
    printf("Printing Stats of all boxes\n");
    for(int i = 0; i < num_rows; i++){
        for(int j = 0; j < num_cols; j++){
            printf("Num Ants in Box (%d, %d) :: %d\n", i, j, boxMat[i][j].numAnts);
            printf("The ants in this box: ");
            display_list(&boxMat[i][j].antList);
        }
    }
    printf("\n");
}

/*Linked List operations*/
void add_front(AntList** head, Ant ant){
    AntList* new_node = (AntList *)malloc(sizeof(AntList));
    new_node->ant = ant;
    if(!(*head)){
    	new_node->next = NULL;
	(*head) = new_node;
    }else{
	new_node->next = (*head);
	(*head) = new_node;
    }
    return;
}

void remove_link(AntList** head, Ant ant){
    AntList* temp_node = NULL;
    AntList* current_node = (*head);
    if((*head)->ant.x == ant.x && (*head)->ant.y == ant.y){
        (*head) = (*head)->next;
    }else{
        while(current_node){
            if(current_node->ant.x == ant.x && current_node->ant.y == ant.y){
                temp_node = current_node->next;
		current_node->next = NULL;
		current_node = temp_node;
            }
            current_node = current_node->next;
	}
    }
    return;
}

void display_list(AntList** head){
	AntList* current_node = *head;
	while(current_node){
		printf("[%lf][%lf]-->", current_node->ant.x, current_node->ant.y);
		current_node = current_node->next;
	}
	printf("NULL\n");
}

void free_ant_list(AntList** head){
   /* deref head_ref to get the real head */
   AntList* current = *head;
   AntList* next;
 
   while (current != NULL) 
   {
       next = current->next;
       free(current);
       current = next;
   }
   
   /* deref head_ref to affect the real head back
      in the caller. */
   *head = NULL;
}
