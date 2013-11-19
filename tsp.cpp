#include <assert.h>
#include <cmath>
#include <iostream>
#include <fstream>
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
	int score;
};

bool PopInitialTour(Tour &tour)//create the initial tour in order 
{
	ifstream fin(DATAFILE); 
	if(fin.good())
	{
		for(int i=0; i<CITIES; i++) //read the data file into the initial tour
		{
			fin >> tour.cities[i].city;
			fin >> tour.cities[i].x;
			fin >> tour.cities[i].y;
		}
		fin.close();
		if(fin.eof())return true;
	}
	return false;
}

double Distance(City A, City B)
{
	return hypot((double)((A.x)-(B.x)), (double)((A.y)-(B.y)));
}

double TourDistance(Tour* tour)
{
	double distance = 0;
 	for(int i=0; i<CITIES-1; i++) distance += Distance(tour -> cities[i], tour -> cities[i+1]); //get the sum of distance between all the cities
	distance += Distance(tour -> cities[0], tour -> cities[CITIES-1]); //and the return to the original city
	return distance;
}

int ScoreTour(Tour* tour)
{
	int score = 0;
	return score;
}

void Bread(vector<Tour*> population) //breads new members of the population favoring the strong
{

}

void Mutate(vector<Tour*> population) //mutates a random member of the population
{

}

void PrintTour(vector<Tour*> &population, unsigned int tour_num) //Print the tours in a population
{
	Tour* current = NULL;
	if(population.size() < tour_num-1) 
	{
		cerr << "Unable to print tour " << tour_num << " it isn't in population" << endl;
		return;
	}

	current = population[tour_num-1];

	cout << "Tour: " << tour_num << " [";
	for(int j=0; j<CITIES; j++)
	{
		cout << current -> cities[j].city;
		if(j < CITIES-1) cout << ", ";
	}
	cout << "] distance: " << TourDistance(population[tour_num-1]);
}

void Reaper(Vector<tour*> population) //Kills members of the population the single strongest always lives
{

}

Tour* InitTour() //Create a new tour and set all it values as zero
{
	Tour* tour = new Tour;
	for(int i=0; i<CITIES; i++)
	{
		tour -> cities[i].city = 0;
		tour -> cities[i].x = 0;
		tour -> cities[i].y = 0;
	}
	tour -> score = 0;
	return tour;
}

void CleanUp(vector<Tour*> &population) //Free everything dynamically allocated
{
	int len = population.size();

	//Free the memory pointed to by every element in the vector then delete that element from the vector
	for(int i=0; i<len; i++)
	{
	 delete population[i];
	 population.erase(population.begin()+i);
	}
}

void Test()
{
	City a;
	City b;

	a.city = 1;
	a.x = 3;
	a.y = 5;
	b.city = 2;
	b.x = 9;
	b.y = -3;

	assert(Distance(a, b) == 10.0); //Test hypotenouse for a known distance
	b.y = 3;
	assert(Distance(a, b) == 6.324555320336759); //and one thats not so pretty
}

int main(void)
{
	int generation = 0;
	Tour progenator;
	vector<Tour*> population;
	
	Test(); //if test passes continue	

	//population.push_back(InitTour()); //add a test member to population
	PopInitialTour(progenator);
	population.push_back(&progenator);	
	PrintTour(population, 1);
	
	//CleanUp(population);
}
