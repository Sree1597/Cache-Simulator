// cacshesimulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//UIN 33000085
#include <iostream>
#include <random>
#include<string.h>
#include<math.h>
#include <sstream>
#include <cmath>
#include <ctime>
#include <stdlib.h>
using namespace std;
using std::cerr;
using std::cout;
using std::cin;
using std::endl;
#include <fstream>
using std::ifstream;
#include <cstdlib>

unsigned long long func_getaddr_access(string , string );
int random(int, int);

int next_power_of_two(int);
int set_number(unsigned long long, int);
int get_tag(unsigned long long);
int set_mask;
int set_offset;
int set_num;
int block_offset;
int tag_offset;
int no_sets;
int index_data;
int assoc;
int hit=0;
int miss = 0;
float percent_hit;
float percent_miss;


int main()
{
	

	string fileloc;
	string line, word;
	string access_type;
	string access;
	string address;
	istringstream iss;
	int i;
	int no_of_acsesses=0;
	string arr[1];
	cout << "Enter the file loc";
	cin >> fileloc;
	unsigned long long int nk;  //capacity of cache in Kbytes
	int size_arr;
	//size_arr = nk * 1000;  //associativity of cache
	unsigned long long arr1[20000];
	int valid[200];  //valid bit array stores if a location is empty or already has data,need when performing write function
	long int blocksize;   //size of single cache block in bytes
	char repl;      //replacement policy LRU or random
	
	//Get the inputs realted to cache on command line
	cout << "nk:";
	cin >> nk;
	cout << "Associativity";
	cin >> assoc;
	cout << "Blocksize:";
	cin >> blocksize;
	cout << "Replacement policy:";
	cin >> repl;
	//cout << "the block numbers";
	long int num_blocks;
	num_blocks = nk * 10 * 10 * 10 / blocksize;        //computes no of blocks in cache(This value gets approximated to next power of 2
	int num_blocksf = next_power_of_two(num_blocks);  //approximates to next power of 2 for the value computed
	no_sets = num_blocksf / assoc;                    //computes no. of sets in cache
	set_mask = no_sets - 1;                           //used to compute set number

	set_offset = log2(no_sets);                       //The number of bits to hold sets


	//cout << num_blocksf << endl;

	block_offset = log2(blocksize);    //no of bits to represent the data in each block
	//cout << block_offset << endl;

    tag_offset =block_offset+set_offset;
	int unsigned long long addr_ret;
	int set_no;
	int tag_no;

	
	
	
	ifstream indata; // indata is like cin
	indata.open(fileloc); // opens the file
	if (!indata) { // file couldn't be opened
		cerr << "Error: file could not be opened" << endl;
		exit(1);
	}
	//indata >> line;
	while (!indata.eof()) { // keep reading until end-of-file
		getline(indata, line);
		//cout << "line got" << endl;
         //if (indata.good())
		//{
			//cout << "line inside if" << endl;
			//cout << line << endl;
			// read word by word from the line
			iss.clear(); // clear out state
			iss.str(line);
			while (iss.good())
			{
				for (i = 0; i < 2;i++)
				{

					iss >> word;
					if (i == 0)
					{
						access_type = word;      //accessing the access type from file
						
						
					}
					else
					{
						address = word;        //accessing the address string from file
					}
					

					//cout << word << endl;

				}
				addr_ret=func_getaddr_access(access_type, address); //returnes the decimal address
				set_num=set_number(addr_ret,set_offset);  //returns the computed set no to check for tags
				cout << "setnum=" << set_num << endl;
				tag_no = get_tag(addr_ret);               //computes the tag value
				cout << "tagnum=" << tag_no << endl;
				index_data = (int)(addr_ret) % ((nk * 10 * 10 * 10) /assoc) * assoc; //computes the index at which data is placed
				cout << "Index of data=" << index_data << endl;
				//access_data(arr[1]);
				//search for tag and read it or write the tag into the set location
				for (i = 0; i < (nk * 1000); i++)
				{
					no_of_acsesses += 1;             //no of accesses to the cache
					int x = (set_num * assoc) - 1;  //search only in the locations of the set
					if (access_type=="r")
					{  //cout << "inside";
					   
					   for (int j = x; j < x+assoc+ 1; j++)
					   {
						   if (tag_no == arr1[j])
						   {
							   hit = hit + 1; //search in the location of the set for the tag ,if fout it's a hit
							   valid[j] = 1;
						   }
						   else
						   {
							   miss = miss + 1;//if the set does not contain tag it's a miss
							   valid[j] = 0;
						   }
					   }

					   

				      }
					else if (access_type == "w")
					{   
						for (int j = x; j < x + assoc + 1; j++)
						{
							if (valid[j]==0)
							{
								arr1[j] = tag_no;  //search for empty location in set and write the tag into that location 
								
							}
							
						}
						
						//int a = random(x, x + assoc);
						//cout << a << endl;
						//arr1[a] = tag_no;

					}

				}
			}
			
			
		//}
		//indata >> line; // sets EOF flag if no value found
	}
	indata.close();
	cout << "End-of-file reached.." << endl;
	percent_hit =( hit / no_of_acsesses)*100;
	percent_miss = (miss / no_of_acsesses)*100;
	cout << "no of hits" << hit << endl;
	cout << "no of misses" << miss << endl;
	cout << "Percentage of hits=" << percent_hit << endl;
	cout << "Percentage of misses=" << percent_miss << endl;

	return 0;
}
//function converts the string address to decimal
unsigned long long func_getaddr_access(string access,string address)
{
	//cout << access << address << endl;
	unsigned long long addr;
	addr = stoull(address, NULL, 16);
	cout << addr<<endl;
	return addr;
}

//function computes the next closet power of 2,needed to compute when no of blocks by default gets approximated to smallest power of 2
int next_power_of_two(int n) {
	int i = 0;
	for (--n; n > 0; n >>= 1) {
		i++;
	}
	return 1 << i;
}
// Extract the set number
// Shift the set to the bottom then extract the set bits
int set_number(unsigned long long addr1, int no_sets1)
{
	
		unsigned long long shifted_address = addr1 >> set_offset;
		return shifted_address & set_mask;
	
}
//Function computes the tag value
int get_tag(unsigned long long addr2)
{
	 return addr2 >> tag_offset;
	 }

//This function computes the random number for the replacement algorithm
int random(int m, int b) {
	return m + rand() % (b - m + 1);
}
