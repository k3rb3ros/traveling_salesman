#include "include/tsp.h"

Genetic::Genetic()
{
	Test(); //Continue if we pass our tests;
	ifstream fin(DATAFILE);
        if(fin.good())
        {
                for(int i=0; i<CITIES; i++) //read the data file into the initial tour
                {
                        fin >> progenator.cities[i].city;
                        fin >> progenator.cities[i].x;
                        fin >> progenator.cities[i].y;
                }
               if(!fin.good())
		{
			cerr << "Unable to populate initial tour cannot continue" << endl;	
		}
                fin.close();
        }
	else cerr << "File " << DATAFILE << " not read check that it exists\n";
}

bool Genetic::TourComplete(Tour* tour) //Returns true if a tour contains all the cities
{
	bool complete[CITIES]; 
	for(int i=0; i<CITIES; i++) complete[i] = false; //init the array
	for(int j=0; j<CITIES; j++) complete[tour -> cities[j].city -1] = true; //set the list true 
	for(int k=0; k<CITIES; k++) if(complete[k] == false) return false;
	return true;
}

double Genetic::Distance(City A, City B)
{
	return hypot((double)((A.x)-(B.x)), (double)((A.y)-(B.y)));
}

double Genetic::TourDistance(Tour* tour)
{
	double distance = 0;
 	for(int i=0; i<CITIES-1; i++) distance += hypot((double)((tour->cities[i].x)-(tour->cities[i+1].x)), ((tour->cities[i].y)-(tour->cities[i+1].y))); //get the sum of distance between all the cities
	distance += hypot((double)((tour->cities[0].x)-(tour->cities[CITIES].x)), ((tour->cities[0].y)-(tour->cities[CITIES].y))); //and the return to the original city
	return distance;
}

double Genetic::ScoreTour(Tour* tour)
{
	double distance = TourDistance(tour);
	double score = 0;
	
	if(TourComplete(tour))
	{
		score = 120 - distance;
	}
	else
	{
		score = 150 - distance;
	}
	tour -> score = score;
	return score;
}

void Genetic::Breed() //breads new members of the population favoring the strong
{

}

void Genetic::Mutate() //mutates a random member of the population
{

}

void Genetic::PrintProg()
{
	cout << "Progenator [";
	for(int i=0; i<CITIES; i++)
	{
		cout << progenator.cities[i].city;
		if(i<CITIES-1) cout << ", ";
	}
	cout << "]\n";
}

void Genetic::PrintTour(unsigned int tour_num) //Print the tours in a population
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

void Genetic::Reap() //Kills members of the population the single strongest always lives
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

void Genetic::Test()
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

Genetic::~Genetic()
{
	for(unsigned int i=0; i<population.size(); i++) //free anything allocated on the heap
	{
		delete population[i];
	}	
}	
