CC=g++

main: Problem.o TreeNode.o Tree.o GeneticProgramming.o main.o
	$(CC) Problem.o TreeNode.o Tree.o GeneticProgramming.o main.o -o main

Problem.o: Problem.cpp Problem.h Random.h
	$(CC) -c Problem.cpp

TreeNode.o: TreeNode.cpp TreeNode.h Problem.h
	$(CC) -c TreeNode.cpp

Tree.o: Tree.cpp Tree.h TreeNode.h
	$(CC) -c Tree.cpp

GeneticProgramming.o: GeneticProgramming.cpp GeneticProgramming.h Tree.h Global.h
	$(CC) -c GeneticProgramming.cpp

main.o: main.cpp Random.h GeneticProgramming.h
	$(CC) -c main.cpp

clean:
	rm *.o main
