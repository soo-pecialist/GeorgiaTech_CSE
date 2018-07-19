/**
 * @file    seq_tests.cpp
 * @author  Saminda Wijeratne <samindaw@gatech.edu>
 * @brief   GTest Unit Tests for the sequential code.
 *
 * Copyright (c) 2018 Georgia Institute of Technology. All Rights Reserved.
 */
/*
 * Add your own test cases here. We will test your final submission using
 * a more extensive tests suite. Make sure your code works for many different
 * input cases.
 */

#include <gtest/gtest.h>
#include <math.h>

#include "score_analyser.h"
#include "mpi_score_analyser.h"
#include "io.h"

//--------------Sample Data Set 1 for Unit Testing-----------------------//
double scores[10]={34, 24.4, 23.7, 12, 35.4, 64, 2, 45, 88, 11};
int n = 10;
// Expected Results for Sample Data Set 1
double expected_max_score=88;
double expected_min_score=2;
double expected_avg_score=33.95;
int expected_grades[10]={2, 3, 3, 3, 2, 1, 4, 2, 0, 3};
int expected_grade_count[5]={1, 1, 3, 4, 1};
int expected_number_of_grades=5;
//-----------------------------------------------------------------------//

TEST(SequentialTest, MaxValueTest) {
    double max_score = get_max_score(scores,n);
    EXPECT_EQ(expected_max_score, max_score);
}

TEST(SequentialTest, MinValueTest) {
    double min_score = get_min_score(scores,n);
    EXPECT_EQ(expected_min_score, min_score);
}

TEST(SequentialTest, AverageValueTest) {
    double avg_score = round(get_average_score(scores,n)*100)/100.0;
    EXPECT_NEAR(expected_avg_score, avg_score, 0.01);
}

TEST(SequentialTest, GradesValueTest) {
    int grades[n];
    calculate_grades(scores, n, grades);
    
    for (int i = 0; i < n; ++i){
        EXPECT_EQ(expected_grades[i], grades[i]) << " grade for student " << i << " is wrong";
    }
}

TEST(SequentialTest, GradesCountsTest) {
    int* grade_counts=NULL; int number_of_grades;
    calculate_grade_count(scores, n, &grade_counts, &number_of_grades);
    
    EXPECT_TRUE(valid(grade_counts)) << "grade_counts has not being allocated memory";
    EXPECT_EQ(expected_number_of_grades, number_of_grades);
    for (int i = 0; i < number_of_grades; ++i){
        EXPECT_EQ(expected_grade_count[i], grade_counts[i]) << " grade count for grade " << i << " is wrong";
    }
    
    if (valid(grade_counts))
        free(grade_counts);
}
