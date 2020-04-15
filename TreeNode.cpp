#include "TreeNode.h"

//StackTreeNodeArray TreeNode::stackTreeNodeArray(1000);

TreeNode::TreeNode()
{
	//children = descriptor(NULL, 0);
	children = NULL;
	numberOfChildren = 0;
	maxSubtreeIndex = 0;
	depth = 0;
}

TreeNode::~TreeNode(void)
{
	//if (children.tabela != NULL)
	if (children != NULL)
		delete [] children;
		//stackTreeNodeArray.push(children);
}

TreeNode::TreeNode(int newNumberOfChildren)
{
	children = new TreeNode*[newNumberOfChildren];
	//children = stackTreeNodeArray.pop(newNumberOfChildren);

	//Setirat na NULL ni nujno, je pa bol varno
	for (int i=0; i < newNumberOfChildren; i++)
		//children.tabela[i] = NULL;
		children[i] = NULL;

	numberOfChildren = newNumberOfChildren;

	maxSubtreeIndex = 0;
}

Problem::Token TreeNode::getToken()
{
	return token;
}

void TreeNode::setToken(Problem::Token currentToken)
{
	token = currentToken;
}

TreeNode *TreeNode::getChildren(int i)
{
	//return children.tabela[i];
	return children[i];
}

void TreeNode::setChildren(TreeNode *pointer, int i)
{
	//children.tabela[i] = pointer;
	children[i] = pointer;
}

TreeNode **TreeNode::getTabOfChildren()
{
	//return children.tabela;
	return children;
}

int TreeNode::getNumberOfChildren()
{
	return numberOfChildren;
}

void TreeNode::setMaxSubtreeIndex(int value)
{
	maxSubtreeIndex = value;
}

int TreeNode::getMaxSubtreeIndex()
{
	return maxSubtreeIndex;
}

void TreeNode::setDepth(int newDepth)
{
	depth = newDepth;
}

int TreeNode::getDepth()
{
	return depth;
}

/*
void TreeNode::changeNumberOfChildren(int numOfChilds)
{
	if (numberOfChildren != numOfChilds)
	{
		if (children.tabela != NULL)
			stackTreeNodeArray.push(children);

		numberOfChildren = numOfChilds;

		if (numOfChilds == 0)
		{
			children.tabela = NULL;
			children.size = 0;
		}
		else
		{
			children = stackTreeNodeArray.pop(numOfChilds);
		}


		maxSubtreeIndex = 0;
		depth = 0;
	}
}
*/

void TreeNode::setNoChildren()
{
	if (children != NULL)
		delete [] children;

	children = NULL;

	numberOfChildren = 0;

	maxSubtreeIndex = 0;
	depth = 0;
}


