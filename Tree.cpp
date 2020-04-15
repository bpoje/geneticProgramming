#include "Tree.h"

//StackTreeNode Tree::stackTreeNode(1000);

//StackTreeNode Tree::stackTreeNode(4,false);
//StackTreeNodeArray Tree::stackTreeNodeArray(4);

Tree::Tree(void)
{
	root = NULL;
	problem = NULL;
}

Tree::~Tree(void)
{
	//deleteTree();
}

void Tree::createProblem()
{
	problem = new Problem();
}

void Tree::destroyProblem()
{
	delete problem;
	problem = NULL;
}

void Tree::gen_rnd(int max_d, bool methodIsGrow)
{
	gen_rnd_expr(&root, 0, max_d, methodIsGrow);
}

void Tree::gen_rnd_expr(TreeNode **father, int childNumber, int max_d, bool methodIsGrow)
{
	TreeNode *treeNode;

	double temp = (double)Problem::getCardinalityOfTerminalSet() / ((double)Problem::getCardinalityOfTerminalSet() + (double)Problem::getCardinalityOfFunctionSet());
	Problem::Token expr;

	if (max_d == 0 || (methodIsGrow == true && myRandom() < temp))
	{
		expr = Problem::getRandomTerminal();

		treeNode = new TreeNode();
		//treeNode = stackTreeNode.pop();

		treeNode -> setToken(expr);
		father[childNumber] = treeNode;
	}
	else
	{
		Problem::Token func = Problem::getRandomFunction();
		int currentArity = Problem::getArity(func);

		treeNode = new TreeNode(currentArity);
		//treeNode = stackTreeNode.pop(currentArity);

		treeNode -> setToken(func);
		father[childNumber] = treeNode;
		for (int i=0; i < currentArity; i++)
		{
			gen_rnd_expr(treeNode->getTabOfChildren(), i, max_d - 1, methodIsGrow);
		}
	}
}

void Tree::printTree()
{
	printf("------OUTPUTING TREE:------\n");
	printStep(root);
	printf("-----END OF TREE OUTPUT----\n");
}

void Tree::printStep(TreeNode *node)
{
	if (node == NULL)
		return;

	if (node -> getNumberOfChildren() == 0)
	{
		printf("Terminal at %p with token %d and minIndex %d\n", (unsigned int*)node, node->getToken(), node->getMaxSubtreeIndex());
	}
	else
	{
		printf("Function at %p with token %d, minIndex %d and %d childs.\n", (unsigned int*)node, node->getToken(), node->getMaxSubtreeIndex(), node->getNumberOfChildren());
		printf("Listing children:\n");
		for (int i=0; i < node->getNumberOfChildren(); i++)
		{
			TreeNode *temp = node->getChildren(i);
			printf("Child at %p with token %d and minIndex %d\n", (unsigned int*)temp, temp->getToken(), temp->getMaxSubtreeIndex());
		}
		printf("\n");

		for (int i=0; i < node->getNumberOfChildren(); i++)
		{
			TreeNode *temp = node->getChildren(i);
			printStep(temp);
		}
	}
}

void Tree::deleteTree()
{
	deleteStep(root);
	root = NULL;
}

void Tree::deleteStep(TreeNode *node)
{
	if (node == NULL)
		return;

	if (node -> getNumberOfChildren() == 0)
	{
		//printf("Deleting terminal at %x\n", node);
		delete node;
		//stackTreeNode.push(node);
	}
	else
	{
		for (int i=0; i < node->getNumberOfChildren(); i++)
		{
			TreeNode *temp = node->getChildren(i);
			deleteStep(temp);
		}
		//printf("Deleting function at %x\n", node);
		delete node;
		//stackTreeNode.push(node);
	}
}

int Tree::getDepth()
{
	return getDepthStep(root, 0);
}

int Tree::getDepthStep(TreeNode *node, int depth)
{
	if (node == NULL)
		return -1;

	if (node -> getNumberOfChildren() == 0)
	{
		node->setDepth(depth);
		return depth;
	}
	else
	{
		//It's function and we know it has at least one child
		node->setDepth(depth);

		int maxFoundDepth = 1;
		for (int i=0; i < node->getNumberOfChildren(); i++)
		{
			TreeNode *temp = node->getChildren(i);
			int currentDepth = getDepthStep(temp, depth + 1);

			if (currentDepth > maxFoundDepth)
				maxFoundDepth = currentDepth;
		}
		return maxFoundDepth;
	}
}

int Tree::getNumberOfTreeNodes()
{
	return getNumberOfTreeNodesStep(root);
}

int Tree::getNumberOfTreeNodesStep(TreeNode *node)
{
	if (node == NULL)
		return -1;

	if (node -> getNumberOfChildren() == 0)
	{
		return 1;
	}
	else
	{
		//It's function and we know it has at least one child
		int n = 0;
		for (int i=0; i < node->getNumberOfChildren(); i++)
		{
			TreeNode *temp = node->getChildren(i);
			n += getNumberOfTreeNodesStep(temp);
		}
		return ++n;
	}
}

void Tree::executeTree(int times)
{
	for (int i=0; i < times && !problem->getWasGoalReached(); i++)
	{
		executeTreeStep(root);
	}
}

void Tree::executeTreeStep(TreeNode *node)
{
	if (node == NULL)
		return ;

	if (node -> getNumberOfChildren() == 0)
	{
		Problem::Token token = node->getToken();
		problem->interp(token);
	}
	else
	{
		//It's function and we know it has at least one child
		Problem::Token token = node->getToken();

		//Expand means we process all subtrees of node
		if (token == Problem::EXPAND)
		{
			for (int i=0; i < node->getNumberOfChildren(); i++)
			{
				TreeNode *temp = node->getChildren(i);
				executeTreeStep(temp);
			}
		}
		else
		{
			//If function has 2 subtrees (true and false subtree)
			if (Problem::getArity(token) == 2)
			{
				if (problem->eval(token))
				{
					executeTreeStep(node->getChildren(0));
				}
				else
				{
					executeTreeStep(node->getChildren(1));
				}
			}
			//Function with other than 2 subtrees
			else
			{

			}
		}

		return ;
	}
}

void Tree::printProgram(FILE *out, double fit)
{
	int numberOfObjectives = Problem::numberOfObjectives;
	fprintf(out, "Number of objectives: %d\n", numberOfObjectives);

	for (int i=0; i < numberOfObjectives; i++)
	{
		fprintf(out, "fitness[%d]: %f\n", i, problem->fitness[i]);
	}

	fprintf(out, "-------------------\n\n");
	fprintf(out, "Program fitness: %f\n", fit);
	fprintf(out, "One objective fitness: %f\n", problem->oneObjectiveFitness);

	fprintf(out, "\nPosition at end is: (y: %d, x: %d)\n\n", problem->getY(), problem->getX());

	if (problem->getWasGoalReached())
		fprintf(out, "Program solved our problem: True\n\n");
	else
		fprintf(out, "Program solved our problem: False\n\n");

	printProgramStep(root, 0, out);
}

void Tree::printProgramStep(TreeNode *node, int step, FILE *out)
{
	if (node == NULL)
		return ;

	if (node -> getNumberOfChildren() == 0)
	{	
		for (int i=0; i < step; i++)
			fprintf(out, "\t");

		Problem::Token token = node->getToken();
		std::string text = Problem::tokenToText(token);
		fprintf(out, "%s\n", text.c_str());
	}
	else
	{
		//It's function and we know it has at least one child
		Problem::Token token = node->getToken();

		//Expand means we process all subtrees of node
		if (token == Problem::EXPAND)
		{
			for (int i=0; i < node->getNumberOfChildren(); i++)
			{
				TreeNode *temp = node->getChildren(i);
				printProgramStep(temp, step, out);
			}
		}
		else
		{
			for (int i=0; i < step; i++)
				fprintf(out, "\t");
			fprintf(out, "if (%s) then\n", Problem::tokenToText(token).c_str());
			step++;

			//If function has 2 subtrees (true and false subtree)
			//if (Problem::getArity(token) == 2)
			//{
				//if (problem->eval(token))
				//{
					printProgramStep(node->getChildren(0), step, out);
				//}

				step--;
				for (int i=0; i < step; i++)
					fprintf(out, "\t");
				fprintf(out, "else\n");
				step++;

				//else
				//{
					printProgramStep(node->getChildren(1), step, out);
				//}

				step--;
				for (int i=0; i < step; i++)
					fprintf(out, "\t");
				fprintf(out, "End if\n");

			//}
			//Function with other than 2 subtrees
			//else
			//{
			//}
		}

		return ;
	}
}

double Tree::executeTreeAndGetFitness(int steps)
{
	problem->restart();

	//printf("------------------------\n");
	//printf("executed for %d steps\n", steps);
	executeTree(steps);

	//printf("Reached position: y: %d, x: %d\n", problem->getY(), problem->getX());
	//printf("Counter value: %d\n", problem->getCounter());
	//printf("CounterInvalid value: %d\n\n", problem->getCounterInvalid());

	//problem->printCurrentField();
	//printf("\n----------------------\n");

	return problem->getFitnessOfThisStep();
}

void Tree::mutate(int max_depth, int maxTotalTreeDepth)
{
	TreeNode **parentsPointer = selectRandomNodeParentPointer();

	getDepth();
	int remainingDepth = maxTotalTreeDepth - (*parentsPointer)->getDepth();
	if (remainingDepth < 0)
		remainingDepth = -remainingDepth;

	int randomTreeDepth = max_depth;

	if (remainingDepth < max_depth)
		randomTreeDepth = remainingDepth;


	Tree *temp = new Tree();
	temp->setRootAndDeleteOldTree(*parentsPointer);
	temp->deleteTree();

	int max_d = randomInt(0,randomTreeDepth);
	bool methodIsGrow = randomBool();
	//printf("max_d: %d, methodIsGrow: %d\n", max_d, methodIsGrow);
	temp->gen_rnd(max_d, methodIsGrow);
	*parentsPointer = temp->getRoot();
	temp->setRoot(NULL);

	delete temp;
}

void Tree::mutateTokenChange()
{
	TreeNode **parentsPointer = selectRandomNodeParentPointer();

	Problem::Token token = (*parentsPointer)->getToken();
	Problem::Token temp = token;
	Problem::Token newToken;

	if (Problem::isTerminal(token))
	{
		int numTerminals = Problem::numberOfTerminals;
		int indOfRandomTerminal;

		indOfRandomTerminal = randomInt(numTerminals-1);
		newToken = Problem::getTerminal(indOfRandomTerminal);

		token = newToken;
	}
	else
	{
		int numFunctions = Problem::numberOfFunctions;
		int tokenArity = Problem::getArity(token);
		int indOfRandomFunction;

		int newTokenArity;

		do
		{
			indOfRandomFunction = randomInt(numFunctions-1);
			newToken = Problem::getFunction(indOfRandomFunction);
			newTokenArity = Problem::getArity(newToken);
		}
		while (tokenArity != newTokenArity);

		token = newToken;
	}

	//printf("Changing token %s to %s\n", Problem::tokenToText(temp).c_str(), Problem::tokenToText(newToken).c_str());
}

void Tree::setRoot(TreeNode *newRoot)
{
	root = newRoot;
}

void Tree::setRootAndDeleteOldTree(TreeNode *newRoot)
{
	if (root != NULL)
		deleteTree();

	root = newRoot;
}

TreeNode *Tree::getRoot()
{
	return root;
}

Tree Tree::crossover(Tree partner, int maxTotalTreeDepth)
{
	if (partner.root == NULL)
		return Tree();

	//Duplicate entire tree so we can modify it without altering the original individuals
	Tree offspring = duplicateSubTree(&root);

	//Select a random node (subtree) in partner tree
	TreeNode **partnersRandomNode = partner.selectRandomNodeParentPointer();

	//Duplicate only the subtree
	Tree partnerSubtreeCopy = duplicateSubTree(partnersRandomNode);

	//printf("duplicated tree1:\n");
	//offspring->printTree();
	//printf("\n");

	//Select a random node in offspring tree
	TreeNode **offspringsRandomNode = offspring.selectRandomNodeParentPointer();

	//printf("POINTER1:\n");
	//printf("Adress of parents pointer to randomly selected node: %x\n", offspringsRandomNode);
	//printf("Adress of randomly selected node: %x\n", *offspringsRandomNode);

	//printf("POINTER2:\n");
	//printf("Adress of parents pointer to randomly selected node: %x\n", partnersRandomNode);
	//printf("Adress of randomly selected node: %x\n", *partnersRandomNode);

	//Partners subtree is glued to offspring
	//*offspringsRandomNode = *partnersRandomNode;
	TreeNode *temp = *offspringsRandomNode;
	*offspringsRandomNode = partnerSubtreeCopy.getRoot();
	partnerSubtreeCopy.setRoot(temp);
	//delete partnerSubtreeCopy;
	partnerSubtreeCopy.deleteTree();

	offspring.deleteAfterDepth(maxTotalTreeDepth);

	return offspring;
}

Tree Tree::duplicateTree()
{
	return duplicateSubTree(&root);
}

Tree Tree::duplicateSubTree(TreeNode **subTreeParentPointer)
{
	if (subTreeParentPointer == NULL && *subTreeParentPointer == NULL)
		return Tree();

	TreeNode *tempRoot = NULL;

	duplicateSubTreeStep(*subTreeParentPointer, &tempRoot);

	Tree tree;
	tree.setRoot(tempRoot);

	return tree;
}

void Tree::duplicateSubTreeStep(TreeNode *node, TreeNode **newNodeFather)
{
	if (node == NULL)
		return;

	if (node -> getNumberOfChildren() == 0)
	{
		TreeNode *newNode = new TreeNode(0);
		//TreeNode *newNode = stackTreeNode.pop(0);

		newNode->setToken(node->getToken());
		*newNodeFather = newNode;
	}
	else
	{
		TreeNode *newNode = new TreeNode(node->getNumberOfChildren());
		//TreeNode *newNode = stackTreeNode.pop(node->getNumberOfChildren());

		newNode->setToken(node->getToken());
		*newNodeFather = newNode;

		for (int i=0; i < node->getNumberOfChildren(); i++)
		{
			TreeNode *temp = node->getChildren(i);
			TreeNode **tabOfChildren = newNode->getTabOfChildren();
			duplicateSubTreeStep(temp, tabOfChildren + i);
		}
	}
}

void Tree::printTreeToFile(FILE *file, double fitness)
{
	fprintf(file, "------OUTPUTING TREE:------\n");
	fprintf(file, "Fitness: %f\n", fitness);
	printTreeToFileStep(root,file);
	fprintf(file, "-----END OF TREE OUTPUT----\n");
}

void Tree::printTreeToFileStep(TreeNode *node, FILE *file)
{
	if (node == NULL)
		return;

	if (node -> getNumberOfChildren() == 0)
	{
		fprintf(file, "Terminal at %p with token: %d\n", (unsigned int*)node, node->getToken());
	}
	else
	{
		fprintf(file, "Function at %p with token %d and %d childs.\n", (unsigned int*)node, node->getToken(), node->getNumberOfChildren());
		fprintf(file, "Listing children:\n");
		for (int i=0; i < node->getNumberOfChildren(); i++)
		{
			TreeNode *temp = node->getChildren(i);
			fprintf(file, "Child at %p with token: %d\n", (unsigned int*)temp, temp->getToken());
		}
		fprintf(file, "\n");

		for (int i=0; i < node->getNumberOfChildren(); i++)
		{
			TreeNode *temp = node->getChildren(i);
			printTreeToFileStep(temp, file);
		}
	}
}

bool Tree::wasGoalReached()
{
	return problem->getWasGoalReached();
}

int Tree::prepareIndexes()
{
	return prepareIndexesStep(root, 0);
}

int Tree::prepareIndexesStep(TreeNode *node, int counter)
{
	if (node == NULL)
		return 0;

	if (node -> getNumberOfChildren() == 0)
	{
		node->setMaxSubtreeIndex(counter);
		counter++;
		return counter;
	}
	else
	{
		for (int i=0; i < node->getNumberOfChildren(); i++)
		{
			TreeNode *temp = node->getChildren(i);
			counter = prepareIndexesStep(temp, counter);
		}

		node->setMaxSubtreeIndex(counter);
		counter++;

		return counter;
	}
}

TreeNode *Tree::selectRandomNode()
{
	int n = prepareIndexes();
	int searchForIndex = randomInt(n-1);
	return selectRandomNodeStep(root, searchForIndex);
}

TreeNode *Tree::selectRandomNodeStep(TreeNode *node, int searchForIndex)
{
	if (node == NULL)
		return NULL;

	//TreeNode returned = NULL;

	if (node -> getNumberOfChildren() == 0)
	{
		return node;
	}
	else
	{
		int nChild = node->getNumberOfChildren();
		//int random = randomInt(0, nChild - 1);
		//TreeNode *temp = node->getChildren(random);

		if (searchForIndex == node->getMaxSubtreeIndex())
			return node;

		int index = 0, indexPrevious = 0;
		for (int i=0; i < nChild; i++)
		{
			TreeNode *temp = node->getChildren(i);
			index = temp->getMaxSubtreeIndex();

			if (indexPrevious <= searchForIndex && searchForIndex <= index)
			{
				return selectRandomNodeStep(temp, searchForIndex);
			}

			indexPrevious = index;
		}
	}

	return NULL;
}

void Tree::testSelectRandomNode(int numOfTests)
{
	int n = getNumberOfTreeNodes();
	TreeNode **tab = new TreeNode*[n];
	int *tab1 = new int[n];
	double *tab2 = new double[n];

	for (int i=0; i < n; i++)
	{
		tab[i] = NULL;
		tab1[i] = 0;
	}

	//Fill the table tab with node addresses
	testSelectRandomNodeStep(root, tab, 0);

	/*
	printf("\n");
	for (int i=0; i < n; i++)
	{
		printf("%d: %x\n",i, tab[i]);
	}
	printf("\n");
	*/

	//int numOfTests = 10;
	for (int i=0; i < numOfTests; i++)
	{
		TreeNode *temp = selectRandomNode();

		for (int j=0; j < n; j++)
		{
			if (tab[j] == temp)
				tab1[j]++;
		}
	}

	double sum = 0;
	for (int i=0; i < n; i++)
	{
		tab2[i] = (double)tab1[i] / (double)numOfTests;
		sum += tab2[i];
	}

	printf("\n");
	printf("Number of all TreeNodes is %d\n", n);
	printf("Tested on %d random picks\n", numOfTests);
	for (int i=0; i < n; i++)
	{
		printf("%d: %p has %d occurences or %f = %f (difference from the mean: %f)\n",i, (unsigned int*)tab[i], tab1[i], tab2[i], tab2[i] * (double)100, ((double)1/n-tab2[i]));
	}
	printf("Sum in total: %f\n", sum);
	printf("\n");

	delete [] tab;
	delete [] tab1;
	delete [] tab2;
}

int Tree::testSelectRandomNodeStep(TreeNode *node, TreeNode **tab, int tabCount)
{
	if (node == NULL)
		return 0;

	if (node -> getNumberOfChildren() == 0)
	{
		tab[tabCount] = node;
		tabCount++;
	}
	else
	{
		//It's function and we know it has at least one child
		tab[tabCount] = node;
		tabCount++;
		for (int i=0; i < node->getNumberOfChildren(); i++)
		{
			TreeNode *temp = node->getChildren(i);
			tabCount = testSelectRandomNodeStep(temp, tab, tabCount);
		}
	}

	return tabCount;
}

TreeNode **Tree::selectRandomNodeParentPointer()
{
	int n = prepareIndexes();
	int searchForIndex = randomInt(n-1);
	return selectRandomNodeParentPointerStep(root, &root, searchForIndex);
}

TreeNode **Tree::selectRandomNodeParentPointerStep(TreeNode *node, TreeNode **parent, int searchForIndex)
{
	if (node == NULL)
		return NULL;

	//TreeNode returned = NULL;

	if (node -> getNumberOfChildren() == 0)
	{
		return parent;
	}
	else
	{
		if (searchForIndex == node->getMaxSubtreeIndex())
			return parent;

		int nChild = node->getNumberOfChildren();

		int index = 0, indexPrevious = 0;
		for (int i=0; i < nChild; i++)
		{
			TreeNode *temp = node->getChildren(i);
			index = temp->getMaxSubtreeIndex();

			if (indexPrevious <= searchForIndex && searchForIndex <= index)
			{
				parent = node->getTabOfChildren() + i;
				return selectRandomNodeParentPointerStep(temp, parent, searchForIndex);
			}

			indexPrevious = index;
		}
	}

	return NULL;
}

void Tree::testSelectRandomNodeParentPointer(int numOfTests)
{
	int n = getNumberOfTreeNodes();
	TreeNode **tab = new TreeNode*[n];
	int *tab1 = new int[n];
	double *tab2 = new double[n];

	for (int i=0; i < n; i++)
	{
		tab[i] = NULL;
		tab1[i] = 0;
	}

	//Fill the table tab with node addresses
	testSelectRandomNodeParentPointerStep(root, tab, 0);

	/*
	printf("\n");
	for (int i=0; i < n; i++)
	{
		printf("%d: %x\n",i, tab[i]);
	}
	printf("\n");
	*/

	//int numOfTests = 10;
	for (int i=0; i < numOfTests; i++)
	{
		TreeNode **temp = selectRandomNodeParentPointer();

		for (int j=0; j < n; j++)
		{
			if (tab[j] == *temp)
				tab1[j]++;
		}
	}

	double sum = 0;
	for (int i=0; i < n; i++)
	{
		tab2[i] = (double)tab1[i] / (double)numOfTests;
		sum += tab2[i];
	}

	printf("\n");
	printf("Number of all TreeNodes is %d\n", n);
	printf("Tested on %d random picks\n", numOfTests);
	for (int i=0; i < n; i++)
	{
		printf("%d: %p has %d occurences or %f = %f (difference from the mean: %f)\n",i, (unsigned int*)tab[i], tab1[i], tab2[i], tab2[i] * (double)100, ((double)1/n-tab2[i]));
	}
	printf("Sum in total: %f\n", sum);
	printf("\n");

	delete [] tab;
	delete [] tab1;
	delete [] tab2;
}

int Tree::testSelectRandomNodeParentPointerStep(TreeNode *node, TreeNode **tab, int tabCount)
{
	if (node == NULL)
		return 0;

	if (node -> getNumberOfChildren() == 0)
	{
		tab[tabCount] = node;
		tabCount++;
	}
	else
	{
		//It's function and we know it has at least one child
		tab[tabCount] = node;
		tabCount++;
		for (int i=0; i < node->getNumberOfChildren(); i++)
		{
			TreeNode *temp = node->getChildren(i);
			tabCount = testSelectRandomNodeStep(temp, tab, tabCount);
		}
	}

	return tabCount;
}

void Tree::deleteAfterDepth(int depth)
{
	deleteAfterDepthStep(root, depth);
}

void Tree::deleteAfterDepthStep(TreeNode *node, int depth)
{
	if (node == NULL)
		return;

	if (node -> getNumberOfChildren() == 0)
	{
		//printf("Deleting terminal at %x\n", node);
		if (depth < 0)
			delete node;
			//stackTreeNode.push(node);
	}
	else
	{
		for (int i=0; i < node->getNumberOfChildren(); i++)
		{
			TreeNode *temp = node->getChildren(i);
			deleteAfterDepthStep(temp, depth-1);
		}
		//printf("Deleting function at %x\n", node);
		if (depth < 0)
			delete node;
			//stackTreeNode.push(node);
		else if (depth == 0)
		{
			node->setNoChildren();
			//node->changeNumberOfChildren(0);

			Problem::Token token = node->getToken();
			if(!Problem::isTerminal(token))
				node->setToken(Problem::getRandomTerminal());
		}
	}
}

Problem *Tree::getProblem()
{
	return problem;
}

void Tree::setProblem(Problem *p)
{
	problem = p;
}