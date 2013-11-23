#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include "rand.h"
#include <vector>

using namespace std;

#define CITIES 15
#define DATAFILE "cities.dat"
#define DEATHRATE .65
#define GROWRATE .70
#define GENERATIONS 45
#define INIT_POP 10000

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
	int BestTour;
	Tour* Crossover(Tour* A, Tour* B);
	Tour* InitTour();
	void Kill(unsigned int choice);
	void PrintProgress(unsigned int ops, unsigned int progress);
	void Sort();
	void Test();

	public:	
	Genetic();
	double TourDistance(Tour* tour);
	double ScoreTour(Tour* tour);
	void Breed();
	void Mutate();
	void PrintPop();
	void PrintProgenator();
	void PrintTour(unsigned int tour_num);
	void Progenate(unsigned int init_pop_size);
	void Reap();
	void RunSimulation();
	void ScorePopulation();
	void TestRand();
	~Genetic();
};
