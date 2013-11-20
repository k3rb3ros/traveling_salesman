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

uint8_t Rand::GetVal(uint8_t upper_lim)
{
	uint8_t num = 0;
	rand >> num;
	return num%upper_lim;
}

Rand::~Rand()
{
	if(rand.is_open())rand.close(); //close any open file handles
}
