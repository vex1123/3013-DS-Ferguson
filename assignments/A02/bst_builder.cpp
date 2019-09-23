#include <iostream>
#include <fstream>
#include <string>


// Ryan Ferguson BST builder assignment
//CMPS-3013   M/W 3 -4:20  Professor Grifin
// --------------------------------------------------------------------------------------------------------
// This program creates an AVL tree that self rotates to become full or complete if and when possible. 
// This program accepts an input file of integers one at a time
// Credit : Professor Griffin provided the BST functions my inclusions are limited to 
// modifications of the balance, and insertion functions, with additions of the right andf left rotations.
// The main function is also written by myself.
// --------------------------------------------------------------------------------------------------------

using namespace std;


struct node
{
	int data;
	node *left;
	node *right;
	node()
	{
		data = -1;
		left = NULL;
		right = NULL;
	}
	node(int x)
	{
		data = x;
		left = NULL;
		right = NULL;
	}
};

class BSTree
{
private:
	node *root;

	int count(node *root)
	{
		if (!root)
		{
			return 0;
		}
		else
		{
			return 1 + count(root->left) + count(root->right);
		}
	}

	void insert(node *&root, node *&temp)
	{

		// start with the standard BST insertion
		if (!root)
		{
			root = temp;
		}
		else
		{
			if (temp->data < root->data)
			{
				insert(root->left, temp);
			}
			else if (temp->data > root->data)
			{
				insert(root->right, temp);
			}
			else
				return;
			//after determining the initial place to put our node we adjust based on balance

			int bal = balance();

			// their exist 4 cases of rotations in AVL trees based on the balance and index inserted

			if (bal > 1 && temp->data < root->left ->data)
			{
				rightRotate(root);
			}

			if (bal < -1 && temp->data > root->right->data)
			{
				leftRotate(root);
			}

			if (bal > 1 && temp->data > root->left->data)
			{
				leftRotate(root->left);
				rightRotate(root);
			}

			if (bal < -1 && temp->data < root->right->data)
			{
				rightRotate(root->right);
				leftRotate(root);
			}
		}
	}
	
	//functions to perform a rotation of the nodes in a tree
	void rightRotate(node* &root)
	{
		node *x = root->left;
		node *y = x->right;

		//perform rotation
		x->right = root;
		root->left = y;
	}


	void leftRotate(node* &root)
	{
		node *x = root->right;
		node *y = x->left;

		//perform rotation

		x->left = root;
		root->right = y;
	}

	void print_node(node *n, string label = "")
	{
		if (label != "")
		{
			cout << "[" << label << "]";
		}
		cout << "[[" << n << "][" << n->data << "]]\n";
		if (n->left)
		{
			cout << "\t|-->[L][[" << n->left << "][" << n->left->data << "]]\n";
		}
		else
		{
			cout << "\t\\-->[L][null]\n";
		}
		if (n->right)
		{
			cout << "\t\\-->[R][[" << n->right << "][" << n->right->data << "]]\n";
		}
		else
		{
			cout << "\t\\-->[R][null]\n";
		}
	}

	/**
	 * type = ['predecessor','successor']
	 */
	node *minValueNode(node *root)
	{
		node *current = root;

		if (root->right)
		{
			current = root->right;
			while (current->left != NULL)
			{
				current = current->left;
			}
		}
		else if (root->left)
		{
			current = root->left;
			while (current->right != NULL)
			{
				current = current->right;
			}
		}

		return current;
	}

	int height(node *root)
	{
		
		if (!root)
		{
			return 0;
		}
		else
		{
			int left = height(root->left);
			int right = height(root->right);
			if (left > right)
			{
				return left + 1;
			}
			else
			{
				return right + 1;
			}
		}
		
	}

	int balance(node *root)
	{
		// This function determines greater of the two heights of the subtrees to be
		//used later in the insertion and rotation functions.
		if (!root)
		{
			return 0;
		}
		else
		{
			int bal = height(root->left + 1) - height(root->right + 1);
		}
	}

	/* Print nodes at a given level */
	void printGivenLevel(node *root, int level)
	{
		if (root == NULL)
			return;
		if (level == 1)
		{
			print_node(root);
		}
		else if (level > 1)
		{
			printGivenLevel(root->left, level - 1);
			printGivenLevel(root->right, level - 1);
		}
	}
	//************************************************************************
	// Method to help create GraphViz code so the expression tree can
	// be visualized. This method prints out all the unique node id's
	// by traversing the tree.
	// Recivies a node pointer to root and performs a simple recursive
	// tree traversal.
	//************************************************************************
	void GraphVizGetIds(node *nodePtr, ofstream &VizOut)
	{
		static int NullCount = 0;
		if (nodePtr)
		{
			GraphVizGetIds(nodePtr->left, VizOut);
			VizOut << "node" << nodePtr->data
				<< "[label=\"" << nodePtr->data << "\\n"
				//<<"Add:"<<nodePtr<<"\\n"
				//<<"Par:"<<nodePtr->parent<<"\\n"
				//<<"Rt:"<<nodePtr->right<<"\\n"
				//<<"Lt:"<<nodePtr->left<<"\\n"
				<< "\"]" << endl;
			if (!nodePtr->left)
			{
				NullCount++;
				VizOut << "nnode" << NullCount << "[label=\"X\",shape=point,width=.15]" << endl;
			}
			GraphVizGetIds(nodePtr->right, VizOut);
			if (!nodePtr->right)
			{
				NullCount++;
				VizOut << "nnode" << NullCount << "[label=\"X\",shape=point,width=.15]" << endl;
			}
		}
	}

	//************************************************************************
	// This method is partnered with the above method, but on this pass it
	// writes out the actual data from each node.
	// Don't worry about what this method and the above method do, just
	// use the output as your told:)
	//************************************************************************
	void GraphVizMakeConnections(node *nodePtr, ofstream &VizOut)
	{
		static int NullCount = 0;
		if (nodePtr)
		{
			GraphVizMakeConnections(nodePtr->left, VizOut);
			if (nodePtr->left)
				VizOut << "node" << nodePtr->data << "->"
				<< "node" << nodePtr->left->data << endl;
			else
			{
				NullCount++;
				VizOut << "node" << nodePtr->data << "->"
					<< "nnode" << NullCount << endl;
			}

			if (nodePtr->right)
				VizOut << "node" << nodePtr->data << "->"
				<< "node" << nodePtr->right->data << endl;
			else
			{
				NullCount++;
				VizOut << "node" << nodePtr->data << "->"
					<< "nnode" << NullCount << endl;
			}

			GraphVizMakeConnections(nodePtr->right, VizOut);
		}
	}

public:
	BSTree()
	{
		root = NULL;
	}
	~BSTree()
	{
	}

	int count()
	{
		return count(root);
	}

	void insert(int x)
	{
		node *temp = new node(x);
		insert(root, temp);
	}

	void minValue()
	{
		print_node(minValueNode(root), "minVal");
	}

	int height()
	{

		return height(root);
	}

	//public function of balance to allow for root use in call
	int balance()
	{
		return balance(root);
	}

	int top()
	{
		if (root)
			return root->data;
		else
			return 0;
	}

	/* Function to line by line print level order traversal a tree*/
	void printLevelOrder()
	{
		cout << "Begin Level Order===================\n";
		int h = height(root);
		int i;
		for (i = 1; i <= h; i++)
		{
			printGivenLevel(root, i);
			cout << "\n";
		}
		cout << "End Level Order===================\n";
	}

	//************************************************************************
	// Recieves a filename to place the GraphViz data into.
	// It then calls the above two graphviz methods to create a data file
	// that can be used to visualize your expression tree.
	//************************************************************************
	void GraphVizOut(string filename)
	{
		ofstream VizOut;
		VizOut.open(filename);
		VizOut << "Digraph G {\n";
		GraphVizGetIds(root, VizOut);
		GraphVizMakeConnections(root, VizOut);
		VizOut << "}\n";
		VizOut.close();
	}
};

void main()
{

	//open files
	//note: outfile is not used but left in for ease of acess
	ifstream infile;
	ofstream outfile;
	infile.open("input.dat");
	outfile.open("out.txt");

	int i;
	BSTree B;
	//pre read for while loop condition
	infile >> i;

	//first read until end of file and insert all values into tree
	while(!infile.eof())
	{
		B.insert(i);
		infile >> i;
	}
	B.printLevelOrder();



	
}
