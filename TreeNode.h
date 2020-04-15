#pragma once

#include "Problem.h"

class TreeNode
{
public:
	TreeNode(void);
	~TreeNode(void);
	TreeNode(int newNumberOfChildren);
	Problem::Token getToken();
	void setToken(Problem::Token token);
	TreeNode *getChildren(int i);
	void setChildren(TreeNode *pointer, int i);
	TreeNode **getTabOfChildren();
	int getNumberOfChildren();
	void setMaxSubtreeIndex(int value);
	int getMaxSubtreeIndex();
	void setDepth(int newDepth);
	int getDepth();
	//void changeNumberOfChildren(int numOfChilds);
	void setNoChildren();

private:
	TreeNode **children;
	//descriptor children;
	int numberOfChildren;
	Problem::Token token;
	int maxSubtreeIndex;
	int depth;
};