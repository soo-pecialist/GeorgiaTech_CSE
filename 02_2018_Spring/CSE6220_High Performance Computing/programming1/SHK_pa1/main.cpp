/**
 * @file    main.cpp
 * @author  Saminda Wijeratne <samindaw@gatech.edu>
 * @brief   Implements the main function for the 'grade calculator' executable.
 *
 * Copyright (c) 2018 Georgia Institute of Technology. All Rights Reserved.
 */

/*********************************************************************
 *                  !!  DO NOT CHANGE THIS FILE  !!                  *
 *********************************************************************/


#include <mpi.h>

#include <stdexcept>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cstring>

#include "utils.h"
#include "io.h"
#include "mpi_score_analyser.h"
#include "score_analyser.h"

void print_usage(char *exec){
    std::cerr << "Usage:    "<<exec<<" -f <filename>" << std::endl;
    std::cerr << "                  Read student scores from the file <filename>" << std::endl;
    std::cerr << "                  When run in parallel, the processor 0 will " << std::endl;
    std::cerr << "                  read all the data and distribute it among other" << std::endl;
    std::cerr << "                  processors" << std::endl;
    std::cerr << "          "<<exec<<" -n <n> [-s <seed>]" << std::endl;
    std::cerr << "                  Creates random scores of size <n> with the " << std::endl;
    std::cerr << "                  the given positive <seed>, as the seed for" << std::endl;
    std::cerr << "                  the random number generator (default seed=1)." << std::endl;
    std::cerr << "                  When run in parallel, each processor will " << std::endl;
    std::cerr << "                  create its own share of random numbers" << std::endl;
}
void print_usage_and_exit(char *exec){
    print_usage(exec);
    MPI_Finalize();
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    
    // set up MPI
    MPI_Init(&argc, &argv);

    // get communicator size and my rank
    MPI_Comm comm = MPI_COMM_WORLD;
    int p, rank;
    get_proc_info(&p, &rank, comm);
    
    //random seed
    int seed = 1;

    int local_n;
    double* local_scores = NULL;
    int* grade_count = NULL;
   
    // analyze command line parameters
    {
        if (argc < 3){
           if (rank == 0){
              print_usage_and_exit(argv[0]);
           }
        }
        
        if (std::string(argv[1]) == "-n"){
           // randomly generate input
            int n = atoi(argv[2]);
            if (!(n > 0)){
              print_usage_and_exit(argv[0]);
            }

            if (argc == 5){
                // determine the seed value for randomize engine
                if (std::string(argv[3]) != "-s"){
                    print_usage_and_exit(argv[0]);
                }
                std::istringstream iss(argv[4]);
                iss >> seed;
                if (seed <= 0){
                    print_usage_and_exit(argv[0]);
                }
            }
            
            //create random numbers in each processor
            local_n = block_decompose(n, p, rank);
            local_scores = (double*) malloc(sizeof(double)*local_n);
            randn(local_scores, local_n, 0.0, 100.0, seed*(rank+1));
        }else if (std::string(argv[1]) == "-f"){
            // load input from file
            if (rank == 0){
                // processor 0 will read the data in file and distribute
                std::string input_file = std::string(argv[2]);
                std::vector<double> data = read_text_file(input_file.c_str());
                int n = data.size();
                if (p > 1){
                    //parallel version: distribute the data from processor 0
                    mpi_distribute_scores(&data[0], n, &local_scores, &local_n, comm);
                } else {
                    //serial version
                    local_n = n;
                    local_scores = (double*) malloc(sizeof(double)*local_n); 
                    memcpy(local_scores,&data[0],sizeof(double)*local_n);
                }         
                data.clear();
            }else{
                // retrieve data from processor 0
                mpi_distribute_scores(NULL, 0, &local_scores, &local_n, comm);
            }
        }else{
            print_usage_and_exit(argv[0]);
        }
    }
   
   // start timer
   //   we omit the file loading and argument parsing from the runtime
   //   timings, we measure the time needed by the processor with rank 0
   struct timespec t_start, t_end;
   if (rank == 0)
      clock_gettime(CLOCK_MONOTONIC,  &t_start);
   int count=-1;
   if (p > 1){
       //parallel version
       mpi_calculate_grade_count(local_scores, local_n, &grade_count, &count, comm);

   }  else  {
       // sequential version
       std::cerr << "[WARNING]: Running the sequential solver. Start with mpirun to execute the parallel version." << std::endl;
       calculate_grade_count(local_scores, local_n, &grade_count, &count);
   }
          
   if (rank == 0){
      // end timer
      clock_gettime(CLOCK_MONOTONIC,  &t_end);
      // time in seconds
      double time_secs = (t_end.tv_sec - t_start.tv_sec)
         + (double) (t_end.tv_nsec - t_start.tv_nsec) * 1e-9;
      // output time
      std::cerr << time_secs << std::endl;
      
      // output results
      if (count > 0 && valid(grade_count)){
        for (int i=0; i < count; i++){
            std::cout << i << ": "<< grade_count[i]<< std::endl;
        }
      } else {
          std::cerr << "No result was generated!!!"<< std::endl;
      }

   }
   
   // free allocated memory
   if (valid(local_scores))
        free(local_scores);
   if (valid(grade_count))
        free(grade_count);

   // finalize MPI
   MPI_Finalize();
   return 0;
}
