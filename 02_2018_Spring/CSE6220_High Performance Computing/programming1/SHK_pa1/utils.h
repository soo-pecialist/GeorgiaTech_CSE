/**
 * @file    utils.h
 * @author  Patrick Flick <patrick.flick@gmail.com>
 * @brief   Implements common utility/helper functions.
 *
 * Copyright (c) 2014 Georgia Institute of Technology. All Rights Reserved.
 */

/*********************************************************************
 *             You can add new functions to this header.             *
 *********************************************************************/

#ifndef UTILS_H
#define UTILS_H

#include <mpi.h>

/*********************************************************************
 * DO NOT CHANGE THE FUNCTION SIGNATURE OF THE FOLLOWING 4 FUNCTIONS *
 *********************************************************************/

inline void get_proc_info(int* p, int* rank, MPI_Comm comm){
    MPI_Comm_rank(comm, rank);
    MPI_Comm_size(comm, p);
}

inline int block_decompose(const int n, const int p, const int rank){
    return n / p + ((rank < n % p) ? 1 : 0);
}

inline int block_decompose(const int n, MPI_Comm comm){
    int rank, p;
    get_proc_info(&p, &rank, comm);
    return block_decompose(n, p, rank);
}

inline int get_local_array_start_pos(int n, int p, int local_rank){
    int pos=0;
    for(int i=0; i<p; i++){
        if (local_rank==i){
            break;
        }
        pos+=block_decompose(n,p,i);
    }
    return pos;
}

/*********************************************************************
 *                  DECLARE YOUR OWN FUNCTIONS HERE                  *
 *********************************************************************/

// ...


#endif // UTILS_H
