#include <assert.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include "rand.h"
#include <vector>

using namespace std;

#define CITIES 15
#define DATAFILE "cities.dat"

struct City
{
	int city;
	int x;
	int y;
};

struct Tour
{
	City cities[CITIES];
	double score;
};

class Genetic
{
	private:
	int generation;
	Rand r_gen;
	Tour progenator;
	vector<Tour*> population;

	bool TourComplete(Tour* tour);
	double BestScore;
	double Distance(City A, City B);
	Tour* InitTour();
	void Test();

	public:	
	Genetic();
	double TourDistance(Tour* tour);
	double ScoreTour(Tour* tour);
	void Breed();
	void Mutate();
	void PrintProg();
	void PrintTour(unsigned int tour_num);
	void Progenate(unsigned int init_pop_size);
	void Reap();
	void ScorePopulation();
	void TestRand();
	~Genetic();
};
