SooHyeon Kim (GTID#: 903230437)

<Contents>
1. All scripts are under “./code/rgGenerator
2. Helper folder holds helper scripts 
3. report.pdf
4. readme.txt


**VERY IMPORTANT: 
	if you use ‘makefile’ on the local terminal use ‘CC=gcc-7’ 
	and on the server use ‘CC =  gcc’ 
	otherwise, you’ll get errors. 
==================Compiling (on your terminal)================

input command ‘nano Makefile’ and check if CC = gcc-7

#To compile:
(make clean)
make

Two binary files will be generated: ‘rgGenerator’ and ‘paraTest’

rgGenerator can be used in an interactive mode while paraTest can be used to test the parallel performance
=================== Usage ==============
#Usage: ./rgGenerator -n N -p P <-t T> <-d>,

where:
   N: number of nodes (vertices)
   P: edge generation probability
   T: number of threads (optional)
  -d: dump the graph to a file

example:

#will use 5 threads and dump the graph to a file (in adjacency matrix form)
./rgGenerator -n 5 -p 0.5 -t 4 -d

# will not write to a file
/rgGenerator -n 5 -p 0.5 -t 4 

#will use the default number of thread (1)
/rgGenerator -n 5 -p 0.5 

To use the paratest, just type:
./parates

===================== Executing the code on the server    =========
Please follow these steps:

(1) copy the contents of the project folder to the server on your terminal 
    (*try following command on the same level of regenerator directory)
    (e.g., scp -r "rgGenerator" skim3026@flamel.pace.gatech.edu:~)
(2) access @flamel.pace.gatech.edu server
(3) cd into the folder (cd ./rgGenerator)
(4) hit command ‘nano Makefile’ & check if CC = gcc (otherwise, you’ll get errors)
(5) command ‘make’ (or ‘make clean’ and ‘make’)
(6) qsub paratest.pbs 
(7) wait for the report to be generated 
	- it will send you message such as: 17999826.shared-sched.pace.gatech.edu
	- hit ‘ls’ and then you can see files having same number 
	  (** it may take several minutes to show result files)
	  (e.g., rgGenerator.e17999826 & rgGenerator.o17999826)
(8) look through the result using ‘nano’
	(e.g., nano rgGenerator.o17999826)

