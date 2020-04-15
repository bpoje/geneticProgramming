#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ON_WINDOWS false

inline double myRandom()
{
	double temp = rand() / (double)RAND_MAX;
	return temp;
}

inline int randomInt(int maxInt)
{
	if (!ON_WINDOWS)
		return (int)(myRandom() * (maxInt+1));
	else
	{
		int rnd;
		do
		{
			rnd = (int)(myRandom() * (maxInt+1));
		}
		while (rnd >= maxInt + 1);

		return rnd;
	}
}

inline int randomInt(int minInt, int maxInt)
{
	return randomInt(maxInt-minInt) + minInt;
}

inline bool randomBool()
{
	return (bool)randomInt(1);
}

/*
void testRandomBool()
{
	int tab[3];
	tab[0] = 0;
	tab[1] = 0;
	tab[2] = 0;

	const int ponovitev = 10000000;
	for (int i=0; i < ponovitev; i++)
	{
		tab[(int)randomBool()]++;
	}

	double tab1[3];
	for (int i=0; i < 3; i++)
	{
		tab1[i] = (double)tab[i] / ponovitev;
		printf("i=%d: %d / %d = %f\n", i, tab[i], ponovitev, tab1[i]);
	}
	printf("Vsota: %f\n", tab1[0] + tab1[1] + tab1[2]);
}

void testRandomInt()
{
	const int dost = 6;
	const int dolzinaTab = dost + 4;
	const int ponovitev = 10000000;
	int tab[dolzinaTab];

	for (int i=0; i < dolzinaTab; i++)
		tab[i] = 0;

	for (int i=0; i < ponovitev; i++)
	{
		int rnd = randomInt(dost);
		tab[rnd]++;
		//printf("%d\n", rnd);
		//printf("tab[%d] = %d\n", rnd, tab[rnd]);
	}

	double tab1[dolzinaTab];

	double vsota = 0;
	for (int i=0; i < dolzinaTab; i++)
	{
		tab1[i] = (double)tab[i] / (double)ponovitev;
		vsota += tab1[i];
		printf("i=%d: %d / %d = %f\n", i, tab[i], ponovitev, tab1[i]);
	}

	printf("Vsota: %f\n", vsota);
}
*/
