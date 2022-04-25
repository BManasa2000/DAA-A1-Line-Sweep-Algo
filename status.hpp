#ifndef STATUS_HPP
#define STATUS_HPP

// #include <bits/stdc++.h>
#include <iostream>
using namespace std;

/**Structure to represent a line segment in terms of endpoints */
struct lineSegment
{
	double sx, sy, ex, ey;
};

/**Structure to represent a node in the status AVL tree */
struct statusNode
{
	lineSegment l;
	statusNode *left;
	statusNode *right;
	// double slope;
	// double yint;
	int height;
};

/**A class for inserting and deleting a line from an AVL tree containing the current status of the sweep line. 
* It also contains functions to find the neighbours of either a point or a line.
*/
class statusTree
{
	public:
    	// globalinsert variable to signify inserting two nodes for each line
		int globalInsert;

		statusTree()
		{
			globalInsert = 0;
		}

		// #########################################################################################
    	/**Function to find the height of the status node in the AVL tree*/
		int statusHeight(statusNode *N)
		{
			if (N == NULL)
				return 0;
			return N->height;
		}

		// #########################################################################################
		/**Function to create a new status node.
		* @param l - new line segment to be inserted
		* @return pointer to new status node
		*/
		statusNode* statusNewNode(lineSegment l)
		{
			statusNode* node = new statusNode();
			node->l.sx = l.sx;
			node->l.sy = l.sy;
			node->l.ex = l.ex;
			node->l.ey = l.ey;
			node->left = NULL;
			node->right = NULL;
			node->height = 1; // new node is initially added at leaf
			return(node);
		}

		// #########################################################################################
		/**Function for right rotating about a node in the AVL tree for rebalancing
		* @param y - status node about which to rotate
		*/
		statusNode *statusRightRotate(statusNode *y)
		{
			statusNode *x = y->left;
			statusNode *T2 = x->right;
			// Perform rotation
			x->right = y;
			y->left = T2;
			// Update heights
			y->height = max(statusHeight(y->left), statusHeight(y->right)) + 1;
			x->height = max(statusHeight(x->left), statusHeight(x->right)) + 1;
			// Return new root
			return x;
		}

		// #########################################################################################
		/**Function for left rotating about a node in the AVL tree for rebalancing
		* @param x - status node about which to rotate 
		*/
		statusNode *statusLeftRotate(statusNode *x)
		{
			statusNode *y = x->right;
			statusNode *T2 = y->left;
			// Perform rotation
			y->left = x;
			x->right = T2;
			// Update heights
			x->height = max(statusHeight(x->left), statusHeight(x->right)) + 1;
			y->height = max(statusHeight(y->left), statusHeight(y->right)) + 1;
			return y;
		}

		// #########################################################################################
    	/**Function to get the balance factor of a status node*/
		int statusGetBalance(statusNode *N)
		{
			if (N == NULL)
				return 0;
			return statusHeight(N->left) - statusHeight(N->right);
		}

		// #########################################################################################
		/**Function to find x co-ordinate of intersection pt of sweep line with another line given the two end points
		* @param l - linesegment
		* @param y - y co-ordinate i.e current position of horizontal sweep line 
		* @retval x co-ordinate of intersection pt
		*/
		double findx (lineSegment l, double y)
		{
		      // return ((y-l.endY)*((l.endX-l.startX)/(l.endY-l.startY)))+l.endX;
		      return ((y-l.ey)*((l.ex-l.sx)/(l.ey-l.sy)))+l.ex;
		}

		// #########################################################################################
		/**Function for inserting a new line segment in status AVL tree 
		* @param root - root status node
		* @param l - new line to be inserted
		* @param y - y co-ordinate i.e current position of sweep line
		* @return root pointer
		*/
		statusNode* statusInsert(statusNode* root, lineSegment l, double y)
		{
			// printf("Insertinggg (%f, %f), (%f, %f)\n", l.sx, l.sy, l.ex, l.ey);
			int *justInserted = &globalInsert;

			if (root == NULL)
			{
				*justInserted = 1;
				// cout << "INSERTING!!!" << endl;
				return statusNewNode(l);
			}

			double x = findx(l, y);
			double rx = findx(root->l, y);

			if (rx > x)
			{
				root->left = statusInsert(root->left, l, y);
				if (*justInserted == 1)
				{
					root->right = statusNewNode(root->l);
					root->l = l;
					*justInserted = 0;
				}
			}
			else if (rx < x)
			{
				root->right = statusInsert(root->right, l, y);
				if (*justInserted == 1)
				{
					root->left = statusNewNode(root->l);
					*justInserted = 0;
				}
			}	
			else
				return root;

			root->height = 1 + max(statusHeight(root->left), statusHeight(root->right));

			int balance = statusGetBalance(root);

			double rxl, rxr;
			if (root->left != NULL)
				rxl = findx(root->left->l, y);
			if (root->right != NULL)
				rxr = findx(root->right->l, y);

			// LL
			// if (balance > 1 && key < node->left->key)
			if (balance > 1 && x < rxl) // node->left->sp > sp
				return statusRightRotate(root);
			// RR
			// if (balance < -1 && key > node->right->key)
			if (balance < -1 && x > rxr)
				return statusLeftRotate(root);
			// LR
			// if (balance > 1 && key > node->left->key)
			if (balance > 1 && x > rxl)
			{
				root->left = statusLeftRotate(root->left);
				return statusRightRotate(root);
			}
			// RL
			// if (balance < -1 && key < node->right->key)
			if (balance < -1 && x < rxr)
			{
				root->right = statusRightRotate(root->right);
				return statusLeftRotate(root);
			}
			return root;
		}

		// #########################################################################################
		/**Function that finds the min value node in a given AVL tree 
		* (used by the deleteNode function)
		* @param node - node that is the root node of the subtree whose min value node we need to find
		* @return pointer to min value node
		*/
		statusNode * statusMinValueNode(statusNode* node)
		{
			statusNode* current = node;

			/* loop down to find the leftmost leaf */
			while (current->left != NULL)
				current = current->left;

			return current;
		}

		// #########################################################################################
		/**Funtion to delete the status node from AVL tree given the line segment 
		* @param root - root node of AVL tree
		* @param l - line segment to be deleted
		* @param y - current position of sweep line
		* @return pointer to root
		*/
		statusNode* statusDelete(statusNode* root, lineSegment l, double y)
		{
			if (root == NULL)
				return root;

			double x = findx(l, y);
			double rx = findx(root->l, y);

			if (rx > x)
				root->left = statusDelete(root->left, l, y);
			else if (rx < x)
				root->right = statusDelete(root->right, l, y);
			else
			{
				if((root->left == NULL) || (root->right == NULL))
				{
					statusNode *temp = root->left ? root->left : root->right;

					if (temp == NULL)
					{
						temp = root;
						root = NULL;
					}
					else
						*root = *temp;
					free(temp);
				}
				else
				{
					statusNode* temp = statusMinValueNode(root->right);
					root->l = temp->l;
					root->right = statusDelete(root->right, temp->l, y);
				}
			}

			if (root == NULL)
				return root;

			root->height = 1 + max(statusHeight(root->left), statusHeight(root->right));

			int balance = statusGetBalance(root);

			// LL
			if (balance > 1 && statusGetBalance(root->left) >= 0)
				return statusRightRotate(root);

			// LR
			if (balance > 1 && statusGetBalance(root->left) < 0)
			{
				root->left = statusLeftRotate(root->left);
				return statusRightRotate(root);
			}

			// RR
			if (balance < -1 && statusGetBalance(root->right) <= 0)
				return statusLeftRotate(root);

			// RL
			if (balance < -1 && statusGetBalance(root->right) > 0)
			{
				root->right = statusRightRotate(root->right);
				return statusLeftRotate(root);
			}
			return root;
		}

		// #########################################################################################
		/**Function to find the left neighbour of a given line segment from the status tree
		* @param root - root node pointer 
		* @param l - line segment whose left neighbour is to be found 
		* @param y - current position of sweep line 
		* @param lastRight - pointer that will point to the left neighbour of l
		*/
		void getLeftNeighbor(statusNode* root, lineSegment l, double y, struct lineSegment* lastRight)
		{
			if (root == NULL)
				return;
		  	if(root->height == 1)
		  	{
		  	  	if(lastRight->sx == -1)
		  	  	{
		  	  		if(findx(root->l,y-0.1) < findx(l,y-0.1))
		  	  		{
		  	  		  	*lastRight = root->l;
		  	  		}
		  	  	}
		  	  	return;
		  	}
		  	if ((findx(l,y-0.1) - 0.1) < findx(root->l,y-0.1))
		  	{
		  		getLeftNeighbor(root->left, l, y, lastRight);
		  	}
		  	else if ((findx(l,y-0.1) - 0.1) > findx(root->l, y-0.1))
		  	{
		  	  	*lastRight = root->l;
		  	  	getLeftNeighbor(root->right, l, y, lastRight);
		  	}
		}

		// #########################################################################################
		/**Function to find the right neighbour of a given line segment from the status tree
		* @param node - root node pointer 
		* @param l - line segment whose right neighbour is to be found 
		* @param y - current position of sweep line 
		* @param lastLeft - pointer that will point to the right neighbour of l
		*/
		void getRightNeighbor(statusNode* node, lineSegment l, double y, struct lineSegment* lastLeft)
		{
			if (node == NULL)
				return;
		  	if(node->height == 1)
		  	{
		    	if (lastLeft->sx == -1) 
		    	{
		    	  	if(findx(node->l,y-0.1) > findx(l,y-0.1))
		    	  	{
		    	  	  	*lastLeft = node->l;
		    	  	}
		    	}
		    	return;
		  	}
		  	if ((findx(l,y-0.1) + 0.1) < findx(node->l,y-0.1))
		  	{
		  	  	*lastLeft = node->l;
		  	  	getRightNeighbor(node->left, l, y, lastLeft);
		  	}
		  	else if ((findx(l,y-0.1) + 0.1) > findx(node->l, y-0.1))
		  	{
		  	  	getRightNeighbor(node->right, l, y, lastLeft);
		  	}
		}

		// #########################################################################################
		/**Function to find the left neighbour of a given line segment from the status tree
		* @param node - root node pointer 
		* @param xcor - x co-ordinate of current event point
		* @param ycor - y co-ordinate of current event point
		* @param lastRight - pointer to left neighbour of the event point
		* @param lastLeft - pointer to right neighbour of the event point
		*/
		void getNeighbors(statusNode* root, double x, double y, struct lineSegment* lastRight, struct lineSegment* lastLeft)
		{
			if (root == NULL)
				return;
		  	if(root->height == 1)
		  	{
		  	  	if(lastRight->sx == -1)
		  	  	{
		  	  	  	if(findx(root->l,y-0.1) <= x)
		  	  	  	{
		  	  	  	  	*lastRight = root->l;
		  	  	  	}
		  	  	}
		  	  	if (lastLeft->sx == -1) 
		  	  	{
		  	  	  	if(findx(root->l,y-0.1) > x)
		  	  	  	{
		  	  	  	  	*lastLeft = root->l;
		  	  	  	}
		  	  	}
		  	  	return;
		  	}

		    if (x < findx(root->l,y-0.1))
		    {
		      	*lastLeft = root->l;
		      	getNeighbors(root->left, x, y, lastRight, lastLeft);
		    }
		    else if (x > findx(root->l, y-0.1))
		    {
		      	*lastRight = root->l;
		      	if (root->right == NULL) 
		      	{
		      	  	return;
		      	}
		      	getNeighbors(root->right, x, y, lastRight, lastLeft);
		    }
		}

		// #########################################################################################
    	/**Function for doing pre order traversal of AVL tree*/
		void statusPreOrder(statusNode *root)
		{
			if(root != NULL)
			{
				// cout << "start = (" << root->sp.first << ", " << root->sp.second << ") ";
				// cout << "end = (" << root->ep.first << ", " << root->ep.second << ")\n";

				cout << "start = (" << root->l.sx << ", " << root->l.sy << ") ";
				cout << "end = (" << root->l.ex << ", " << root->l.ey << ")\n";

				statusPreOrder(root->left);
				statusPreOrder(root->right);
			}
		}
};



#endif












