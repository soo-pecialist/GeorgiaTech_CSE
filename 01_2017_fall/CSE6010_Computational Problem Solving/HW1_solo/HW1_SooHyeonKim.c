//
//  main.c
//  CSE-6010 Assignment1
//
//  Created by Karl Gemayel on 8/17/17.
//  Copyright © 2017 Karl Gemayel. All rights reserved.
//
//  Student name: Soo Hyeon Kim
//  Student username: skim3026 (#:903230437)

#include <stdio.h>
#include <stdlib.h>     // for rand, srand
#include <time.h>       // for time, clock
#include <string.h>     // for strncmp

// Use these variables for simplicity of code
#define RED 'R'
#define GREEN 'G'
#define VACANT ' '

// Function Prototypes: generic
void usage(const char *programName);

// Function Prototypes: matrix specific
char**      malloc_matrix   		(int n1, int n2);							// allocate memory to matrix
void        free_matrix     		(int n1, int n2, char **a);					// free memory allocated to matrix
void        init_matrix     		(int n1, int n2, char **a);					// initialize matrix with random values (' ', R, G)
int         simulate        		(int n1, int n2, char **a, double f);  		// update matrix
void        print_matrix    		(int n1, int n2, char **a);					// print out matrix
void 		matrix_intepretor	(int n1, int n2, char **a); 					// for plotting purpose
int 			not_happy 			(int n1, int n2, double f, char** a); 		// how many are unhappy
int 			are_you_red			(int i, int j, char **a); 					// calculate how many neighbors are red
int 			are_you_green    	(int i, int j, char **a);					// calculate how many neighbors are green
void 		to_csv				(char *fname,int n1,int n2, char **a);		// write into .csv file
// Main function
int main(int argc, const char * argv[]) {
	/*
    // Start by reading the input from the command line prompt.
    // Note: when no additional arguments are passed through the
    // the command prompt, argc equals 1, and argv[0] is the
    // name of our executable file.

    // In this case, the program takes in 3 addition arguments:
    // (1) number of rows, (2) number of columns, (3) number of
    // times to run 'simulate', and (4) value of f
     */
    if (argc != 5) {
        usage(argv[0]);
        return 1;
    }
// defaults argvs
    int n1 = (int) strtol(argv[1], NULL, 10);                   // number of rows
    int n2 = (int) strtol(argv[2], NULL, 10);                   // number of columns
    int s = (int) strtol(argv[3], NULL, 10);       // number of times to run 'simulate'
    double f = strtod(argv[4], NULL);                           // the satisfiability fraction
// for Test run
	//	int n1 = 20;
	//	int n2 = 20;
	//	int s = 20;
	//	double f = 0.3;
// other argvs
    int i = 0;
    int change = 0;   				// this takes simulate() return - how many changes have occurred
    int unhappys = 0; 				// this takes not_happy() return - how many changes are required
// mat declaration
    char** mat = malloc_matrix(n1,n2); // record matrix
//    if(mat == '-1'){ return -1; } // memory shortage -> end program

    // initialize matrix
    init_matrix(n1, n2, mat);

// check if inputs are okay
    if (n1 == 0 || n2 == 0 || s == 0 || f == 0) // if non-number & 0
	{
		printf("INVALID INPUTS!\n");
		fflush(stdout);
		printf("Input integers and floating numbers accordingly (>0)");
		fflush(stdout);
		return -1;
	}
    else //inputs are fine - running part here
	{
		printf("Program initiated ...\n\n");
		printf("  <Round %d>: Original City\n", 0);
		print_matrix(n1, n2, mat);

//TODO: uncomment only when file needed
//		 // create 'original'.csv ----------------------------------------
//				char fname[100];
//				printf("\n Enter the filename for original city : ");
//				fgets(fname, 1000, stdin);
//				fname[strlen(fname) - 1] = '\0';
//
//				to_csv(fname, n1, n2, mat);
//		// ----------------------------------------------------


		printf("\n= = = = = = = = = = = = = = = = = = = = = = = = = = =  \n");
	//------------------------------------------
		for (i=1; i<s+1; i++)
		{
			unhappys = not_happy(n1, n2, f, mat);
			//get change number via simulate
			change = simulate(n1, n2, mat, f);
			// Error check
			if(change == -1)
			{
				printf("\n   ERROR: something is not right. Try again");
				break;
			}

			if (unhappys == 0) // everybody is happy
			{
				printf("  <Round %d>\n", i);
				print_matrix(n1, n2, mat);

				printf("  - # of change: %d\n", change);
				printf("\tAll is well. Nice work!\n\n");
				break; // end loop
			}
			else // some are not happy
			{

				printf("  <Round %d>\n", i);
//TODO: uncomment if want to see the PROGRESS --------------------
//				print_matrix(n1, n2, mat);
// ---------------------------------------------------------------

				printf("  - # of change: %d\n", change);

			}
			printf("= = = = = = = = = = = = = = = = = = = = = = = = = = =  \n");
		} // end for

		if(i >= s+1){printf("[Program has stopped because it has reached maximum repetition # of '%d']\n\n", s);}

		printf("  <Summary>\n");
		printf("   - City cize: %d x %d\n", n1, n2);
		printf("   - Maximum repetition: %d\n", s);
		printf("   - Fraction of same neighbors: %.2f\n", f);
		printf("   - Repetition occurred: %d time(s)\n", i-1);
		printf("= = = = = = = = = = = = = = = = = = = = = = = = = = =  \n");
	} // end else

 //TODO: uncomment only when file needed
// // create 'result'.csv ----------------------------------------
//		char str[100];
//		printf("\n Enter the filename for end result: ");
//	//    gets(str);
//		fgets(str, 1000, stdin);
//		str[strlen(str) - 1] = '\0';
//
//		to_csv(str, n1, n2, mat);
//// ----------------------------------------------------

    free_matrix(n1, n2, mat); // free memory
	printf("\n Memory freed.\n");
	printf(" Program has ended.\n");

    return 0;
}

/**
 * Print out the usage message.
 *
 * @param programName the name of the executable file.
 */
void usage(const char *programName) {
    printf("usage: %s  n1 n2 s f\n", programName);
    printf("where:\n");
    printf("    n1      : number of rows\n");
    printf("    n2      : number of columns\n");
    printf("    s       : number of simulation iterations\n");
    printf("    f       : minimum fraction for satisfiability\n");
    printf("\nExample: %s 10 10 100 0.3\n", programName);
}

/****** Matrix Specific Functions ******\
 *                                     *
 * These functions define what matrix  *
 * operations can be performed.        *
 *                                     *
\***************************************/


/**
 * Allocate memory for a 2D matrix
 * @param n1 the number of rows
 * @param n2 the number of columns
 * @return a pointer to the matrix
 */
char **malloc_matrix(int n1, int n2)
{
	char **mat = (char**) malloc((n1+1) * sizeof(char *));       // somehow requires buffer slots
																// b.c. when debugging, there's loss of block

	for (int i=0; i < n1+2; i++)
	{
			mat[i] = (char*) malloc((n2+1) * sizeof(char));
	}
// check if memory is full
	if (mat == NULL)
	{
			printf("Error: MEMORY IN SHORT");
	}
// if no problem

	return mat;
}


/**
 * Free the space allocated for the matrix.
 * @param n1 the number of rows
 * @param n2 the number of columns
 * @param a the pointer to the matrix
 */
void free_matrix(int n1, int n2, char **a) {

	for(int i=0; i< n1; i++)
		free((void *) a[i]);
	free((void *) a);
}


/**
 * Initialize the matrix.
 * @param n1 the number of rows
 * @param n2 the number of columns
 * @param a the pointer to the matrix
 */
void init_matrix(int n1, int n2, char **a) {
	srand((unsigned)time(NULL)); //random seed generation
//Let's shake up composition of citizens considering possibility of races

	int num; // take randomly pick values
	char* p_neighbor = malloc((n1*n2)*sizeof(char));
	int i=0, j=0;

	for (i = 0; i < n1*n2; i++) // length of n1*n2 array in which each pointer holds race value(R, G, V)
	{
		// P('R') = 0.4 ; P('G') = 0.4; P(' ') = 0.2
		num = rand()%10; // num is 0~9
		if (num >= 0 && num < 4) 		// 0, 1, 2, 3 -> RED
			*(p_neighbor + i) = RED; 	// 'R'
		else if (num >= 4 && num < 8)	// 4, 5, 6, 7 -> GREEN
			*(p_neighbor + i) = GREEN; 	// 'G'
		else								// 8, 9		  -> VACANT
			*(p_neighbor + i) = VACANT; // ' '
	} // end for


	/* Matrix concept (R: Red, G: Green, V: Vecant)
	 *  --> this way, it's easy to check neighbors
	 * 			 0th               (n2+1)th
	 * 		0th:	  V V V V V V V V V V
	 *			  V R G R G R G R G V
	 *			  V G R V R V R V R V
	 *			  V R G R G R G R G V
	 *			  V G R V R V R V R V
	 *			  V R G R G R G R G V
	 *			  V G R V R V R V R V
	 *			  V R G R G R G R G V
	 *			  V G R V R V R V R V
	 *	(n1+1)th: V V V V V V V V V V
	 */

	// need to put ' ' in buffer rows(0 & n1+1) & columns(0 & n2+1)
	for (int i = 0; i < n1+2; i++)
	{
		*(*(a+i)) = ' ';
		*(*(a+i)+n1+1) = ' ';
	}
	for (int j = 1; j < n2+1; j++)
	{
		*(*(a)+j) = ' ';
		*(*(a+n2+1)+j) = ' ';
	}


	// let's put those values into matrix using pointer concept
	for (i = 1; i < n1+1; i++) // remember a* starts from 01 address, not 00
	        for (j = 1; j < n2+1; j++) // remember a** starts from 01 address, not 00
	            *(*(a+i)+j) = *(p_neighbor+(i-1)*n2+j-1);

	free(p_neighbor); // free memory
	// TODO different from original

// TODO: Remove this logic - somehow not compatible well with pointers
//	int num; // take randomly pick values
//
//	for (int i=0; i< n1; i++)
//	{
//		for (int j=0; j< n2; j++)
//		{
//			num = rand()%10; // num is 0~9
//			if (num >= 0 && num < 4) 		// 0, 1, 2, 3 -> RED
//				a[i][j] = RED;
//			else if (num >= 4 && num < 8) 	// 4, 5, 6, 7 -> GREEN
//				a[i][j] = GREEN;
//			else								// 8, 9		  -> VACANT
//				a[i][j] = VACANT;
//
//		} // end for#2
//	} // end for #1
}

/**
 * Print the matrix to standard output
 * @param n1 the number of rows
 * @param n2 the number of columns
 * @param a the pointer to the matrix
 */
void print_matrix(int n1, int n2, char **a)
{
    for (int i=1; i<n1+1; i++)
    {
        for(int j=1; j<n2+1; j++)
        {
            if (*(*(a+i)+j) == RED) {
                printf(" %c", 'R');
            }
            if (*(*(a+i)+j) == GREEN) {
                printf(" %c", 'G');
            }
            if (*(*(a+i)+j) == VACANT) {
                printf(" %c", ' ');
            }
        } // end inner for
        printf("\n");
        fflush(stdout);
    } // end outer for
} // end print_matrix()

void matrix_intepretor(int n1, int n2, char **a) // Later for coloring cells
{
	for (int i=1; i<n1+1; i++)
	    {
	        for(int j=1; j<n2+1; j++)
	        {
	            //1: R, 2: G, 3: V
	            if (*(*(a+i)+j) == RED) { printf("%d,", 1);}
	            else if (*(*(a+i)+j) == GREEN) { printf("%d,", 2);}
	            else if (*(*(a+i)+j) == VACANT) { printf("%d,", 3);}
	        }
	        printf("\n");
	    }
}

int are_you_red(int i, int j, char **a) // return red neighbor number
{
	int hi_reds = 0;  // gets the number of 'R' neighbors (# of neighbors can be: 3, 5, 8)
	hi_reds = ((*(*(a+i-1)+j-1)=='R') + (*(*(a+i-1)+j)=='R') + (*(*(a+i-1)+j+1)=='R')
			+ (*(*(a+i)+j-1)=='R') + (*(*(a+i)+j+1)=='R') + (*(*(a+i+1)+j-1)=='R')
			+ (*(*(a+i+1)+j)=='R') + (*(*(a+i-1)+j+1)=='R'));

	return hi_reds;
}

int are_you_green(int i, int j, char **a) // return green neighbor number
{
	int hi_greens = 0;
	hi_greens = ((*(*(a+i-1)+j-1)=='G') + (*(*(a+i-1)+j)=='G') + (*(*(a+i-1)+j+1)=='G')
			+ (*(*(a+i)+j-1)=='G') + (*(*(a+i)+j+1)=='G') + (*(*(a+i+1)+j-1)=='G')
			+ (*(*(a+i+1)+j)=='G') + (*(*(a+i-1)+j+1)=='G'));

	return hi_greens;
}

int	not_happy (int n1, int n2, double f, char** a) // return unhappy citizen numbers
{
	int i = 0, j = 0;
	int reds = 0, greens = 0; // neighbor numbers
	int tot_n = 0; // total neighbor numbers
	double r_ratio = 0, g_ratio =0; // ratio of r, g neighbors
	int unhappys = 0;

	// let's take a loop and check how many are not happy
	for ( i = 1; i < n1+1; i++)
	{
		for (j = 1; j < n2+1; j++)
		{
			// if I don't initialize values in init_matrix in buffer values, error occurs
			reds = are_you_red(i,j,a);
			greens = are_you_green(i,j,a);
			tot_n = reds + greens;
			r_ratio = (double)reds / tot_n;
			g_ratio = (double)greens / tot_n;

			// 1) if vacant -> do nothing
			// 2) if red
				if ((*(*(a+i)+j) == 'R') && r_ratio < f){ unhappys++; }
			// 3) if green
				if ((*(*(a+i)+j) == 'G') && g_ratio < f){ unhappys++; }
		} // end inner for
	} // end outer for

	return unhappys;
} // end not_happy()

/**
 * Simulate one time-step using matrix 'a', where f determines
 * the minimum number of desired neighbors similar to the occupant.
 * @param n1 the number of rows
 * @param n2 the number of columns
 * @param a the pointer to the matrix
 * @param f the fraction indicating the minimum number of desired similar neighbours
 * @return the number of citizens that moved or left the game in this time-step; -1 if
 * function failed.
 */
int simulate(int n1, int n2, char **a, double f)
{
/* return value
 *  0: all set
 *  n(>0): number of citizens moving or leaving
 */

/* <GAME PLAN>
 * 1) create array(recorder) to store position of unhappy cell
 * 2) store unhappy cell in recorder by looping through matrix a
 * 3) randomly pick starting position to search vacant cells
 * 4) if find vacant cell, move unhappy citizen to the cell
 * 5) if happy, let the person stay there & previous cell becomes vacant
 * 6) if can't find satisfactory vacant cell, evict the citizen, and the cell updates to "VACANT"
 * 7) return "evicted" + "moved" citizen number
 */
	char** recorder = malloc((n1*n2)*sizeof(char*));
	if (recorder == NULL)
	{
		printf("Error: MEMORY IN SHORT");
		return -1;
	}

	// arguments
	int e = 0, m = 0; // e: evicted number, m: moving number
	int l = 0; // length of recorder array
	int i = 0, j = 0;
	int reds = 0, greens = 0; // neighbor numbers
	int tot_n = 0; // total neighbor numbers
	double r_ratio = 0, g_ratio =0; // ratio of r, g neighbors


// step 1) find unhappy's
	for (i = 1; i<n1+1; i++)
	{
		for (j = 1; j<n2+1; j++)
		{
			reds = are_you_red(i, j, a);
			greens = are_you_green(i, j, a);
			tot_n = reds+greens;
			r_ratio = (tot_n != 0)? (double)reds / tot_n : 1;
			g_ratio = (tot_n != 0)? (double)greens / tot_n : 1;

			// 1) if vacant -> do nothing
			// 2) if red
				if ((*(*(a+i)+j) == 'R') && r_ratio < f)
				{
					*(recorder+l) =  *(a+i)+j; // store the address of the unhappy cell
					l++; 				 // move on
				}
			// 3) if green
				if ((*(*(a+i)+j) == 'G') && g_ratio < f)
				{
					*(recorder+l) =  *(a+i)+j; // store the address of the unhappy cell
					l++; 				 // move on
				}
		} // end inner for
	} // end outer for

	// printf("not happy! %p\n", *(recorder-1)) // where the last address is stored

// step 2) track down unhappys' location
	// starting cell randomly picked
	for (int rec = 0; rec < l; rec++)
	{
		char curr = ' '; // store current value: 'G', 'R', ' '
		char in_recorder = **(recorder + rec); // unhappy cell's value
		int pos_i = rand() % n1 + 1; // (1, ... , n1)
		int pos_j = rand() % n2 + 1;	// (1, ... , n2)
	 // int rec - to move recorder pointer
		int size = n1*n2; // maximum repetition number
		int reds = 0, greens = 0; // neighbors in color
		int tots = 0; // sum of reds and greens
		double g_f, r_f; // ratio of green, red
		int is_moving = 0; // check if the move happened

	// e+m : total change
		i = pos_i;
		j = pos_j;  // starting point, when reach (pos_i, pos_j) -> terminates
		// we re in for (rec) yet --------------- -----
		while(size > 0) // will move with i & j
		{
			size--;
			if(l == 0){ //everybody is happy
				free(recorder);
				return e+m; // 0
			}

			curr = *(*(a+i)+j); // current cell color
				// we can ignore when curr is R or G
			if (curr == ' ') // if vacant
			{
				reds = are_you_red(i, j, a);
				greens = are_you_green(i, j, a);
				tots = reds+greens; // when reds&greens =0 -> make g_f & r_f happy 1
				g_f = (tots!=0)? (double) greens/tots:1;
				r_f = (tots!=0)? (double) reds/tots:1;

				if (in_recorder == 'R' && r_f >= f) // when put red -> happy
				{
					**(recorder + rec) = ' '; // leave
					*(*(a+i)+j) = 'R'; 		 // move in
					is_moving = 1;
				}

				if (in_recorder == 'G' && g_f >= f) // when put red -> happy
				{
					**(recorder + rec) = ' '; // leave
					*(*(a+i)+j) = 'G'; 		 // move in
					is_moving = 1;
				}

				// move on to next cell
				j++;
				// if the cell was end-cell of matrix -> move to (0,0)
				if ((j > n2) && (i = n1))
				{
					i=1;
					j=1;
//					continue;
				}
				// if j goes beyond last column index (n2-1) -> row++ & col:1
				if ((j>n2) && (i != n1))
				{
					i++;
					j=1;
//					continue;
				}

			} // end if(curr)

		}// end while(size)
		if(is_moving == 1) { m++;} // moving +1
		else
		{
			**(recorder+rec) = ' ';
			e++; // evicted +1
		}
	} // end for (rec)

	free(recorder);
	return m+e;
} // end simulate()

void to_csv(char *fname,int n1,int n2, char **a)
{

    FILE *fp;
    fname=strcat(fname,".csv");

    fp=fopen(fname, "w+");

//    for (int i=1; i<n1+1; i++)
//    {
//        for(int j=1; j<n2+1; j++)
//        {
//            fprintf(fp, "%c ", *(*(a+i)+j));
//        }
//        fprintf(fp, "\n");
//    }

    for (int i=1; i<n1+1; i++)
    	    {
    	        for(int j=1; j<n2+1; j++)
    	        {
    	            //1: R, 2: G, 3: V
    	            if (*(*(a+i)+j) == RED) { fprintf(fp, "%d,", 1);}
    	            else if (*(*(a+i)+j) == GREEN) { fprintf(fp, "%d,", 2);}
    	            else if (*(*(a+i)+j) == VACANT) { fprintf(fp, "%d,", 3);}
    	        }
    	        fprintf(fp, "\n");
    	    }

    fclose(fp);

    printf(" '%s' is created \n",fname);
}














