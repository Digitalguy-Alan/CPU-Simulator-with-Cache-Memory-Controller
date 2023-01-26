#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>

using namespace std;

#define L1_CACHE_SETS 16
#define L2_CACHE_SETS 16
#define L2_CACHE_WAYS 8
#define MEM_SIZE 4096
#define BLOCK_SIZE 4 // bytes per block
#define DM 0
#define SA 1

struct cacheBlock
{
	int tag; // you need to compute offset and index to find the tag.
	int lru_position; // for SA only
	int data; // the actual data stored in the cache/memory
	bool valid;
	// add more things here if needed
};



class cache {
private:
	cacheBlock L1[L1_CACHE_SETS]; // 1 set per row.
	cacheBlock L2[L2_CACHE_SETS][L2_CACHE_WAYS]; // x ways per row

public:
	cache();
	void controller(bool MemR, bool MemW, int data, int adr, int *myMem);
	int derive_index(int adr);
	int derive_tag(int adr);
/*
    int L1_address0_note;
    int L1_address1_note;
    int L1_address2_note;
    int L1_address3_note;
    int L1_address4_note;
    int L1_address5_note;
    int L1_address6_note;
    int L1_address7_note;
    int L1_address8_note;
    int L1_address9_note;
    int L1_address10_note;
    int L1_address11_note;
    int L1_address12_note;
    int L1_address13_note;
    int L1_address14_note;
    int L1_address15_note;
    */
};

class calculation{
public:
	float find_miss_rateL1();
	float find_miss_rateL2();
	float find_ATT();
};



