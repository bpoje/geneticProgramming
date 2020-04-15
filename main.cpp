#include <time.h>
#include "Random.h"
#include "GeneticProgramming.h"

int main(int argc, char **argv)
{
	clock_t start, finish;
	start = clock();

	/*
	//Init. generatorja nakljucnih stevil
	time_t t = time(NULL);
	srand((unsigned int)t);
	myRandom();
	*/

	//Genetic programming
	//GeneticProgramming(int newPopulationSize, int max_d, int lMaxTotalTreeDepth)
	int iMaxTotalTreeDepth = 9;
	int iMaxDepthOfInitialTree = iMaxTotalTreeDepth / 2;
	int newPopulationSize = 500;

	GeneticProgramming gp(newPopulationSize, iMaxDepthOfInitialTree, iMaxTotalTreeDepth);
	//evolve(int forGenerations, int stepsInFitnessEval, int sampleSize)

	//int forMaxGenerations = 1000000;
	int forMaxGenerations = 1000;
	int afterGoalReachedGenerations = 1000;
	int stepsInFitnessEval = 1;
	int sampleSize = 2;
	int iMaxDepthOfTreeAppendedAtMutation = iMaxTotalTreeDepth / 2;
	gp.evolve(forMaxGenerations,afterGoalReachedGenerations,stepsInFitnessEval,sampleSize,iMaxDepthOfTreeAppendedAtMutation);
	//gp.evolve(100,10,1,20,20);

	finish = clock();

	double elapsed = ( (finish - start)/(double)CLOCKS_PER_SEC );
	printf("1elapsed: %f\n", elapsed);
	//getc(stdin);

	return 0;
}
