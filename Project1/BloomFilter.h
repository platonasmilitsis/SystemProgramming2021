#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H
#include <stdio.h>

typedef struct BloomFilter{
    int Bloomsize;
    unsigned char *Filter;
    int K;
}BloomFilter;

BloomFilter *BloomFilter_Create (int Bloomsize, int K);
BloomFilter *BloomFilter_Insert (BloomFilter *bf,char* str);
int BloomFilter_Search(BloomFilter *bf,char* str);
void BloomFilter_Delete(BloomFilter *bf);

#endif //BLOOMFILTER_H
