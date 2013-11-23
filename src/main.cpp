#include <iostream>
#include "include/tsp.h"

int main(void)
{
	Genetic solve_tsp;

	solve_tsp.PrintProgenator();
	cout << "Spawning Generation 1\n";
	//solve_tsp.TestRand();
	solve_tsp.RunSimulation();
	

	return 0;
}
