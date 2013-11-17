#include <iostream>
#include <vector>
using namespace std;

#define NUM_CITIES 15

struct City
{
	int city;
	int x;
	int y;
};

struct Tour
{
	City cities[NUM_CITIES];
};

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
	for(int j=0; j<NUM_CITIES; j++)
	{
		cout << current -> cities[j].city;
		if(j < NUM_CITIES-1) cout << ", ";
	}
	cout << "]" << endl;
}

Tour* InitTour() //Create a new tour and set all it values as zero
{
	Tour* tour = new Tour;
	for(int i=0; i<NUM_CITIES; i++)
	{
		tour -> cities[i].city = 0;
		tour -> cities[i].x = 0;
		tour -> cities[i].y = 0;
	}
	return tour;
}

int main(void)
{
	vector<Tour*> population;
	
	population.push_back(InitTour()); //add a test member to population
	
	PrintTour(population, 1);
	
	CleanUp(population);
	cout << "The Optimal solution is 42" << endl;
}
