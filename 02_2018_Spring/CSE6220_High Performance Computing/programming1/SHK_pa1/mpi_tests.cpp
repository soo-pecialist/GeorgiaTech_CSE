/**
 * @file    mpi_tests.cpp
 * @ingroup group
 * @author  Saminda Wijeratne <samindaw@gatech.edu>
 * @brief   GTest Unit Tests for the parallel MPI code.
 *
 * Copyright (c) 2018 Georgia Institute of Technology. All Rights Reserved.
 */
/*
 * Add your own test cases here. We will test your final submission using
 * a more extensive tests suite. Make sure your code works for many different
 * input cases.
 *
 * Note:
 * The google test framework is configured, such that
 * only errors from the processor with rank = 0 are shown.
 */

#include <mpi.h>
#include <gtest/gtest.h>

#include <math.h>
#include <cstdlib>
#include "utils.h"
#include "io.h"
#include "mpi_score_analyser.h"

//--------------Sample Data Set 1 for Unit Testing-----------------------//
// Global array values
int n=32;
double scores[32] = {30.66, 21.68, 21.25, 14.6, 8.39, 93.43, 5.46, 13.04, 
                     65.17, 58.99, 83.22, 57.54, 71.54, 40.2, 89.89, 22.83, 
                     82.55, 92.95, 3.09, 25.46, 60.35, 47.82, 30.4, 11.82, 
                     45.1, 62.15, 11.94, 19.16, 28.07, 70.9, 86.52, 10.28};
// Local processor values (will be calculated during runtime)
int local_n;
double *local_scores;

// Expected Results for Sample Data Set 1
int expected_local_n ;
double *expected_local_scores;
double expected_max_score = 93.43;
double expected_avg_score = 43.33;
double expected_min_score = 3.09;
double expected_grades[32] = {3, 3, 3, 3, 4, 0, 4, 3, 1, 2, 0, 2, 1, 3, 0, 3,
                              0, 0, 4, 3, 1, 2, 3, 3, 2, 1, 3, 3, 3, 1, 0, 4};
double *expected_local_grades;
int expected_grade_count[5] = {6, 5, 4, 13, 4};
int expected_number_of_grades = 5;
//-----------------------------------------------------------------------//

int rank, p;
int initiated = 0;

// do a one time initiation of the variables based on the proc rank
void init(){
    if (!initiated){
        get_proc_info(&p, &rank, MPI_COMM_WORLD);
        // local size of the global array
        local_n = block_decompose(n, p, rank);
        expected_local_n = block_decompose(n, p, rank);
        
        // local portion of the global array
        int pos = get_local_array_start_pos(n, p, rank);
        local_scores = &scores[pos];
        expected_local_scores = &scores[pos];
        expected_local_grades = &expected_grades[pos];
        
        initiated=1;
    }
}

TEST(MpiTest, DataDistributeTest){ 
    init();
    // override the global variables to be used as placeholder for the result
    double *local_scores = NULL; int local_n;
    if (rank == 0){
        mpi_distribute_scores(scores, n, &local_scores, &local_n, MPI_COMM_WORLD);
    } else {
        mpi_distribute_scores(NULL, 0, &local_scores, &local_n, MPI_COMM_WORLD);
    }
    
    // perform validation
    //rank 0 should test this for all processors rather than each processor
    //tests its own because the "gtest" will only return test failures in rank 0.
    int v = valid(local_scores);
    if (rank == 0){
        // Check if all processors had allocated memory for local_scores
        int valid_memory[p];
        MPI_Gather(&v, 1, MPI_INT, valid_memory, 1, MPI_INT, 0, MPI_COMM_WORLD);
        for (int j = 0; j < p; ++j){
            EXPECT_TRUE(valid_memory[j]) << "local_scores has not being allocated memory  in processor "<<j;
        }
        
        // Check if all processors had allocated correct amount of memory
        int recv_local_n[p];
        MPI_Gather(&local_n, 1, MPI_INT, recv_local_n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        for (int j = 0; j < p; ++j){
            int expected_local_n = block_decompose(n, p, j);
            EXPECT_EQ(expected_local_n, recv_local_n[j]) << "Block decompose size for processor "<<j<<" is incorrect";
        }
        
        // Check if all processors had received the correct scores
        double rcv_local_scores[n];
        int send_displ[p];
        send_displ[0]=0;
        for(int i=1; i<p; i++){
            send_displ[i]=send_displ[i-1]+recv_local_n[i-1];
        }        
        MPI_Gatherv(local_scores, local_n, MPI_DOUBLE, rcv_local_scores, recv_local_n, send_displ, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        for (int i = 0; i < n; ++i){
            EXPECT_EQ(scores[i], rcv_local_scores[i]) 
            << " grade for student " << i << " is wrong";
        }        
    } else {
        // send the data to proc 0
        MPI_Gather(&v, 1, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);
        if (v){
            MPI_Gather(&local_n, 1, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Gatherv(local_scores, local_n, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }        
        
    }

    //free the local variable
    if (valid(local_scores))
        free(local_scores);
}

TEST(MpiTest, MaxValueTest){
    init();
    
    double max_score = mpi_get_max_score(local_scores, local_n, MPI_COMM_WORLD);
    EXPECT_EQ(expected_max_score, max_score);
}

TEST(MpiTest, MinValueTest){
    init();
    
    double min_score = mpi_get_min_score(local_scores, local_n, MPI_COMM_WORLD);
    EXPECT_EQ(expected_min_score, min_score);
}

TEST(MpiTest, AverageValueTest){
    init();
    
    double avg_score = round(mpi_get_average_score(local_scores, local_n, 
            MPI_COMM_WORLD) * 100) / 100.0;
    EXPECT_NEAR(expected_avg_score, avg_score, 0.01);
}

TEST(MpiTest, GradesValueTest) {
    init();
    
    int local_grades[local_n];
    mpi_calculate_grades(local_scores, local_n, local_grades, MPI_COMM_WORLD);
    
    //rank 0 should test this for all processors rather than each processor
    //tests its own because the "gtest" will only return test failures in rank 0.
    if (rank == 0){
        int rcv_local_grades[n];
        int send_counts[p];
        int send_displ[p];
        for(int i=0; i<p; i++){
            send_counts[i]=block_decompose(n, p, i);
            if (i==0){
                send_displ[i]=0;
            }else{
                send_displ[i]=send_displ[i-1]+send_counts[i-1];
            }
        }        
        MPI_Gatherv(local_grades, local_n, MPI_INT, rcv_local_grades, send_counts, send_displ, MPI_INT, 0, MPI_COMM_WORLD);
        for (int i = 0; i < n; ++i){
            EXPECT_EQ(expected_grades[i], rcv_local_grades[i]) 
            << " grade for student " << i << " is wrong";
        }        
    } else {
        // send the grades to proc 0
        MPI_Gatherv(local_grades, local_n, MPI_INT, NULL, NULL, NULL, MPI_INT, 0, MPI_COMM_WORLD);
    }
}

TEST(MpiTest, GradesCountsTest) {
    init();

    int* grade_counts = NULL; int number_of_grades;
    mpi_calculate_grade_count(local_scores, local_n, &grade_counts, 
            &number_of_grades, MPI_COMM_WORLD);
    
    if (rank == 0){ // the result should at least be in proc 0
        EXPECT_TRUE(valid(grade_counts)) << "grade_counts has not being allocated memory";
        EXPECT_EQ(expected_number_of_grades, number_of_grades);
//        validate_array(expected_grade_count,grade_counts,number_of_grades, rank);
        for (int i = 0; i < number_of_grades; ++i){
            EXPECT_EQ(expected_grade_count[i], grade_counts[i]) 
                    << " grade count for grade " << i<< " in processor "<< rank  
                    << " is wrong";
        }
    }
    //free allocated memory
    if (valid(grade_counts))
        free(grade_counts);
}
