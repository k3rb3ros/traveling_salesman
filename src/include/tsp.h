#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <float.h>
#include <fstream>
#include "rand.h"
#include <vector>

using namespace std;

#define CITIES 15
#define DATAFILE "cities.dat"
#define DEATHRATE 70
#define GROWRATE 75
#define GENERATIONS 750
#define INIT_POP 100
#define MUTATERATE 20
#define TARGET_POP 1000

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
	short Repeats(Tour* tour);
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
	void Breed(unsigned int cur_pop);
	void Mutate(unsigned int cur_pop);
	void PrintPop();
	void PrintProgenator();
	void PrintTour(unsigned int tour_num);
	void Progenate(unsigned int init_pop_size);
	void Reap(unsigned int cur_pop);
	void RunSimulation();
	void ScorePopulation();
	void TestRand();
	~Genetic();
};
