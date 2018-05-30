/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   engine.h
 * Author: rahul
 *
 * Created on October 21, 2017, 8:45 AM
 */

#ifndef ENGINE_H
#define ENGINE_H

    
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#if 0
	#define DEBUG(a) printf a
#else
	#define DEBUG(a) (void)0
#endif


#if 1
	#define INFO(a) printf a
#else
	#define INFO(a) (void)0
#endif

typedef struct BoxID{
    int x;
    int y;
}BoxID;

typedef struct Ant{
    BoxID boxID; //which box the ant belongs to 
    double x; //position of the ant in the horizontal direction
    double y; //position in the vertical direction
    double v_x; //velocity in the horizontal direction
    double v_y; //velocity in the vertical direction
    double v_x_old; //previous velocity in the horizontal direction
    double v_y_old; //previous velocity in the vertical direction

} Ant;

typedef struct AntList{
    Ant ant;
    struct AntList* next;
}AntList;

typedef struct ViscekBox{
    AntList* antList;
    int numAnts;
} ViscekBox;

void usage(const char *programName);
double min(double a, double b);
int ant_overflow_check(int num_ants, double ant_min_size, double container_width, double container_height); //check if the entered amount of ants can be contained within the container. If overflow occurs then keep only the maximum capacity of the container.
Ant* malloc_ant_array(int num_ants);
void init_ant_array(int num_ants, double ant_min_size, double container_width, double container_height, double radius, Ant ant[]);
ViscekBox** malloc_box_matrix(int num_rows, int num_cols);
void init_box_matrix(ViscekBox **box, int num_rows, int num_cols);
void free_box_matrix(int num_rows, int num_cols, ViscekBox **boxMat);
void populate_box_matrix(ViscekBox **box, Ant ant[],  int num_ants);
double rand_in_range(double min, double max); // Return a random double within the given range
void print_ant_stats(Ant *ant, int num_ants);
void print_box_stats(ViscekBox **boxMat, int num_rows, int num_cols);

//Linked list operations
void add_front(AntList** head, Ant ant);
void remove_link(AntList** head, Ant ant);
void display_list(AntList** head);
void free_ant_list(AntList** head);

#endif /* ENGINE_H */

