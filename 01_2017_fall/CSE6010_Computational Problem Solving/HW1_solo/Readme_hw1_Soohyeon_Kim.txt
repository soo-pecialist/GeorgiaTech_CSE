SooHyeon Kim (GTID#: 903230437)

** if there is a problem with my .c program, please let me know, then I’ll email you right away **

<Readme_hw1>

<references>
- I went through tutorials on what I needed for this assignment in this website
https://www.tutorialspoint.com/c_standard_library/
- I learned the meaning of “int main(int arg, const char* argv[])” here
https://forums.bignerdranch.com/t/int-main-int-argc-const-char-argv/1720
- strncmp()
https://www.tutorialspoint.com/c_standard_library/c_function_strncmp.htm
- c programming question reference
http://c-faq.com/index.html
- understand the concept of Schelling’s model
http://nifty.stanford.edu/2014/mccown-schelling-model-segregation/
- creating .txt file
http://codingstreet.com/create-csv-file-in-c/
- how to plot colormap in excel
https://www.extendoffice.com/documents/excel/849-excel-change-background-color-based-on-cell-value.html

<How to run my program> 
1. Access to your current directory on terminal (I used Mac)

2. Type in this command to compile: 
        gcc -g HW1_SooHyeonKim.c -o run.out

2-1. if you want to directly compile and run on Terminal type in these command:
	gcc -g HW1_SooHyeonKim.c -o h
        ./HW1_SooHyeonKim.c arg1(n1) arg2(n2) arg3(repetition limit) arg4(f)
	
	for example, ./HW1_SooHyeonKim.c 100 100 100 0.3

3. To run .out exec file, type in following command on Terminal:
	./run.out  n1 n2 s f 
	for example,  ./run.out 10 10 20 0.4

4. Be aware that when you type in big numbers for matrix size (n1, n2), results would not be seen on one screen. 


<Function summary>
char** malloc_matrix (int n1, int n2); 	     // allocate memory to matrix
void free_matrix (int n1, int n2, char **a); // free memory allocated to matrix
void init_matrix (int n1, int n2, char **a); // initialize matrix with random values (' ', R, G)
int simulate (int n1, int n2, char **a, double f);  // update matrix
void print_matrix (int n1, int n2, char **a);	    // print out matrix
int not_happy (int n1, int n2, double f, char** a); // how many are unhappy
int are_you_red (int i, int j, char **a);      // calculate how many neighbors are red
int are_you_green (int i, int j, char **a);    // calculate how many neighbors are green
void to_csv (char *fname,int n1,int n2, char **a); // write into .csv file


<Notice>
1) Since I have commented out .csv file creating part and print_matrix() for processing duration. If you want to create .csv file or see the progress of change of colormap layout you can simply uncomment. For easiness, I commented such part as following style. 
	//TODO: uncomment if want to see the PROGRESS --------------------
	//				print_matrix(n1, n2, mat);
	// ---------------------------------------------------------------

2) I noticed that when matrix size starts to go beyond around 300x300, it gets slowly.
For example, when the size input was 300x300, it takes around 3~4 seconds; 400x400, 5~6 seconds; 500x500, 14~18 seconds; 600x600, 40~50 seconds; 700x700 1 minute 40 ~ 50 seconds, 800x800, around 5 minutes and so on. As you can see the program is sensitive to matrix size from certain dimension