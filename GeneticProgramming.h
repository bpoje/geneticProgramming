#pragma once

#include <stdio.h>
#include <time.h>
#include "Tree.h"
#include "Global.h"

class GeneticProgramming
{
public:
	GeneticProgramming(int newPopulationSize, int iMaxDepthOfInitialTree, int iMaxTotalTreeDepth);
	~GeneticProgramming(void);
	void evolve(int forMaxGenerations, int afterGoalReachedGenerations, int stepsInFitnessEval, int sampleSize, int iMaxDepthOfTreeAppendedAtMutation);
	int positiveTournament(double *fitness, int sampleSize);
	int negativeTournament(double *fitness, int sampleSize);
private:
	void startLog(FILE *events, int &forMaxGenerations, int &afterGoalReachedGenerations, int &stepsInFitnessEval, int &sampleSize);
	void endLog(FILE *events, int &generation, int &stepsInFitnessEval, double &elapsedTime);
	void loopLog(FILE *events, double &bestFitnessInThisGeneration, int &generation, unsigned int &bestFitForGenerations, double &fitIncrease);

	inline Tree makeCrossover(int &sampleSize);	//Make crossover tree (tree duplicate)
	inline Tree makeCopy(int &sampleSize);			//Make copy of a good ranking tree (tree duplicate)
	inline void makeMutation(Tree treeToMutate);	//Perform direct (no duplicate) mutation on a tree
	inline void makeMutationTokenChange(Tree treeToMutate);

	void programsToFiles();		//Write all programs from population to individual files named program%d.txt
	void programsToFilesAtImprovement(int generation);

	void paretoOptimality();

	//Arrays
	Tree *population;
	Tree *newPopulation;
	double *fitness;

	//Size of population
	int populationSize;

	//Global constrains
	int maxTotalTreeDepth;

	//Initialization tree generation
	int maxDepthOfInitialTree;

	//Mutations
	int maxDepthOfTreeAppendedAtMutation;

	//Counters for statistics
	unsigned int crossovers;
	unsigned int copies;
	unsigned int mutations;
};
