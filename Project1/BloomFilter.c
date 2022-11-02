#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BloomFilter.h"
#include "Record.h"

/*
This algorithm (k=33) was first reported by dan bernstein many years 
ago in comp.lang.c. 
The magic of number 33 (why it works better than many other constants, 
prime or not) has never been adequately explained.
*/
unsigned long djb2(unsigned char *str) {
	unsigned long hash = 5381;
	int c; 
	while (c = *str++) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash;
}


/*
This algorithm was created for sdbm (a public-domain reimplementation of ndbm) 
database library. it was found to do well in scrambling bits, causing better 
distribution of the keys and fewer splits. it also happens to be a good 
general hashing function with good distribution. The actual function 
is hash(i) = hash(i - 1) * 65599 + str[i]; what is included below 
is the faster version used in gawk. There is even a faster, duff-device 
version. The magic constant 65599 was picked out of thin air while experimenting 
with different constants, and turns out to be a prime. this is one of the 
algorithms used in berkeley db (see sleepycat) and elsewhere.
*/
unsigned long sdbm(unsigned char *str) {
	unsigned long hash = 0;
	int c;

	while (c = *str++) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}


/* 
Return the result of the Kth hash funcation. This function uses djb2 and sdbm.
None of the functions used here is strong for cryptography purposes but they
are good enough for the purpose of the class.

The approach in this function is based on the paper:
https://www.eecs.harvard.edu/~michaelm/postscripts/rsa2008.pdf
*/
unsigned long hash_i(unsigned char *str, unsigned int i) {
	return djb2(str) + i*sdbm(str) + i*i;
}


////Function that initializes a bloom filter/////////
BloomFilter *BloomFilter_Create (int Bloomsize,int K){
    //Allocate space for a bloom filter
    BloomFilter *bf = malloc(sizeof(BloomFilter));  
    bf->Bloomsize = Bloomsize;  
    bf->K=K;
    //Filling the bloom filter with 0                       
    bf->Filter = malloc(sizeof(char) * Bloomsize);
    memset(bf->Filter,0, Bloomsize);

    return bf;
}

/////Insert into Bloom filter///////////
BloomFilter *BloomFilter_Insert (BloomFilter *bf,char* str){
    //for every k
    for (int i=0 ; i < bf->K ; i++ ){
        // finding the position in bloom filter
        unsigned long hash=hash_i(str,i);
        hash = hash % (bf->Bloomsize * 8);
        //finding the byte
        unsigned long index=hash/8;
        //finding the position in byte
        unsigned long position=hash%8;
        //applying mask to current byte and changing the bit we want to 1
        unsigned char mask = 1;
        mask = mask << position;
        //change bit to 1
        bf->Filter[index] = mask | bf->Filter[index];
    }

    return bf;

}

int BloomFilter_Search(BloomFilter *bf,char* str){

     //for every k
    for (int i=0 ; i < bf->K ; i++ ){
        // finding the position in bloom filter
        unsigned long hash=hash_i(str,i);
        hash = hash % (bf->Bloomsize * 8);
        //finding the byte
        unsigned long index=hash/8;
        //finding the position in byte
        unsigned long position=hash%8;
        //applying mask to current byte and changing the bit we want to 1
        unsigned char mask = 1;
        mask = mask << position;
        //FInding the 1 bit 
        unsigned char item = bf->Filter[index] & mask;
        item = item >> position;

        if (item == 0)
        return 0;
    }
    return 1;
}



 void BloomFilter_Delete(BloomFilter *bf){
     free (bf->Filter);
     free(bf);
 }

