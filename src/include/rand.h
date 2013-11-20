#include <fstream>
#include <stdint.h>

class Rand
{
	private:
	bool is_good;
	std::ifstream rand;
	uint8_t rand_num;
	
	public:
	Rand(); //Ctor
	bool IsGood();
	uint8_t GetVal(uint8_t upper_lim); //get a random number between zero and upper lim
	~Rand(); //Dtor
};
