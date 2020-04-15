#pragma once

#include "TreeNode.h"

class Tree
{
public:
	enum Condition
	{
		FALSE,
		TRUE,
		NOT_DECIDED
	};

	Tree(void);
	~Tree(void);
	void createProblem();
	void destroyProblem();
	void gen_rnd_expr(TreeNode **father, int childNumber, int max_d, bool methodIsGrow);
	void gen_rnd(int max_d, bool methodIsGrow);
	void printTree();
	void printStep(TreeNode *father);
	void deleteTree();
	void deleteStep(TreeNode *node);
	int getDepth();
	int getDepthStep(TreeNode *node, int depth);
	int getNumberOfTreeNodes();
	int getNumberOfTreeNodesStep(TreeNode *node);
	void executeTree(int times);
	void executeTreeStep(TreeNode *node);
	void printProgram(FILE *out, double fit);
	void printProgramStep(TreeNode *node, int step, FILE *out);
	double executeTreeAndGetFitness(int steps);
	void mutate(int max_depth, int maxTotalTreeDepth);
	void mutateTokenChange();
	void setRoot(TreeNode *newRoot);
	void setRootAndDeleteOldTree(TreeNode *newRoot);
	TreeNode *getRoot();
	Tree crossover(Tree partner, int maxTotalTreeDepth);
	Tree duplicateTree();
	Tree duplicateSubTree(TreeNode **subTreeParentPointer);
	void duplicateSubTreeStep(TreeNode *node, TreeNode **newNodeFather);
	void printTreeToFile(FILE *file, double fitness);
	void printTreeToFileStep(TreeNode *node, FILE *file);
	bool wasGoalReached();
	int prepareIndexes();
	int prepareIndexesStep(TreeNode *node, int counter);
	TreeNode *selectRandomNode();
	TreeNode *selectRandomNodeStep(TreeNode *node, int searchForIndex);
	void testSelectRandomNode(int numOfTests);
	int testSelectRandomNodeStep(TreeNode *node, TreeNode **tab, int tabCount);
	TreeNode **selectRandomNodeParentPointer();
	TreeNode **selectRandomNodeParentPointerStep(TreeNode *node, TreeNode **parent, int searchForIndex);
	void testSelectRandomNodeParentPointer(int numOfTests);
	int testSelectRandomNodeParentPointerStep(TreeNode *node, TreeNode **tab, int tabCount);
	void deleteAfterDepth(int depth);
	void deleteAfterDepthStep(TreeNode *node, int depth);
	Problem *getProblem();
	void setProblem(Problem *p);

private:
	TreeNode *root;
	Problem *problem;
	//static StackTreeNode stackTreeNode;
	//static StackTreeNodeArray stackTreeNodeArray;
};
