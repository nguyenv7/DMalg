#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// bit array
#define SetBit(A,k)     ( A[(k/32)] |= (1 << (k%32)) )
#define ClearBit(A,k)   ( A[(k/32)] &= ~(1 << (k%32)) )
#define TestBit(A,k)    ( A[(k/32)] & (1 << (k%32)) )

#define sizeA 33554432
#define sizeC 1073741824
#define UINT_MAX 65535
int isA[sizeA];     // This is our bit array (32×33554432 = 2^30 bits !)

unsigned long djb2( char* str);
unsigned long sdbm( char* str);

const unsigned long HashTableSize = (unsigned long) pow(2.0, 30.0);

int main() {
	//__uint32_t* countA;
	//countA = (__uint32_t *)malloc(sizeof(__uint32_t)*sizeC);
	// init bit array
	int i;
	for (i = 0; i < sizeA; i++) {
		isA[i] = 0;                    // Clear the bit array
		//countA[i] = 0;
	}

	/*
	 for ( i = 0; i < sizeC; i++ ){
	 countA[i] = 0;
	 }
	 */
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	unsigned long numLine = 0;
	unsigned long F0 = 0;
	//fp = fopen("100kb.txt", "r");
	fp = fopen("/home/linh/twitter_words.txt", "r");

	//fp = fopen("twitter_words.txt", "r");

	if (fp == NULL)
		exit(EXIT_FAILURE);

	unsigned long tmpHash;
	clock_t start = clock(), diff;
	while ((read = getline(&line, &len, fp)) != -1) {
		//printf("Retrieved line of length %zu :\n", read);
		//printf("%s ", line);
		//printf("%lu \n",djb2(line));
		tmpHash = djb2(line);
		// for F0
		if (TestBit(isA,tmpHash) == 0) {
			F0++;
			SetBit(isA, tmpHash);
		}
		// for F2
		/*
		 if(TestBit(isA,tmpHash)==1){
		 countA[tmpHash]++;
		 }
		 */
		numLine++;
		/*if((numLine % 10000) == 0){
		 //if((numLine == 10000)){
		 printf("numLine: %lu\t",numLine);
		 diff = clock() - start;
		 int msec = diff * 1000 / CLOCKS_PER_SEC;
		 printf("%d s %d ms \n", msec/1000, msec%1000);
		 //start = clock();
		 }*/
	}
	diff = clock() - start;
	int msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("%d s %d ms \n", msec / 1000, msec % 1000);
	printf("Line: %lu \n", numLine);
	printf("F0: %lu \n", F0);

	// test the limit of frequency
	/*

	 unsigned long F2 = 0;
	 unsigned int tmpI;
	 for ( i = 0; i < sizeC; i++ ){
	 if(TestBit(isA,i)==1)
	 {
	 tmpI = countA[i];
	 F2 = F2 + tmpI*tmpI;
	 }
	 }

	 free(countA);
	 printf("F2: %lu \n", F2);
	 */

	fclose(fp);
	if (line)
		free(line);
	exit(EXIT_SUCCESS);

	return 1;
}

unsigned long djb2( char* str) {
	unsigned long hash = 5381;
	int c;
	while ((c = *str++) != 0)
		hash = ((hash << 5) + hash) + c;
	return hash % HashTableSize;
}

unsigned long sdbm( char* str){
	//unsigned char *str;
	unsigned long hash = 0;
	int c;

	while ((c = *str++) != 0)
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash % HashTableSize;
}
