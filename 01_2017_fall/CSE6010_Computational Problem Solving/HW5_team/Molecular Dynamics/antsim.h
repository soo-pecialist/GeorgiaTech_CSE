/*
 * antsim.h
 *
 *  Created on: Nov 13, 2017
 *      Author: Caleb
 */

#ifndef ANTSIM_H_
#define ANTSIM_H_

#include "engine.h"

void 		interact			(Ant* a, double width, double height, ViscekBox** boxMat, double R,int numBoxRows,int numBoxCols);
void 		walls				(Ant* a, double width, double height);
int 		motion_pressure		(Ant* a, ViscekBox* neighbors, double motionpref, double waitnoise, double anglenoise);
void 		direction_pressure	(Ant* a, ViscekBox* neighbors);
void 		gravity				(Ant* a, ViscekBox* neighbors, double leg_length);
void		hard_body			(Ant* a, ViscekBox* neighbors, double radius);
void		noise				(Ant* a, double strength);
ViscekBox*	find_neighbors		(Ant* a, ViscekBox** boxMat, double R, int numBoxRows, int numBoxCols);
double 		normal_rand			(double mu, double sigma);










#endif /* ANTSIM_H_ */

