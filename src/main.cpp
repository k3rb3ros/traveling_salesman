#include <iostream>
#include "include/tsp.h"

int main(void)
{
	Genetic solve_tsp;

	solve_tsp.PrintProg();
	cout << "Spawning Generation 1\n";
	solve_tsp.Progenate(10000);
	cout << "Done\n";
	solve_tsp.ScorePopulation();
	//solve_tsp.TestRand();
	

	return 0;
}
