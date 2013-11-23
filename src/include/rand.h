#include <fstream>
#include <stdint.h>
#include <random>

class Rand
{
	private:
	bool is_good;
	std::ifstream rand;
	
	public:
	Rand(); //Ctor
	bool IsGood();
	uint8_t RChar(uint8_t upper_lim); //get a random number between zero and upper lim
	uint32_t GetVal (uint32_t upper_lim);
	~Rand(); //Dtor
};
