#pragma once

#include <stdio.h>
#include <math.h>
#include <string>
#include "Random.h"

class Problem
{
public:
	enum Token
	{
		//Do NOT modify names in CAPS

		//Functions (Non-leaf statements)
		EXPAND,		//0
		IfWallAhead,

		//End of first section
		DELIMETER1,

		//Terminals (Leaf statements)
		Move,		//6
		RotateClockwise,

		//End of second section
		DELIMETER2
	};

	Problem(void);
	~Problem(void);
	void restart();
	bool goalReached();
	int getX();
	int getY();
	static void printField();
	void printCurrentField();
	void interp(Problem::Token currentToken);
	bool eval(Problem::Token currentToken);
	static Token getRandomFunction();
	static Token getRandomTerminal();
	static Token getFunction(int indexOfFunction);
	static Token getTerminal(int indexOfTerminal);
	static bool isTerminal(Token token);
	static const int getCardinalityOfFunctionSet();
	static const int getCardinalityOfTerminalSet();
	static int getArity(Problem::Token function);
	double getFitnessOfThisStep();
	int getCounter();
	int getCounterInvalid();
	bool getWasGoalReached();
	static std::string tokenToText(Token token);

	static const int numberOfFunctions = (int)DELIMETER1;
	static const int numberOfTerminals = (int)(DELIMETER2 - DELIMETER1 - 1);

	static const int numberOfObjectives = 3;
	double fitness[numberOfObjectives];
	double oneObjectiveFitness;
	//bool dominated;
	//int rank;

private:

	int direction;

	//Grid size
	static const int xSize = 20;
	static const int ySize = 20;

	//Grid
	static const char field[ySize][xSize];

	//Current position
	int xPosition, yPosition;

	//Start position
	//static const int xPositionStart = 1, yPositionStart = 16;
	static const int xPositionStart = 1, yPositionStart = 1;

	//End position
	//static const int xPositionCilj = 18, yPositionCilj = 16;
	static const int xPositionCilj = 18, yPositionCilj = 18;

	//Counter of steps made since last restart
	int counter;

	//Counter of invalid actions
	int counterInvalid;

	//Was goal reached;
	bool wasGoalReached;

	//Visited
	bool visited[ySize][xSize];
	int distinctVisited;
};
