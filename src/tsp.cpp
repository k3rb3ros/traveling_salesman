#include "include/tsp.h"

struct cmpr
{
        bool operator() (Tour* a, Tour* b) { return (a -> score > b -> score); }
} cmpr_t;

Genetic::Genetic()
{
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
		BestScore = -9999.99999;
		generation = 0;
		Test(); //Continue if we pass our tests;
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
	 city = (tour -> cities[j].city-1);
	 if(complete[city] == false) complete[city] = true; //set the list to true
	}
	for(int k=0; k<CITIES; k++) if(complete[k] == false) return false;
	return true;
}

//unused
double Genetic::Distance(City A, City B)
{
	if(A.city != B.city) return hypot((double)((A.x)-(B.x)), (double)((A.y)-(B.y)));
	return 0.0;
}

double Genetic::TourDistance(Tour* tour)
{
	double distance = 0;
 	for(int i=0; i<CITIES-1; i++)
	{
		//get the sum of distance between all the cities
		distance += hypot((double)((tour->cities[i].x)-(tour->cities[i+1].x)), ((tour->cities[i].y)-(tour->cities[i+1].y)));
	}
	//and then the return to the original city
	distance += hypot((double)((tour->cities[0].x)-(tour->cities[CITIES-1].x)), ((tour->cities[0].y)-(tour->cities[CITIES-1].y)));
	return distance;
}

double Genetic::ScoreTour(Tour* tour)
{
	double distance = TourDistance(tour);
	double score = 0;
	if(tour != NULL)
	{
		if(TourComplete(tour))
		{
			score = 200 - distance;
		}
		else
		{
			//Scale down score based on number of repeats
			score = (400*(1.0/(double)Repeats(tour))) - distance;
		}
		/*if(score > 500)
		{ 
			cout << "Score: " << score << " Repeats: " << Repeats(tour) << "Distance: " << distance << "Tour: " << TourComplete(tour) << endl;
			cout << "[";
			for(int i=0; i<CITIES; ++i) cout << tour -> cities[i].city << " ";
			cout << "]\n";
		}*/
	}
	return score;
}

short Genetic::Repeats(Tour* tour)
{
	short repeat[CITIES];
	short repeats = 0;

	//Init Repeats
	for(short i=0; i<CITIES; ++i)repeat[i]=0;
	//Count the number of repeats
	for(short a=0; a<CITIES; ++a)repeat[tour -> cities[a].city-1]++;
	for(short j=0; j<CITIES; ++j)if(repeat[j] > 1)repeats+=repeat[j];
	
	return repeats;
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
	unsigned int barWidth = 50;
	double percent = ((double)progress/(double)ops)*100;
	unsigned int pos = barWidth * (percent/100);
	cout << "Progress: [";
	for(unsigned short i=0; i<barWidth; ++i)
	{
		if(i<pos) cout << "#";
		else if(i == pos) cout << ">";
		else cout << " ";
	}
	cout << "] " << percent << "\% Gen: " << generation << " Best: " << BestScore << "    \r";
	//cout << "] " << percent << "\% Gen: " << generation << "    \r";
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

	//typedef std::numeric_limits< double > dbl;

	a.city = 1;
	a.x = 3;
	a.y = 5;
	b.city = 2;
	b.x = 9;
	b.y = -3;

	assert(Distance(a, b) == 10.0); //Test hypotenouse for a known distance
	b.y = 3;
	assert(Distance(a, b) == 6.324555320336759); //and one thats not so pretty
	//cout.precision(dbl::digits10+1);
	//cout << "$% " << ScoreTour(&progenator);
	assert(ScoreTour(&progenator) == 80.79048061087767);
}

void Genetic::Breed(unsigned int cur_pop) //breads new members of the population favoring the strong
{
 	unsigned int children = (cur_pop/100)*GROWRATE;
	unsigned int ch1 = 0;
	unsigned int ch2 = 0;
	//unsigned int five_percent = (cur_pop/100)*5;
	//unsigned int ten_percent = (cur_pop/100)*10;
	//unsigned int ninety_five_percent = (cur_pop/100)*95;
	Tour* spawn = NULL;

	for(unsigned int i=0; i<children; i++)
	{
		spawn = NULL;
		{
			ch1 = r_gen.GetVal(r_gen.GetVal(population.size()-1));
			ch2 = r_gen.GetVal(r_gen.GetVal(population.size()-1));
			spawn = Crossover(population[ch1], population[ch2]);
			population.push_back(spawn); //Add the child to the population
		}
	}
	generation++; //increment generation counter
}

void Genetic::Mutate(unsigned int cur_pop) //mutates a random member of the population (attempts to get rid of duplicate cities
{
	int num_mutations = (cur_pop/100)*MUTATERATE;
	for(unsigned short mutate=0; mutate<num_mutations; ++mutate)
	{
		//choose a non optimal member of the population at random
		int choice = 1+r_gen.GetVal(population.size()-2); 
		int repeats[CITIES]; 
		
		//Init our count array
		for(short i=0; i<CITIES; ++i) repeats[i] = 0;
		//count any repeated cities
		for(short k=0; k<CITIES; ++k)
		{
			int index = (population[choice] -> cities[k].city-1);
			repeats[index]++; 
		}
		//cout << "Repeats [";
		//for(short tmp=0; tmp<CITIES; ++tmp) cout << repeats[tmp] << " ";
		//cout << "]\n";
		//replace repeated cities with cities not in the tour
		for(short l=0; l<CITIES; l++)
		{
			//if a city is a repeat then choose one that is not
			if(repeats[(population[choice] -> cities[l].city-1)] > 1)
			{
				short a=0;
				//randomly choose a new a that isn't in the tour
				while(repeats[a] != 0) a = r_gen.RChar(CITIES);
				//decrement the count of the old on in repeats
				repeats[(population[choice] -> cities[l].city-1)] --; 
				//increment the count of the new city
				repeats[a]++;
				//update the tour
				population[choice] -> cities[l] = progenator.cities[a];
			}
		}
		//Set the score of the mutated tour to zero so that it gets rescored
		population[choice] -> score = 0;
	}
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
	cout << "] distance: " << TourDistance(&progenator) << endl;
	
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
	cout << "] distance: " << TourDistance(current);
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

void Genetic::Reap(unsigned int cur_pop) //Kills members of the population (not children) the single strongest always lives 
{
	int dead = 0;
	if(cur_pop > 1)
	{
		if(cur_pop < TARGET_POP) dead = (cur_pop/100) * DEATHRATE;
		else dead = cur_pop - TARGET_POP;
		for(int i=0; i<dead; i++)
		{
			Kill(1+r_gen.GetVal(population.size()-2));
		}
	}
	else cerr << "Population is 1 can't kill anyone else" << endl;
}

void Genetic::RunSimulation()
{
	unsigned int cur_pop = 0;
	Progenate(INIT_POP);
	ScorePopulation();
	cout << "Best Score Generation:" << generation-1 << " " << BestScore << " " << endl;
	PrintTour(1);
	cout << endl;
	for(unsigned int i=0; i<GENERATIONS; i++)
	{
		cur_pop = population.size();
		Reap(cur_pop);
		Breed(cur_pop);
		Mutate(cur_pop);
		ScorePopulation();
		if(i%10==0) PrintProgress(GENERATIONS, i);
	}
	cout << "\n";
	cout << "Best Score Generation:" << generation-1 << " " << BestScore << " " << endl ;
	PrintTour(1);
}

void Genetic::ScorePopulation() //FIXME if I need to score all of population for every generation
{
	double cur_score = 0.0;
	for(unsigned int i=0; i<population.size(); i++)
	{
		if(population[i] -> score == 0.0)
		{
			cur_score = ScoreTour(population[i]);
			population[i] -> score = cur_score;
			if(BestScore < cur_score)
			{
				BestScore = cur_score;
			}
		}
		//else
	}
	this -> Sort(); //Sort the population
}

Tour* Genetic::Crossover(Tour* A, Tour*B) //Take two tours randomly combine them to create a new one
{
	int fate = 0;
	Tour* child = new Tour;
	
	for(int i=0; i<CITIES; i++)
	{
		fate = (int)r_gen.RChar(100);
		if(fate > 50) child -> cities[i] = A -> cities[i];
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
	tour -> score = 0.0;
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
	cout << "\nTesting random uint8_t generation with low bounds" <<  endl;
	cout << "[";
	for(int i=0; i<32; i++)
	{
		cout << (int)r_gen.RChar(CITIES);
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

