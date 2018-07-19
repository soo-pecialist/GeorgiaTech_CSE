/**
 * @file    io.h
 * @author  Patrick Flick <patrick.flick@gmail.com>
 * @brief   Implements common IO and input generation functions.
 *
 * Copyright (c) 2014 Georgia Institute of Technology. All Rights Reserved.
 */

/*********************************************************************
 *                  !!  DO NOT CHANGE THIS FILE  !!                  *
 *********************************************************************/

#ifndef IO_H
#define IO_H

#include <fstream>
#include <iterator>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <limits>
#include <cmath>


std::vector<double> read_text_file(const char* filename){
    std::ifstream in(filename, std::ios::binary | std::ios::ate);
    if (!(in.good() && in.is_open()))
        throw std::runtime_error(std::string("Couldn't open file ") + filename);    
    in.close();

    std::ifstream txtf(filename);
    std::string line;
    std::vector<double> result;
    while(getline(txtf,line)){
        result.push_back(atof(line.c_str()));
    }
    txtf.close();
    return result;
}

/**
 * @brief   Reads a file into a vector in in binary mode.
 *
 * @tparam T        The value type of the vector.
 * @param filename  The filename of the input file.
 *
 * @return  A vector containing the data read from file.
 */
template<typename T>
std::vector<T> read_binary_file(const char* filename)
{
    // get file size
    std::ifstream in(filename, std::ios::binary | std::ios::ate);
    if (!(in.good() && in.is_open()))
        throw std::runtime_error(std::string("Couldn't open file ") + filename);
    std::size_t nbytes = in.tellg();
    in.close();
    // open again, this time at the beginning
    std::ifstream infile(filename, std::ios::binary);

    // create vector of the correct size
    std::size_t n = nbytes / sizeof(T);
    std::vector<T> result(n);
    // read file into vector:
    infile.read((char*) &result[0], nbytes);
    return result;
}

/**
 * @brief   Writes binary data from a vector to a file.
 *
 * @param filename  The filename of the output file.
 * @param data      Vector of templated type `T` to be written to the file as
 *                  bytes.
 */
template<typename T>
void write_binary_file(const char* filename, const std::vector<T>& data)
{
    // open file in binary mode
    std::ofstream out(filename, std::ios::binary);
    if (!(out.good() && out.is_open()))
        throw std::runtime_error(std::string("Couldn't open file ") + filename);
    // write data to file
    out.write(reinterpret_cast<const char*>(&data[0]), sizeof(T)*data.size());
    out.close();
}


// generates a random numbers between min and max values (edit: samindaw)
void randn(double* v, int n, double min, double max, int seed){
    srand(seed);
    for (int i = 0; i < n; ++i){
        v[i] = min + (max-min)*(1.0*rand()/RAND_MAX);
    }
}

int valid(void *p) {
  extern char _etext;
  return (p != NULL) && ((char*) p > &_etext);
}
#endif // IO_H
