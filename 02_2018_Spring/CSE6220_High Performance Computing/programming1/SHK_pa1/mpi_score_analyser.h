/**
 * @file    mpi_score_analyser.h
 * @author  Saminda Wijeratne <samindaw@gatech.edu>
 * @brief   Define parallel functions required to calculate grades
 *
 * Copyright (c) 2018 Georgia Institute of Technology. All Rights Reserved.
 */

/*********************************************************************
 *                  !!  DO NOT CHANGE THIS FILE  !!                  *
 *********************************************************************/

#include <mpi.h>

#ifndef MPI_SCORE_ANALYSER_H
#define MPI_SCORE_ANALYSER_H

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * Distribute the complete list of scores in the same order and equally 
     * between all the processors (i.e. block decompose)
     * @param all_scores    Set of all student scores. This is valid only in 
     *                      proc 0. 
     * @param n             Total number of student scores. This is valid only  
     *                      in proc 0.
     * @param scores        Pointer to a memory location to save the memory
     *                      location for the local array that would contain
     *                      the local processor student scores
     *                      (memory to this array is not allocated. you need 
     *                      to allocate it using malloc function.
     *                        eg: *scores = (double*) malloc(sizeof(double) * n_);)     
     * @param local_n       pointer to an integer to save the total number of 
     *                      student scores stored in current processor.
     *                        eg: *no_of_grades = n_;)     
     * @param comm
     */
    void mpi_distribute_scores(double* all_scores, int n, double** scores, int* local_n, MPI_Comm comm);
    
    /**
     * Calculate how many students achieved each grade and return it to proc 0
     * @param scores        Score of each student in the processor
     * @param n             no of students in each processor
     * @param grade_counts  A pointer to an array to store the grade count including
     *                      0 count grades. Each index of the array would correspond
     *                      to the grade map defined for mpi_calculate_grades(...) func
     *                      (memory to this array is not allocated. you need 
     *                      to allocate it using malloc function.
     *                        eg: *grade_counts = (int*) malloc(sizeof(int) * 5);)
     * @param no_of_grades  Int pointer to save number of grades. 
     *                      i.e. no of elements in grade_count array. 
     *                        eg: *no_of_grades = 5;)          
     * @param comm
     */
    void mpi_calculate_grade_count(double* scores, int n, int** grade_counts, int* no_of_grades, MPI_Comm comm);

    //--------------------------Helper Functions---------------------------//
    /**
     * Following functions will be useful when implementing above functions *
     *                                                                      */
    
    /**
     * Calculate the maximum score among all students
     * @param scores        Score of each student in the processor
     * @param n             Number of students in the processor
     * @param comm
     * @return the max score
     */
    double mpi_get_max_score(double* scores, int n, MPI_Comm comm);
    
    /**
     * Calculate the minimum score among all students
     * @param scores        Score of each student in the processor
     * @param n             Number of students in the processor
     * @param comm
     * @return the min score
     */
    double mpi_get_min_score(double* scores, int n, MPI_Comm comm);

    /**
     * Calculate the average score among all students
     * @param scores        Score of each student in the processor
     * @param n             Number of students in the processor
     * @param comm
     * @return the average score
     */
    double mpi_get_average_score(double* scores, int n, MPI_Comm comm);
    
    /**
     * Calculate the grades for each of the score
     * @param scores        Score of each student in the processor
     * @param n             Number of students in the processor
     * @param grades        Array to store the Grade for each student 
     *                      in the processor. A grade is represented 
     *                      by an integer value
     *                      (eg:    A - 0
     *                              B - 1
     *                              C - 2
     *                              D - 3
     *                              F - 4)
     * @param comm
     */
    void mpi_calculate_grades(double* scores, int n, int* grades, MPI_Comm comm);
    
    //---------------------------------------------------------------------//
#ifdef __cplusplus
}
#endif

#endif /* MPI_SCORE_ANALYSER_H */

