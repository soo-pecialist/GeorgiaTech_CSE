/**
 * @file    score_analyser.h
 * @author  Saminda Wijeratne <samindaw@gatech.edu>
 * @brief   Define serial functions required to calculate grades
 *
 * Copyright (c) 2018 Georgia Institute of Technology. All Rights Reserved.
 */

/*********************************************************************
 *                  !!  DO NOT CHANGE THIS FILE  !!                  *
 *********************************************************************/

#ifndef SCORE_ANALYSER_H
#define SCORE_ANALYSER_H

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * Calculate how many students achieved each grade
     * @param scores        Score of each student
     * @param n             number of students
     * @param grade_counts  A pointer to an array to store the grade count including
     *                      0 count grades. Each index of the array would correspond
     *                      to the grade map defined for calculate_grades(...) func
     *                      (memory to this array is not allocated. you need 
     *                      to allocate it using malloc function.
     *                        eg: *grade_counts = (int*) malloc(sizeof(int) * 5);)
     * @param no_of_grades  No of grades. i.e. no of elements in 
     *                      grade_count array. 
     *                        eg: *no_of_grades = 5;)
     * @param comm
     */
    void calculate_grade_count(double* scores, int n, int** grade_counts, int* no_of_grades);

    //--------------------------Helper Functions---------------------------//
    /**
     * Following functions will be useful when implementing above functions *
     *                                                                      */
    
    /**
     * Calculate the maximum score among all students
     * @param scores        Score of each student
     * @param n             Number of students
     * @return the max score
     */
    double get_max_score(double* scores, int n);
    
    /**
     * Calculate the minimum score among all students
     * @param scores        Score of each student
     * @param n             Number of students
     * @return the min score
     */
    double get_min_score(double* scores, int n);

    /**
     * Calculate the average score among all students
     * @param scores        Score of each student
     * @param n             Number of students
     * @return the average score
     */
    double get_average_score(double* scores, int n);
    
    /**
     * Calculate the grades for each of the score
     * @param scores        Score of each student
     * @param n             Number of students
     * @param grades        Array to store the Grade for each student 
     *                      A grade is represented by an integer value
     *                      eg:     A - 0
     *                              B - 1
     *                              C - 2
     *                              D - 3
     *                              F - 4
     */
    void calculate_grades(double* scores, int n, int* grades);

    //---------------------------------------------------------------------//
    
#ifdef __cplusplus
}
#endif

#endif /* SCORE_ANALYSER_H */

