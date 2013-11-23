#include "include/rand.h"

Rand::Rand()
{
	rand.open("/dev/urandom", std::ifstream::in); //open handle to /dev/urandom
	if(rand.good())
	{
		is_good = true;
	}
	else is_good = false;	
}

bool Rand::IsGood()
{
	return is_good;
}

uint32_t Rand::GetVal(uint32_t upper_lim) //will never reach upper_lim (only approaches)
{
	uint32_t num = 0;
	uint8_t* fill= (uint8_t*)&num;

	if(rand.good())
	{ //For some strange (unknown but likely intended) reason /dev/urandom can only be read a char at a time
		rand >> fill[0] >> fill[1] >> fill[2] >> fill[3];
		return num%upper_lim+1;
	}
	else return 0;
}

uint8_t Rand::RChar(uint8_t upper_lim) //can reach upper_lim
{
	uint8_t num = 0;
	if(rand.good())
	{
		rand >> num;
		return num%upper_lim+1;
	}
	else return 0;
}

Rand::~Rand()
{
	if(rand.is_open())rand.close(); //close any open file handles
}
