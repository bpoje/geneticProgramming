#include "Problem.h"

/*
const char Problem::field[20][20] = {{'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
			      	     {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
			      	     {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
			      	     {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
			      	     {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1','1','1'},
			      	     {'1','0','1','1','1','1','1','1','1','1','1','1','1','1','1','1','0','1','1','1'},
			      	     {'1','0','1','1','1','1','1','1','1','1','1','1','1','1','1','1','0','1','1','1'},
			      	     {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1','0','1','1','1'},
			      	     {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','0','1','0','1','1','1'},
			      	     {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','0','1','0','1','1','1'},
			      	     {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1','0','1','1','1'},
			      	     {'1','0','1','1','1','1','1','1','1','1','1','1','1','1','1','1','0','1','1','1'},
			      	     {'1','0','1','1','1','1','1','1','1','1','1','1','1','1','1','1','0','1','1','1'},
			      	     {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1','0','1','1','1'},
			      	     {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','0','1','0','1','1','1'},
			      	     {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','0','1','0','1','1','1'},
			      	     {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1','0','0','0','1'},
			      	     {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
			      	     {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
			      	     {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}};
*/

const char Problem::field[20][20] = {{'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
			      	     {'1','c','0','0','0','0','0','1','0','0','0','0','0','0','0','0','0','0','0','1'},
			      	     {'1','0','1','1','1','1','0','0','0','1','1','1','1','1','1','1','1','1','0','1'},
			      	     {'1','0','0','0','1','0','0','1','0','1','0','0','0','0','0','0','0','1','0','1'},
			      	     {'1','1','1','0','1','0','0','0','0','1','0','1','1','1','1','1','0','1','0','1'},
			      	     {'1','0','0','0','1','1','1','1','1','1','1','1','0','0','0','1','0','1','0','1'},
			      	     {'1','0','1','1','1','0','1','0','0','0','0','1','0','0','0','1','0','1','0','1'},
			      	     {'1','0','0','0','0','0','1','0','1','1','0','1','0','0','0','1','0','1','0','1'},
			      	     {'1','0','0','0','1','0','1','0','1','0','0','1','0','0','0','1','0','1','0','1'},
			      	     {'1','0','1','1','1','1','1','0','1','0','0','1','0','0','0','1','0','1','0','1'},
			      	     {'1','0','0','0','0','0','1','0','1','0','0','0','0','0','0','1','0','1','0','1'},
			      	     {'1','0','1','0','1','1','1','0','1','1','1','0','0','0','0','1','0','1','0','1'},
			      	     {'1','0','1','0','0','0','1','0','0','0','1','0','0','0','0','1','0','1','0','1'},
			      	     {'1','0','1','1','1','0','1','0','1','0','1','0','0','0','0','1','0','1','0','1'},
			      	     {'1','0','1','0','1','0','0','0','1','0','1','0','0','0','0','1','0','0','0','1'},
			      	     {'1','0','1','0','1','0','1','1','1','0','1','0','0','0','0','1','1','1','1','1'},
			      	     {'1','0','1','0','1','0','0','0','1','0','1','0','0','0','0','0','0','0','0','1'},
			      	     {'1','0','1','0','1','0','1','1','1','0','1','0','0','0','0','0','0','0','0','1'},
			      	     {'1','0','0','0','1','0','0','0','1','0','1','0','0','0','0','0','0','0','0','1'},
			      	     {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}};

Problem::Problem(void)
{
	xPosition = xPositionStart;
	yPosition = yPositionStart;

	counter = 0;
	counterInvalid = 0;
	wasGoalReached = false;

	for (int i=0; i < ySize; i++)
		for (int j=0; j < xSize; j++)
			visited[i][j] = false;

	distinctVisited = 1;
	visited[yPositionStart][xPositionStart] = true;

	direction = 0;
}

Problem::~Problem(void)
{
}

void Problem::restart()
{
	xPosition = xPositionStart;
	yPosition = yPositionStart;

	counter = 0;
	counterInvalid = 0;
	wasGoalReached = false;

	for (int i=0; i < ySize; i++)
		for (int j=0; j < xSize; j++)
			visited[i][j] = false;

	distinctVisited = 1;
	visited[yPositionStart][xPositionStart] = true;
}

bool Problem::goalReached()
{
	return (xPosition == xPositionCilj && yPosition == yPositionCilj);
}

int Problem::getX()
{
	return xPosition;
}

int Problem::getY()
{
	return yPosition;
}

void Problem::printField()
{
	for (int i=0; i < ySize; i++)
	{
		for (int j=0; j < xSize; j++)
		{
			printf("%c", field[i][j]);
		}
		printf("\n");
	}
}

void Problem::printCurrentField()
{
	for (int i=0; i < ySize; i++)
	{
		for (int j=0; j < xSize; j++)
		{
			if (yPosition == i && xPosition == j)
				printf("+");
			else
				printf("%c", field[i][j]);
		}
		printf("\n");
	}
}

//Execute one leaf statement
void Problem::interp(Problem::Token currentToken)
{
	char temp;
	switch (currentToken)
	{
		case Move:
			switch (direction)
			{
				//Up 0
				case 0:
					temp = field[yPosition-1][xPosition];
					if (temp != '1')
					{
						yPosition--;
					}
					else
						counterInvalid++;
					break;

				//Down 1
				case 1:
					temp = field[yPosition+1][xPosition];
					if (temp != '1')
					{
						yPosition++;
					}
					else
						counterInvalid++;
					break;

				//Right 2
				case 2:
					temp = field[yPosition][xPosition+1];
					if (temp != '1')
					{
						xPosition++;
					}
					else
						counterInvalid++;
					break;

				//Left 3
				case 3:
					temp = field[yPosition][xPosition-1];
					if (temp != '1')
					{
						xPosition--;
					}
					else
						counterInvalid++;
					break;

				default:
					printf("Unknown direction!\n");
					break;

			}
			break;

		case RotateClockwise:
			switch (direction)
			{
				case 0:
					direction = 2;
					break;
				case 1:
					direction = 3;
					break;
				case 2:
					direction = 1;
					break;
				case 3:
					direction = 0;
					break;
			}
			break;

		default:
			printf("Unknown leaf statement!\n");
			break;
	}

	counter++;

	if (visited[yPosition][xPosition] == false)
	{
		visited[yPosition][xPosition] = true;
		distinctVisited++;
	}

	if (goalReached())
	{
		wasGoalReached = true;
	}
}

//Evaluate one non-leaf statement
bool Problem::eval(Problem::Token currentToken)
{
	char temp;
	bool checkVisited;
	switch (currentToken)
	{
		case IfWallAhead:
			switch(direction)
			{
				case 0:
					temp = field[yPosition-1][xPosition];
					if (temp == '1')
						return true;
					else
						return false;
					break;

				case 1:
					temp = field[yPosition+1][xPosition];
					if (temp == '1')
						return true;
					else
						return false;
					break;

				case 2:
					temp = field[yPosition][xPosition+1];
					if (temp == '1')
						return true;
					else
						return false;
					break;

				case 3:
					temp = field[yPosition][xPosition-1];
					if (temp == '1')
						return true;
					else
						return false;
					break;

				default:
					return false;
					break;
			}
			break;

		default:
			return false;
			break;
	}
}

Problem::Token Problem::getRandomFunction()
{
	int rnd = randomInt(0, DELIMETER1 - 1);
	return (Token)rnd;
}

Problem::Token Problem::getRandomTerminal()
{
	int rnd = randomInt(DELIMETER1 + 1, DELIMETER2 - 1);
	return (Token)rnd;
}

Problem::Token Problem::getFunction(int indexOfFunction)
{
	if (indexOfFunction >= 0 && indexOfFunction < numberOfFunctions)
		return (Token)indexOfFunction;
	else
		return (Token)0;
}

Problem::Token Problem::getTerminal(int indexOfTerminal)
{
	if (indexOfTerminal >= 0 && indexOfTerminal < numberOfTerminals)
		return (Token)(indexOfTerminal + numberOfFunctions + 1);
	else
		return (Token)0;
}

bool Problem::isTerminal(Token token)
{
	int num = (int)token;
	if (DELIMETER1 < num && num < DELIMETER2)
		return true;
	else
		return false;
}

const int Problem::getCardinalityOfFunctionSet()
{
	return (int)DELIMETER1;
}

const int Problem::getCardinalityOfTerminalSet()
{
	return (int)(DELIMETER2 - DELIMETER1 - 1);
}

int Problem::getArity(Problem::Token function)
{
	switch (function)
	{
		case EXPAND:
			return 2;
			break;

		case IfWallAhead:
			return 2;
			break;

		default:
			return 0;
	}
}

double Problem::getFitnessOfThisStep()
{
	//double dx2 = (xPositionCilj - xPosition)*(xPositionCilj - xPosition);
	//double dy2 = (yPositionCilj - yPosition)*(yPositionCilj - yPosition);

	//double df = sqrt(dx2 + dy2) * (counterInvalid + 1);

	double s;

	if (wasGoalReached)
		s = 1;
	else
		s = 0;

	//double df = 1 / (double)counter * s + (1 - 1 / (double)distinctVisited);

	//Zf2 = 1-invalidRatio = 1 - [0,1] = [1,0] = [0,1]
	double invalidRatio = (double)counterInvalid / (double)counter; //Je na intervalu [0,1]. Pri 0 ni napacnih ukazov, pri 1 vsi ukazi so napacni.

	//double df = 1 / (double)counter * s + (1-invalidRatio);
	//double df = 1 / (double)counter * s + (1 - 1 / (double)distinctVisited) + (1-invalidRatio);
	//double okRatio = (1-invalidRatio);

	//double sigmoid = (double)1 / (1 + exp(36.841361 * invalidRatio - 18.420681));
	//double sigmoid = 1;
	//double sigmoid = 1 - invalidRatio;
	//double df = 1 / (double)counter * s + (1 - 1 / (double)distinctVisited) * sigmoid;

	//Zf1 = [0,1)
	//double v = ((double)1 - (double)1 / (double)distinctVisited);
	double v = ((double)1 - (double)1 / ( (1-invalidRatio) * (double)distinctVisited));

	//Zf3 = [1,0)
	double g = (double)1 / (double)counter * s;

	//Zf1 + Zf2 = [0,1) + [0,1] = [0,2)
	//if s == 1: Zf1 + Zf2 + Zf3 = [0,2) + [1,0) = [1,2)
	//if s == 0: Zf1 + Zf2 + Zf3 = [0,2) + [0,0] = [0,2)
	//Interval: [0,2)

	//double sum = g * g + v * v + (1 - invalidRatio) * (1 - invalidRatio) / 1000000;

	//double sum = g + v + (1 - invalidRatio);
	double sum = g + v;
	double df = sum;

	//double df = sqrt(sum);

	//fitness[0] = -(double)1 / (double)counter * s; //Goal reached
	//fitness[1] = -((double)1 - (double)1 / (double)distinctVisited);
	//fitness[2] = -(1-invalidRatio);

	fitness[0] = -1 / (double)counter * s; //Goal reached
	fitness[1] = -(1-invalidRatio);
	fitness[2] = -(1 - 1 / (double)distinctVisited);

	//double df1 = 1 / (double)counter * s + (1 - 1 / (double)distinctVisited) + (1-invalidRatio);
	//double df1 = 1 / (double)counter * s + (1 - 1 / (double)distinctVisited);

	double df1 = fitness[0] + fitness[1] + fitness[2];

	oneObjectiveFitness = df1;
	return df1;
	//return -df;
}

int Problem::getCounter()
{
	return counter;
}

int Problem::getCounterInvalid()
{
	return counterInvalid;
}

bool Problem::getWasGoalReached()
{
	return wasGoalReached;
}

std::string Problem::tokenToText(Token token)
{
	switch (token)
	{
		case EXPAND:
			return std::string("Expand");
			break;

		case IfWallAhead:
			return std::string("IfWallAhead");
			break;

		case Move:
			return std::string("Move");
			break;

		case RotateClockwise:
			return std::string("RotateClockwise");
			break;

		default:
			return std::string("Unknown token!");
	}
}
