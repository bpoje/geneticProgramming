#include "GeneticProgramming.h"

//extern unsigned int same;

GeneticProgramming::GeneticProgramming(int newPopulationSize, int iMaxDepthOfInitialTree, int iMaxTotalTreeDepth)
{
	if (newPopulationSize <= 0)
	{
		newPopulationSize = 1;
		printf("Wrong population size!\n");
	}

	populationSize = newPopulationSize;
	population = new Tree[populationSize];
	newPopulation = new Tree[populationSize];

	maxDepthOfInitialTree = iMaxDepthOfInitialTree;

	fitness = new double[populationSize];

	for (int i=0; i < populationSize; i++)
	{
		//population[i] = new Tree();
		bool methodIsGrow = randomBool();
		population[i].gen_rnd(maxDepthOfInitialTree, methodIsGrow);

		//newPopulation[i] = NULL;
		population[i].createProblem();
	}

	maxTotalTreeDepth = iMaxTotalTreeDepth;

	/*
	userFinishReq = false;

	int rc;
	long t;
	rc = pthread_create(&control, NULL, Control, (void *)t);
	if (rc)
	{
		printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	}
	*/
}

GeneticProgramming::~GeneticProgramming(void)
{
	for (int i=0; i < populationSize; i++)
	{
		//delete population[i];
		population[i].destroyProblem();
		population[i].deleteTree();
	}
	delete [] population;

	delete [] fitness;

	delete [] newPopulation;
}

void GeneticProgramming::evolve(int forMaxGenerations, int afterGoalReachedGenerations, int stepsInFitnessEval, int sampleSize, int iMaxDepthOfTreeAppendedAtMutation)
{
	//Time measurement
	clock_t startTime, finishTime;
	startTime = clock();

	//Counters for statistics
	crossovers = 0, copies = 0, mutations = 0;

	//Init mutations
	maxDepthOfTreeAppendedAtMutation = iMaxDepthOfTreeAppendedAtMutation;

	//KEEP TRACK OF BEST FITNESS IMPROVEMENTS
	double bestFitnessFoundSoFar = 0;	//Best fitness found so far
	unsigned int bestFitForGenerations = 0;	//For how many generations was this fitness the best
											//(for how many generations no better fitness was found)
	double fitIncrease = 0;	//The increase in fitness (when a better fitness is found)

	//Was goal reached
	bool goalReached = false;
	unsigned int goalReachedInGeneration = 0;

	//TEXT OUTPUT
	#ifdef WRITE_TO_STDOUT_IN_LOOP
		startLog(stdout, forMaxGenerations, afterGoalReachedGenerations, stepsInFitnessEval, sampleSize);
	#endif
	#ifdef WRITE_EVENTS_LOG_FILE
		FILE *events = fopen("events.txt", "w");
		startLog(events, forMaxGenerations, afterGoalReachedGenerations, stepsInFitnessEval, sampleSize);
	#endif

	//INITIAL EVALUATION
	//Evaluate fitness of initial trees
	for (int i=0; i < populationSize; i++)
		fitness[i] = population[i].executeTreeAndGetFitness(stepsInFitnessEval);

	//-----------------------------
	paretoOptimality();
	//-----------------------------

	//Find best fitness from initial trees
	//double bestFitnessInThisGeneration = fitness[0];
	double bestFitnessInThisGeneration = population[0].getProblem()->oneObjectiveFitness;
	for (int i=0; i < populationSize; i++)
		//if (fitness[i] < bestFitnessInThisGeneration)
		if (population[i].getProblem()->oneObjectiveFitness < bestFitnessInThisGeneration)
			//bestFitnessInThisGeneration = fitness[i];
			bestFitnessInThisGeneration = population[i].getProblem()->oneObjectiveFitness;

	int generation;
	for (generation=0; generation < forMaxGenerations; generation++)
	{
		//Generation 0 represents first evolved generation (that evolved from initial random population)
		#ifdef WRITE_TO_STDOUT_IN_LOOP
			printf("Generation: %d\n", generation);
		#endif

		//GENETIC OPERATIONS
		for (int i=0; i < populationSize; i++)
		{
			//int offspring = negativeTournament(fitness, sampleSize);

			//Tree *newTree = NULL;
			Tree newTree;
			double rnd = myRandom();
			if (rnd < 0.90)
			{
				newTree = makeCrossover(sampleSize);
			}
			else
			{
				newTree = makeCopy(sampleSize);
			}

			rnd = myRandom();
			if (rnd < 0.20)
			{
				rnd = myRandom();
				if (rnd < 0.5)
					makeMutation(newTree);
				else
					makeMutationTokenChange(newTree);
			}

			newTree.setProblem(population[i].getProblem());
			newPopulation[i] = newTree;
		}

		//Delete old population from array population
		//and fill it with newly evolved generation
		for (int i=0; i < populationSize; i++)
		{
			//delete population[i];
			population[i].deleteTree();
			population[i] = newPopulation[i];
		}

		//Evaluate fitness
		for (int i=0; i < populationSize; i++)
			fitness[i] = population[i].executeTreeAndGetFitness(stepsInFitnessEval);

		//-----------------------------
		paretoOptimality();
		//-----------------------------

		//Find best fitness in this generation
		//double bestFitnessInThisGeneration = fitness[0];
		double bestFitnessInThisGeneration = population[0].getProblem()->oneObjectiveFitness;
		for (int i=0; i < populationSize; i++)
			//if (fitness[i] < bestFitnessInThisGeneration)
			if (population[i].getProblem()->oneObjectiveFitness < bestFitnessInThisGeneration)
				//bestFitnessInThisGeneration = fitness[i];
				bestFitnessInThisGeneration = population[i].getProblem()->oneObjectiveFitness;

		//programsToFilesAtImprovement(generation);

		//Keep track of best fitness improvements
		if (bestFitnessInThisGeneration < bestFitnessFoundSoFar)
		{
			fitIncrease = bestFitnessInThisGeneration - bestFitnessFoundSoFar;
			bestFitnessFoundSoFar = bestFitnessInThisGeneration;
			#ifdef WRITE_EVENTS_LOG_FILE
				fprintf(events, "Best fit: %2.12f in generation %d (improved after %d non-improving generations) (fit increase: %e)\n", bestFitnessInThisGeneration, generation, bestFitForGenerations, fitIncrease);
				fflush(events);
			#endif

			#ifdef WRITE_PROGRAMS_AT_IMPROVEMENTS
				programsToFilesAtImprovement(generation);
			#endif

			bestFitForGenerations = 0;
		}
		else
		{
			fitIncrease = 0;
			bestFitForGenerations++;
		}

		//Did i-th tree reach goal?
		for (int i=0; i < populationSize; i++)
		{
			if (population[i].wasGoalReached())
			{
				if (!goalReached) //Execute this part only once
				{
					goalReachedInGeneration = generation;
					forMaxGenerations = goalReachedInGeneration + afterGoalReachedGenerations + 1;
				}

				#ifdef WRITE_TO_STDOUT_IN_LOOP
					if (!goalReached) //Display only once
					{
						printf("Program %d reached goal (part of generation: %d)\n", i, generation);
						printf("After this iteration there will be execution of %d generations.\n", afterGoalReachedGenerations);
						printf("Evolution will end with generation: %d\n", generation + afterGoalReachedGenerations);
					}
				#endif
				#ifdef WRITE_EVENTS_LOG_FILE
					if (!goalReached) //Display only once
					{
						fprintf(events, "Program %d reached goal (part of generation: %d)\n", i, generation);
						fprintf(events, "After this iteration there will be execution of %d generations.\n", afterGoalReachedGenerations);
						fprintf(events, "Evolution will end with generation: %d\n", generation + afterGoalReachedGenerations);
					}
				#endif

				goalReached = true;
			}
		}

		#ifdef WRITE_TO_STDOUT_IN_LOOP
			loopLog(stdout, bestFitnessInThisGeneration, generation, bestFitForGenerations, fitIncrease);
		#endif
	}
	generation--;

	//Time measurement
	finishTime = clock();
	double elapsedTime = ( (finishTime - startTime)/(double)CLOCKS_PER_SEC );

	//TEXT OUTPUT
	#ifdef WRITE_TO_STDOUT_IN_LOOP
		endLog(stdout, generation, stepsInFitnessEval, elapsedTime);
	#endif
	#ifdef WRITE_EVENTS_LOG_FILE
		endLog(events, generation, stepsInFitnessEval, elapsedTime);
		fclose(events);	//Close events file
	#endif

	//getc(stdin);

	programsToFiles();
	
}

void GeneticProgramming::startLog(FILE *events, int &forMaxGenerations, int &afterGoalReachedGenerations, int &stepsInFitnessEval, int &sampleSize)
{
	//events = fopen("events.txt", "w");
	fprintf(events, "Population size: %d\nMax tree depth at init: %d\nMax tree depth in evolving: %d\n\n", populationSize, maxDepthOfInitialTree, maxTotalTreeDepth);
	fprintf(events, "Evolving for max. %d generations (trying to reach goal).\nEvolving for %d generations after goal is reached (after that loop ends).\nSteps in fitness evaluation: %d\nSample size for tournament: %d\n\n", forMaxGenerations, afterGoalReachedGenerations, stepsInFitnessEval, sampleSize);

	fprintf(events, "Available functions (%d):\n", Problem::getCardinalityOfFunctionSet());
	for (int i=0; i < Problem::getCardinalityOfFunctionSet(); i++)
	{
		Problem::Token temp = Problem::getFunction(i);
		fprintf(events, "\t%s (with arity = %d)\n", Problem::tokenToText(temp).c_str(), Problem::getArity(temp));
	}

	fprintf(events, "\nAvailable terminals (%d):\n", Problem::getCardinalityOfTerminalSet());
	for (int i=0; i < Problem::getCardinalityOfTerminalSet(); i++)
	{
		Problem::Token temp = Problem::getTerminal(i);
		fprintf(events, "\t%s\n", Problem::tokenToText(temp).c_str());
	}

	fprintf(events, "\nStarting evolution:\n");
	fflush(events);
}

void GeneticProgramming::endLog(FILE *events, int &generation, int &stepsInFitnessEval, double &elapsedTime)
{
	fprintf(events, "Ended evolving with generation: %d\n\n", generation);

	fprintf(events, "Program fitnesses from generation %d:\n", generation);
	double bestFit = fitness[0];
	int indBest = 0;
	for (int i=0; i < populationSize; i++)
	{
		fitness[i] = population[i].executeTreeAndGetFitness(stepsInFitnessEval);
		fprintf(events, "fitness[%d] = %f\n", i, fitness[i]);

		if (fitness[i] < bestFit)
		{
			bestFit = fitness[i];
			indBest = i;
		}
	}

	fprintf(events, "\nprogram%d.txt contains best program (fitness: %f)\n", indBest, bestFit);

	fprintf(events, "\nSTATISTICS\n");
	fprintf(events, "crossovers: %d\n", crossovers);
	fprintf(events, "copies: %d\n", copies);
	fprintf(events, "mutations: %d\n\n", mutations);

	fprintf(events, "\nEvolution time: %f\n", elapsedTime);
	fflush(events);
}

void GeneticProgramming::loopLog(FILE *events, double &bestFitnessInThisGeneration, int &generation, unsigned int &bestFitForGenerations, double &fitIncrease)
{
	int maxNumNodes = -1;
	int maxDepth = -1;
	int minCounterInvalid = population[0].getProblem()->getCounterInvalid();
	bool x = false;
	for (int i=0; i < populationSize; i++)
	{
		//Get number of nodes in tree
		int numNodes = population[i].getNumberOfTreeNodes();

		if (numNodes > maxNumNodes)
			maxNumNodes = numNodes;

		//Get tree depth
		int depth = population[i].getDepth();

		if (depth > maxDepth)
			maxDepth = depth;
		
		//Calculate invalidRatio
		int counterOk = population[i].getProblem()->getCounter();
		int counterInvalid = population[i].getProblem()->getCounterInvalid();
		double invalidRatio = (double)counterInvalid / (double)counterOk; //Je na intervalu [0,1]. Pri 0 ni napacnih ukazov, pri 1 vsi ukazi so napacni.

		if (counterInvalid < minCounterInvalid)
		{
			minCounterInvalid = counterInvalid;
		}

		//Print data for i-th tree
		printf("%d: numNodes: %d, depth: %d, fitness: %2.12f, counter: %d, counterInvalid: %d, invalidRatio: %f\n", i, numNodes, depth, fitness[i], counterOk, counterInvalid, invalidRatio);
	}

	//Output statistics about the entire current generation
	printf("Best fitness: %2.12f in generation %d (same for %d generations) (fit increase: %e)\n", bestFitnessInThisGeneration, generation, bestFitForGenerations, fitIncrease);
		
	printf("Max. number of nodes: %d\n", maxNumNodes);
	printf("Max. tree depth: %d\n", maxDepth);
	printf("Min. counter invalid: %d\n", minCounterInvalid);
	printf("\n");
	printf("\n");
}

inline Tree GeneticProgramming::makeCrossover(int &sampleSize)
{
	int parent1 = positiveTournament(fitness, sampleSize);
	int parent2 = positiveTournament(fitness, sampleSize);

	Tree tree1 = population[parent1];
	Tree tree2 = population[parent2];
	Tree newTree = tree1.crossover(tree2, maxTotalTreeDepth);
	crossovers++;

	return newTree;
}

inline Tree GeneticProgramming::makeCopy(int &sampleSize)
{
	int parent1 = positiveTournament(fitness, sampleSize);
	Tree tree1 = population[parent1];
	Tree newTree = tree1.duplicateTree();
	copies++;

	return newTree;
}

inline void GeneticProgramming::makeMutation(Tree treeToMutate)
{
	treeToMutate.mutate(maxDepthOfTreeAppendedAtMutation, maxTotalTreeDepth);
	mutations++;
}

inline void GeneticProgramming::makeMutationTokenChange(Tree treeToMutate)
{
	treeToMutate.mutateTokenChange();
	mutations++;
}

void GeneticProgramming::programsToFiles()
{
	printf("WRITING TO FILES\n");
	for (int i=0; i < populationSize; i++)
	{
		printf("Tree %d: Fitness %f\n", i, fitness[i]);

		char buffer [50];
		sprintf (buffer, "program%d.txt", i);

		FILE *file = fopen(buffer, "w");;
		population[i].printProgram(file, fitness[i]);

		fprintf(file, "\n\n\n");
		population[i].printTreeToFile(file, fitness[i]);
		fclose(file);
	}
}

void GeneticProgramming::programsToFilesAtImprovement(int generation)
{

	printf("WRITING TO FILES\n");
	for (int i=0; i < populationSize; i++)
	{
		printf("Tree %d: Fitness %f\n", i, fitness[i]);

		char buffer [50];
		sprintf (buffer, "generation%d_program%d.txt", generation, i);

		FILE *file = fopen(buffer, "w");;
		population[i].printProgram(file, fitness[i]);

		fprintf(file, "\n\n\n");
		population[i].printTreeToFile(file, fitness[i]);
		fclose(file);
	}
}

int GeneticProgramming::positiveTournament(double *fitness, int sampleSize)
{
	int best = randomInt(populationSize - 1);
	double fbest = fitness[best];

	for (int i=0; i < sampleSize; i++)
	{
		int competitor = randomInt(populationSize - 1);
		if (fitness[ competitor ] < fbest)
		{
			fbest = fitness[competitor];
			best = competitor;
		}
	}

	return best;

	/*
	int best = randomInt(populationSize - 1);
	//double fbest = fitness[best];
	bool isDominated = population[best]->getProblem()->dominated;

	double sum = 0;
	for (int j=0; j < Problem::numberOfObjectives; j++)
	{
		sum = sum + pow(population[best]->getProblem()->fitness[j],2);
	}
	sum = sqrt(sum);

	for (int i=0; i < sampleSize; i++)
	{
		int competitor = randomInt(populationSize - 1);

		double sumCompetitor = 0;
		for (int j=0; j < Problem::numberOfObjectives; j++)
		{
			sumCompetitor = sumCompetitor + pow(population[competitor]->getProblem()->fitness[j],2);
		}
		sumCompetitor = sqrt(sumCompetitor);

		if (sumCompetitor > sum)
		{
			isDominated = population[ competitor ]->getProblem()->dominated;
			best = competitor;
		}
	}

	return best;
	*/

	/*
	int best = randomInt(populationSize - 1);
	//double fbest = fitness[best];
	bool isDominated = population[best]->getProblem()->dominated;

	double sum = 0;
	for (int j=0; j < Problem::numberOfObjectives; j++)
	{
		sum = sum + pow(population[best]->getProblem()->fitness[j],2);
	}
	sum = sqrt(sum);

	for (int i=0; i < sampleSize; i++)
	{
		int competitor = randomInt(populationSize - 1);
		if ((population[ competitor ]->getProblem()->dominated == false && isDominated == true) || (population[ competitor ]->getProblem()->dominated == false && isDominated == false))
		{
			double sumCompetitor = 0;
			for (int j=0; j < Problem::numberOfObjectives; j++)
			{
				sumCompetitor = sumCompetitor + pow(population[competitor]->getProblem()->fitness[j],2);
			}
			sumCompetitor = sqrt(sumCompetitor);

			if (sumCompetitor > sum)
			{
				isDominated = population[ competitor ]->getProblem()->dominated;
				best = competitor;
			}
		}
	}

	return best;
	*/

	/*
		int best = randomInt(populationSize - 1);
		double fbest = fitness[best];

		for (int i=0; i < sampleSize; i++)
		{
			int competitor = randomInt(populationSize - 1);
			if (fitness[ competitor ] < fbest)
			{
				fbest = fitness[competitor];
				best = competitor;
			}
		}

		return best;
	*/
}

int GeneticProgramming::negativeTournament(double *fitness, int sampleSize)
{
		int worst = randomInt(populationSize - 1);
		double fworst = fitness[worst];

		for (int i=0; i < sampleSize; i++)
		{
			int competitor = randomInt(populationSize - 1);
			if (fitness[ competitor ] > fworst)
			{
				fworst = fitness[competitor];
				worst = competitor;
			}
		}
		return worst;
}

void GeneticProgramming::paretoOptimality()
{
	/*
	FOR i=1 TO N
		D = FALSE;
		j = 1;
		WHILE (NOT D) AND (j <= N)
			D = dominate(a[j], a[i]);
			j++;
		END WHILE
		IF (NOT D) output(a[i]);
	END FOR
	*/

	double delta = -(double)populationSize / 100;
	double incr = (double)populationSize / 1000;

	#ifdef WRITE_TO_STDOUT_IN_LOOP
		printf("List population:\n");
		Tree tree3;
		for (int i=0; i < populationSize; i++)
		{
			tree3 = population[i];
			printf("tree %d, fitness: ", i);
			for (int k=0; k < Problem::numberOfObjectives; k++)
				printf("%e ", tree3.getProblem()->fitness[k]);
			printf("\n");
		}
		printf("\n\n");
	#endif

	bool *beenNotDominated = new bool[populationSize];

	for (int i=0; i < populationSize; i++)
		beenNotDominated[i] = false;

	bool work = true;
	while (work)
	{
		Tree tree1, tree2;
		bool d;
		bool criteria1, criteria2;
		for (int i=0; i < populationSize; i++)
		{
			if (!beenNotDominated[i])
			{
				tree1 = population[i];
				d = false;
				int j = 0;

				while ((d == false) && (j < populationSize))
				{
					if (!beenNotDominated[j])
					{
						tree2 = population[j];

						criteria1 = true;
						criteria2 = false;
						for (int k=0; k < Problem::numberOfObjectives; k++)
						{
							double fit1 = tree1.getProblem()->fitness[k];
							double fit2 = tree2.getProblem()->fitness[k];

							if (fit1 < fit2)
							{
								criteria1 = false;
								break;
							}

							if (fit1 > fit2)
							{
								criteria2 = true;
							}
						}

						if (criteria1 && criteria2)
							d = true;
					}
					j++;
				}

				//tree1->getProblem()->dominated = d;

				if (d == false)
				{
					#ifdef WRITE_TO_STDOUT_IN_LOOP
						printf("tree %d is not dominated, fitness: ", i);
						for (int k=0; k < Problem::numberOfObjectives; k++)
							printf("%2.14f ", tree1.getProblem()->fitness[k]);
						printf("\n");
					#endif
					
					beenNotDominated[i] = true;
					fitness[i] = delta;
				}
			}
		}

		delta = delta + incr;

		bool temp = false;
		for (int i=0; i < populationSize; i++)
		{
			if (!beenNotDominated[i])
			{
				temp = true;
				break;
			}
		}

		work = temp;
	}

	delete [] beenNotDominated;

	#ifdef WRITE_TO_STDOUT_IN_LOOP
		printf("List population:\n");
		for (int i=0; i < populationSize; i++)
		{
			tree3 = population[i];
			printf("tree %d, fitness: %f\n", i, fitness[i]);
		}
		printf("\n\n");
	#endif
}