#include "cache.h"


int missL1;
int missL2;
int accL1;
int accL2;


cache::cache()
{
	for (int i=0; i<L1_CACHE_SETS; i++)
		L1[i].valid = false;
	for (int i=0; i<L2_CACHE_SETS; i++)
		for (int j=0; j<L2_CACHE_WAYS; j++)
			L2[i][j].valid = false;

	missL1 = 0;
	missL2 = 0;
	accL1 = 0;
	accL2 = 0;
}



int cache::derive_index(int adr){
	int x=adr%16;
	return x;
}




int cache::derive_tag(int adr){
    int x=adr/16;
    return x;
}





float calculation::find_miss_rateL1()
{
	float temporary = (float) missL1 / accL1;
	return temporary;
}



float calculation::find_miss_rateL2()
{
	float temporary = (float) missL2 / accL2;
	return temporary;
}



float calculation::find_ATT()
{
	return calculation::find_miss_rateL1() *(calculation::find_miss_rateL2() *100 + 8) + 1;
}



void cache::controller(bool MemR, bool MemW, int data, int adr, int *myMem)
{
	unsigned int index;
	unsigned int tag;
	bool data_find = false;

	tag = cache::derive_tag(adr);
	index = cache::derive_index(adr);
//////////////////////////////////////////////////////write to memory/////////////////////////////////////////////////////////////////////////////////
	if (MemW == 1)
	{
		accL1++;
		if (cache::L1[index].valid==true && cache::L1[index].tag == tag)
		{
			cache::L1[index].data = data;
		}
		else
		{
            missL1++;
			accL2++;


			int i = 0;
			while (i < 8)
			{
				if (cache::L2[index][i].valid== true && cache::L2[index][i].tag == tag)
				{
					cache::L2[index][i].data = data;
					cache::L2[index][i].valid = true;
					cache::L2[index][i].tag = cache::L1[index].tag;
					cache::L2[index][i].data = cache::L1[index].data;
//update l1
					cache::L1[index].valid = true;
					cache::L1[index].tag = tag;
					cache::L1[index].data = data;
					// cout <<"myL1data"<<"["<<index<<"]"<<"= "<< cache::L1[index].data<< endl;

//update lru;----------------------
                    int j=0;
					while(j<8)
					{
						if (cache::L2[index][j].valid== true && cache::L2[index][j].lru_position > cache::L2[index][i].lru_position )
						{
							cache::L2[index][j].lru_position--;
						}
						j++;
					}
					data_find = true;
					L2[index][i].lru_position = 7;//update lru;------------------------------------------

					break;
				}
				i++;
			}

			if (data_find == false)
			{
				missL2++;
			}
		}

		myMem[adr] = data;
		   // cout <<"myMem"<<"["<<adr<<"]"<<"= "<< data<< endl;
	}




















/////////////////////////////////////////////read from memory or cache///////////////////////////////
	if (MemR == 1)
	{
		bool data_find = false;
		accL1++;
		if (cache::L1[index].valid==true && cache::L1[index].tag == tag)
		{
			data_find = true;
		}
		else
		{
			missL1++;
			accL2++;
			int i = 0;
			while (i < 8)
			{
				if (cache::L2[index][i].valid==true && cache::L2[index][i].tag == tag)
				{
                    cache::L2[index][i].valid = true;
					cache::L2[index][i].tag = cache::L1[index].tag;
					cache::L2[index][i].data = cache::L1[index].data;
					// cout <<"myL1data"<<"["<<index<<"]"<<"= "<< cache::L1[index].data<< endl;
//update l1
					cache::L1[index].valid = true;
					cache::L1[index].tag = tag;
					cache::L1[index].data = data;

//----------------------------------------------------------------------update
                    int j=0;
					while(j < 8)
					{
						if (cache::L2[index][j].valid==true && cache::L2[index][j].lru_position > cache::L2[index][i].lru_position )
						{
							cache::L2[index][j].lru_position--;
						}
						j++;
					}
					cache::L2[index][i].lru_position = 7;
					data_find= true;
					break;
				}

				i++;
			}

			if (data_find == false){missL2++;}
		}

//L1 &L2 not find;
		if (data_find == false)
		{
		    bool judge_valid_L1 = cache::L1[index].valid;
		    int judge_tag_L1 = cache::L1[index].tag;
			int judge_data_L1 = cache::L1[index].data;


			cache::L1[index].valid = true;
            cache::L1[index].tag = tag;
			cache::L1[index].data = data;




// evict data from L1 to L2
			if (judge_valid_L1 == true)
			{
				int i = 0;
				while (i < 8)
				{
					if (cache::L2[index][i].valid == 0)
					{
					    cache::L2[index][i].valid = true;
					    cache::L2[index][i].tag = judge_tag_L1;
						cache::L2[index][i].data = judge_data_L1;
					//	cout <<"myL2data"<<"["<<index<<"]"<<"= "<< cache::L2[index][i].data<< endl;

                        int j=0;
                        while(j < 8)

						{
							if (cache::L2[index][j].valid==true)
							{
							    if(cache::L2[index][j].lru_position > cache::L2[index][i].lru_position){
								cache::L2[index][j].lru_position--;
							    }
							}
							j++;
						}
						cache::L2[index][i].lru_position = 7;
						break;
					}
					i++;
				}

				if (i >= 8)
				{
					int j = 0;
					while (j < 8)
					{
						if (cache::L2[index][j].lru_position == 0)
						{
						    cache::L2[index][j].valid = true;
						    cache::L2[index][j].tag = judge_tag_L1;
							cache::L2[index][j].data = judge_data_L1;
 //cout <<"myL2data"<<"["<<index<<"]"<<"= "<< cache::L2[index][j].data<< endl;

                            int q;
							while( q< 8)
							{
								if (j != q){
									cache::L2[index][q].lru_position--;
								}
								q++;
							}
							break;
						}

						j++;
					}
				}
			}
		}
	}
}
