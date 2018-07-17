// mnist.c
// jason corso
//
// Written for CSE 555 Spring 2009.  (You don't really need this file, it's 
//  provided for your information in the case that you want to get and use 
//  more of the MNIST data.)
//  
//
// C code to convert the MNIST images of a particular set and digit into
//  a directory full of pgm images (these are grayscale).
// The PGM format is as follows
// Line1 P5
// Line2 WIDTH HEIGHT
// Line3 255
// Line4 ROW-WISE-BYTE-CHUNK-OF-PIXELS
//
//
// NOTE:  Need to byteswap to little-endian
// Written on a Mac OS X.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>  
#include <sys/stat.h>

#define BUFLEN 1024

int main (int argc, char** argv)
{
	int digit=-1;	
	int maxout=-1;    // this is the maximum number of a particular class to output
	char* imagefn=0x0;
	char* labelfn=0x0;
	FILE* fi;
	FILE* fl;
	FILE* fpgm;
	int ia,ib;
	int nimages,nrows,ncols;
	int nwritten=0;
	int nseen=0;
	unsigned char* pixels;
	unsigned char label;
	char buf[BUFLEN+1];

	if (argc != 5)
	{
		fprintf(stderr,"usage: mnist DIGIT IMAGEFN LABELFN NUMBER {both files must be uncompressed}\n");
		exit(-1);
	}

	digit = atoi(argv[1]);
	maxout = atoi(argv[4]);
	imagefn = argv[2];
	labelfn = argv[3];

	snprintf(buf,BUFLEN,"%d",digit);
	mkdir(buf,S_IROTH | S_IXOTH | S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP);

	fi = fopen(imagefn,"rb");

	fread(&ia,sizeof(int),1,fi);
	ia = ntohl(ia);
	assert(ia == 2051);

	fread(&nimages,sizeof(int),1,fi);
	nimages = ntohl(nimages);
	fread(&nrows,sizeof(int),1,fi);
	nrows = ntohl(nrows);
	fread(&ncols,sizeof(int),1,fi);
	ncols = ntohl(ncols);
	pixels = (unsigned char*)malloc(nrows*ncols);
	
	printf("working with %d images of %d x %d\n",nimages,nrows,ncols);


	fl = fopen(labelfn,"rb");

	fread(&ia,sizeof(int),1,fl);
	ia = ntohl(ia);
	assert(ia == 2049);

	fread(&ib,sizeof(int),1,fl);
	ib = ntohl(ib);
	assert(ib == nimages);

	while ( (nwritten < maxout) && (nseen < nimages))
	{
		fread(&label,1,1,fl);	
		fread(pixels,1,nrows*ncols,fi);

		if (label == digit)
		{
			snprintf(buf,BUFLEN,"%d/%05d.pgm",digit,nwritten);
			nwritten++;
		
			fpgm = fopen(buf,"wb");
			fprintf(fpgm,"P5\n%d %d\n255\n",ncols,nrows);
			fwrite(pixels,1,nrows*ncols,fpgm);
			fclose(fpgm);
		}

		nseen++;
	}

	printf("found and wrote %d %d digits\n",nwritten,digit);

	fclose(fi);
	fclose(fl);

	return 0;
}
