SooHyeon Kim (GTID#: 903230437) & XIANGYU KONG(GTID#: 903322862)

<How to run program> 
There are four files - HW2.c (main() is here), kmeans_soohyeon.c, kmeans_soohyeon.h under “kmeans” folder; and KNN.c(main() is here) under “knn” folder

(1) Kmeans model
1. Access to your current directory on terminal (I used Mac)

2. Type in this command to compile: 
        gcc -g HW2.c -o run.out

2-1. if you want to directly compile and run on Terminal type in these command:
	gcc -g HW2.c -o h
        ./HW2.c arg1 arg2 arg3
	
	for example, ./HW2.c ./data/gbm-KM.norm 6 ./data/out_gbm-KM/gbm_k_6.txt

3. To run .out exec file, type in following command on Terminal:
	./run.out  n1 n2 s f 
	for example,  ./run.out ./data/gbm-KM.norm 6 ./data/out_gbm-KM/gbm_k_6.txt

4. Be aware that we have to main.c file (kmeans_soohyeon.c & KNN.c), so you have to run them separately for HW2.c(kmeans) and KNN.c.
Also, put source codes of “HW2.c”, “kmeans_soohyeon.c”, “kmeans_soohyeon.h” under same directory so they can reference each other. 

(2) kNN model
1. Open the Terminal in the computer, find the directory which contains the C program. 

	Then type "gcc -o KNN KNN.c" to compile the program.

2. Instructions on how to run the program
	
	 type "./KNN inputfile1, inputfile2, outputfile, k " in the Terminal

	 the program takes 4 inputs and all the inputs must be provided in the following 3. order above:

	replace the inputfile1, inputfile2, output, k with the value you want to input:

	e.g., 
	-gbm-KM_out.txt	dataset that produced by the K-means program(used as train data)
	-gbm-KNN-norm	the target data set need to be classified
	-KNN_output.txt	name the file that you use to record the results produced by KNN
	-int K          the value used for KNN


	Then we can run the program.

-an Example: 

c-24-72-149-119:Desktop auguskong$ gcc -o KNN KNN.c

c-24-72-149-119:Desktop auguskong$ ./KNN gbm-KM-out.txt gbm-KNN-norm KNN_output.txt k


