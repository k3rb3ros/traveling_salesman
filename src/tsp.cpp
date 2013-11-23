#include "include/tsp.h"

struct cmpr
{
	bool operator() (Tour* a, Tour* b) { return (a -> score > b -> score); }
} cmpr_t;

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
		//Set our private members
		BestScore = 0.0;
		BestTour = 0;
		generation = 0;
        }
	else cerr << "File " << DATAFILE << " not read check that it exists\n";
}

bool Genetic::TourComplete(Tour* tour) //Returns true if a tour contains all the cities
{
	bool complete[CITIES];
	int city = 0; 
	for(int i=0; i<CITIES; i++) complete[i] = false; //init the array
	for(int j=0; j<CITIES; j++)
	{
	 city = (tour -> cities[j].city) -1;
	 if(complete[city] == false) complete[city] = true; //set the list to true
	}
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

void Genetic::Kill(unsigned int choice)
{
	if(choice < population.size())
	{
		delete population[choice]; //free the memory
		population.erase(population.begin()+choice); //remove it from the vector
	}
}

void Genetic::PrintProgress(unsigned int ops, unsigned int progress)
{
	unsigned int barWidth = 70;
	double percent = ((double)progress/(double)ops)*100;
	unsigned int pos = barWidth * (percent/100);
	cout << "Progress: [";
	for(unsigned short int i=0; i<barWidth; ++i)
	{
		if(i<pos) cout << "#";
		else if (i == pos) cout << ">";
		else cout << " ";
	}
	cout << "] " << percent << "%      \r";
	cout.flush();
}

void Genetic::Sort()
{
	sort(population.begin(), population.end(), cmpr_t);
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

void Genetic::Breed() //breads new members of the population favoring the strong
{
	int _pop_size = population.size();
	int breed_rate = (_pop_size/100)*GROWRATE;
	int ch1 = 0;
	int ch2 = 0;
	int five_percent = (_pop_size/100)*5;
	int ten_percent = (_pop_size/100)*10;
	int ninety_five_percent = (_pop_size/100)*95;
	Tour* spawn = NULL;

	for(int i=0; i<breed_rate; i++)
	{
		if(i <= _pop_size/10) //top 10% of the population breads with itself
		{
			ch1 = r_gen.GetVal(ten_percent-1);
			ch2 = r_gen.GetVal(ten_percent-1);
			//do {ch2 = r_gen.GetVal(ten_percent-1);} while(ch1 == ch2); //prevent self breeding
			spawn = Crossover(population[ch1], population[ch2]);
			population.push_back(spawn); //Add the child to the population
		}
		else //the bottom 95% breeds with everybody in it (includes 5% mixing with the the top 10%)
		{
			ch1 = five_percent + r_gen.GetVal(ninety_five_percent-1);
			ch2 = five_percent + r_gen.GetVal(ninety_five_percent-1);
			//do {ch2 = five_percent + r_gen.GetVal(ninety_five_percent-1);} while(ch1 == ch2); //prevent self breeding
			spawn = Crossover(population[ch1], population[ch2]);
			population.push_back(spawn);
		}
	}
	generation++; //increment generation counter
}

void Genetic::Mutate() //mutates a random member of the population
{

}

void Genetic::PrintPop()
{
	cout << "Population scores {";
	for(unsigned int i=0; i<population.size(); i++)
	{
		cout << population[i] -> score;
		if(i < population.size()-1) cout << ", ";
	}
	cout << "}\n";
}

void Genetic::PrintProgenator()
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

void Genetic::Progenate(unsigned int init_pop_size)
{
	Tour* spawn = NULL;
	uint8_t rand = 0;
	generation = 1;
	for(unsigned int i=0; i<init_pop_size; i++)
	{
		spawn = InitTour(); //Spawn a new tour
		for(int k=0; k<CITIES; k++) //populate each tour with a random ordering of cities from progenator
		{
			rand = r_gen.RChar(CITIES-1);
			spawn -> cities[k].city = progenator.cities[rand].city;
			spawn -> cities[k].x = progenator.cities[rand].x;
			spawn -> cities[k].y = progenator.cities[rand].y;
		}
		population.push_back(spawn); //add it to the population
	}
}

void Genetic::Reap() //Kills members of the population the single strongest always lives
{
	int dead = ((population.size()/100) * DEATHRATE); //calculate the number of population to kill off
	for(int i=0; i<dead; i++)
	{ 
		Kill(1 + r_gen.GetVal(population.size()));
	}
}

void Genetic::RunSimulation()
{
	cout << "Running Simulation\n";
	Progenate(INIT_POP);
	ScorePopulation();
	cout << "Initial Best: " << BestScore << endl;
	for(unsigned int i=0; i<GENERATIONS; i++)
	{
		Reap();
		Breed();
		ScorePopulation();
		PrintProgress(GENERATIONS, i);
	}
	cout << "\n";
	cout << "Best Score: " << BestScore << " ";
	PrintTour(BestTour);
}

void Genetic::ScorePopulation() //FIXME if I need to score all of population for every generation
{
	double cur_score = 0.0;
	//cout << "Scoring generation: " << generation << endl;
	for(unsigned int i=0; i<population.size(); i++)
	{
		if(population[i] -> score == 0.0)
		{
			cur_score = ScoreTour(population[i]);
			population[i] -> score = cur_score;
			if(BestScore < cur_score)
			{
				BestScore = cur_score;
				BestTour = i;
			}
		}
		//else
	}
	this -> Sort(); //Sort the population
	//cout << "Best score: " << BestScore << endl;
}

Tour* Genetic::Crossover(Tour* A, Tour*B) //Take two tours randomly combine them to create a new one
{
	double fate = 0;
	Tour* child = new Tour;
	for(int i=0; i<CITIES; i++)
	{
		fate = r_gen.GetVal(100);
		if(fate > 50.0) child -> cities[i] = A -> cities[i];
		else child -> cities[i] = B -> cities[i];
	}
	return child;
}

Tour* Genetic::InitTour() //Create a new tour and set all it values as zero
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

void Genetic::TestRand()
{
	cout << "Testing random uint32_t generation" << endl;
	cout << "[";
	for(int i=0; i<32; i++)
	{
		cout << r_gen.GetVal(6060694);
		if(i<31) cout << ", ";
	} 
	cout << "]";
	cout << "\nTesting random uint8_t generation" <<  endl;
	cout << "[";
	for(int i=0; i<32; i++)
	{
		cout << (int)r_gen.RChar(255);
		if(i<31) cout << ", ";
	}
	cout << "]";
}

Genetic::~Genetic()
{
	for(unsigned int i=0; i<population.size(); i++) //free anything allocated on the heap
	{
		delete population[i];
	}	
}	
