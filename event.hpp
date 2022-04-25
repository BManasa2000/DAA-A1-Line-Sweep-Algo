#ifndef EVENT_HPP
#define EVENT_HPP

// #include <bits/stdc++.h>

#include "status.hpp"

#include <iostream>
#include <vector>
using namespace std;

/**Structure to represent a point*/
struct point
{
	double x,y;
};

/**A structure to represent a node in the event queue*/
struct eventNode
{
	public:
		// pair<double, double> p;
		point p;
		eventNode *left;
		eventNode *right;
		vector<lineSegment> U; // list of segments with p as upper endpoint
		vector<lineSegment> L; // list of segments with p as lower endpoint
		vector<lineSegment> C; // list of segments with p as interior point
		int height;
};

/**A class for inserting, deleting and popping the next event from an AVL tree containing the remaining events*/
class eventTree
{
public:
	// #########################################################################################
	/**Fucntion to compare 2 points
	* @param p1 - point 1
	* @param p2 - point 2
	* @retval 1 if p1 is greater than p2
	* @retval 0 otherwise
    */
	int eventCompare (point p1, point p2)
	{
		if ((p1.y > p2.y) || ((p1.y == p2.y) && (p1.x < p2.x)))
		{
			return 1;
		}
		return 0;
	}

	// #########################################################################################
    /**Function to find the height of the event node in the AVL tree*/
	int eventHeight(eventNode *N)
	{
		if (N == NULL)
			return 0;
		return N->height;
	}

	// #########################################################################################
    /**Function to find maximum of two given numbers*/
	int max(int a, int b)
	{
		return (a > b)? a : b;
	}

	// #########################################################################################
	/**Function to create a new event node.
	* @param p - event point
	* @param l - line segment
	* @param flag - indicated whether p is a start point, end point or interior point of l

	value of flag
	1 => upper endpoint
	2 => lower endpoint
	3 => interior point

	@return pointer to new event node
	*/
	eventNode* eventNewNode(point p, lineSegment l,int flag)
	{
		eventNode* node = new eventNode();
		node->p = p;
		
		if (flag == 1)
			node->U.push_back(l);
		else if (flag == 2)
			node->L.push_back(l);
		else
			node->C.push_back(l);
		
		node->left = NULL;
		node->right = NULL;
		node->height = 1;
		return(node);
	}

	// #########################################################################################
	/**Function for right rotating about a node in the AVL tree for rebalancing
    *@param y - event node about which to rotate
    */
	eventNode *eventRightRotate(eventNode *y)
	{
		eventNode *x = y->left;
		eventNode *T2 = x->right;

		// Perform rotation
		x->right = y;
		y->left = T2;

		// Update heights
		y->height = max(eventHeight(y->left), eventHeight(y->right)) + 1;
		x->height = max(eventHeight(x->left), eventHeight(x->right)) + 1;

		// Return new root
		return x;
	}

	// #########################################################################################
	/**Function for left rotating about a node in the AVL tree for rebalancing
    * @param x - event node about which to rotate 
    */
	eventNode *eventLeftRotate(eventNode *x)
	{
		eventNode *y = x->right;
		eventNode *T2 = y->left;

		// Perform rotation
		y->left = x;
		x->right = T2;

		// Update heights
		x->height = max(eventHeight(x->left), eventHeight(x->right)) + 1;
		y->height = max(eventHeight(y->left), eventHeight(y->right)) + 1;
		return y;
	}

	// #########################################################################################
    /**Function to get the balance factor of an event node*/
	int eventGetBalance(eventNode *N)
	{
		if (N == NULL)
			return 0;
		return eventHeight(N->left) - eventHeight(N->right);
	}

	// #########################################################################################
	/**Function for inserting a new event point 
    * @param node - root event node
    * @param p - point
    * @param l - line segment
    * @param flag - indicated whether p is a start point, end point or interior point of l
    value of flag
    1 => upper endpoint
    2 => lower endpoint
    3 => interior point

    @return root pointer
    */
	eventNode* eventInsert(eventNode* node, point p, lineSegment l, int flag)
	{
		if (node == NULL)
			return(eventNewNode(p, l, flag));

		if (p.y < node->p.y)
			node->left = eventInsert(node->left, p, l, flag);
		else if (p.y > node->p.y)
			node->right = eventInsert(node->right, p, l, flag);
		else if (p.x > node->p.x)
			node->left = eventInsert(node->left, p, l, flag);
		else if (p.x < node->p.x)
			node->right = eventInsert(node->right, p, l, flag);
		else
		{
			int f = 1;
			if (flag == 1) // event point p is an upper endpoint of l
			{
				for (int i = 0; i < node->U.size(); i++) // checking if line segment already exists in U(p)
				{
					lineSegment l1 = node->U[i];
					if (l1.sx == l.sx && l1.sy == l.sy && l1.ex == l.ex && l1.ey == l.ey)
					{
						f = 0; // line segment is aready present in this list so don't need to insert again
						break;
					}
				}
				if (f == 1) // line segment not already present in U(p)
				{
					node->U.push_back(l);
				}
			}
			else if (flag == 2) // event point p is a lower endpoint of l
			{
				for (int i = 0; i < node->L.size(); i++) // checking if line segment already exists in U(p)
				{
					lineSegment l1 = node->L[i];
					if (l1.sx == l.sx && l1.sy == l.sy && l1.ex == l.ex && l1.ey == l.ey)
					{
						f = 0; // line segment is aready present in this list so don't need to insert again
						break;
					}
				}
				if (f == 1) // line segment not already present in U(p)
				{
					node->L.push_back(l);
				}
			}
			else // p is an interior point of l
			{
				for (int i = 0; i < node->C.size(); i++) // checking if line segment already exists in C(p)
				{
					lineSegment l1 = node->C[i];
					if (l1.sx == l.sx && l1.sy == l.sy && l1.ex == l.ex && l1.ey == l.ey)
					{
						f = 0; // line segment is aready present in this list so don't need to insert again
						break;
					}
				}
				if (f == 1) // line segment not already present in U(p)
				{
					node->C.push_back(l);
				}
			}
			if (f == 0)
				return node;
		}

		node->height = 1 + max(eventHeight(node->left), eventHeight(node->right));

		int balance = eventGetBalance(node);
		// LL
		if (balance > 1 && (eventCompare (node->left->p, p) == 1)) // node->left->p > p
			return eventRightRotate(node);
		// RR
		if (balance < -1 && (eventCompare(p, node->right->p) == 1)) // p > node->right->p
			return eventLeftRotate(node);
		// LR
		if (balance > 1 && (eventCompare(p, node->left->p) == 1)) // p > node->left->p
		{
			node->left = eventLeftRotate(node->left);
			return eventRightRotate(node);
		}
		// RL
		if (balance < -1 && (eventCompare(node->right->p, p) == 1)) // node->right->p > p
		{
			node->right = eventRightRotate(node->right);
			return eventLeftRotate(node);
		}
		return node;
	}

	// #########################################################################################
	/**Function that finds the min value node in a given AVL tree 
    * (used by the deleteNode function)
    * @param node - node that is the root node of the subtree whose min value node we need to find
    * @return pointer to min value node
    */
	eventNode * eventMinValueNode(eventNode* node)
	{
		eventNode* current = node;

		/* loop down to find the leftmost leaf */
		while (current->left != NULL)
			current = current->left;

		return current;
	}

	// #########################################################################################
	/**Function that finds the max value node in a given AVL tree
    * (used for finding the next event node to pop)
    * @param node - node that is the root node of the subtree whose max value node we need to find
    * @return pointer to min value node
    */
	eventNode * eventMaxValueNode(eventNode* node)
	{
		eventNode* current = node;

		/* loop down to find the rightmost leaf */
		while (current->right != NULL)
			current = current->right;

		return current;
	}

	// #########################################################################################
	/**Funtion to delete the event node from AVL tree given the coordinates of the event point 
    * @param root - root node of AVL tree
    * @param p - event point to be deleted
    * @return pointer to root
    */
	eventNode* eventDelete(eventNode* root, point p)
	{
		if (root == NULL)
		{
			cout << "ROOT IS NULL in eventDelete\n";
			return root;
		}

		if (p.y < root->p.y)
			root->left = eventDelete(root->left, p);
		else if (p.y > root->p.y)
			root->right = eventDelete(root->right, p);
		else if (p.x > root->p.x)
			root->left = eventDelete(root->left, p);
		else if (p.x < root->p.x)
			root->right = eventDelete(root->right, p);
		else // (key == root->key) i.e (p == root->p)
		{
			if((root->left == NULL) || (root->right == NULL))
			{
				eventNode *temp = root->left ? root->left : root->right;

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
				/*eventNode* temp = eventMinValueNode(root->right);
				root->key = temp->key;
				root->right = eventDelete(root->right, temp->key);*/
				eventNode* temp = eventMinValueNode(root->right);
				root->p = temp->p;
				root->U = temp->U;
				root->L = temp->L;
				root->C = temp->C;
				root->right = eventDelete(root->right, temp->p);
			}
		}
		if (root == NULL)
		{
			return root;
		}

		root->height = 1 + max(eventHeight(root->left), eventHeight(root->right));

		int balance = eventGetBalance(root);

		// LL
		if (balance > 1 && eventGetBalance(root->left) >= 0)
			return eventRightRotate(root);

		// LR
		if (balance > 1 && eventGetBalance(root->left) < 0)
		{
			root->left = eventLeftRotate(root->left);
			return eventRightRotate(root);
		}

		// RR
		if (balance < -1 && eventGetBalance(root->right) <= 0)
			return eventLeftRotate(root);

		// RL
		if (balance < -1 && eventGetBalance(root->right) > 0)
		{
			root->right = eventRightRotate(root->right);
			return eventLeftRotate(root);
		}

		return root;
	}

	// #########################################################################################
    /**Function for doing pre order traversal of AVL tree*/
	void eventPreOrder(eventNode *root)
	{
		/*if (root == NULL)
		{
			cout << "NULL\n";
			return;
		}*/
		if(root != NULL)
		{
			cout << "(" << root->p.x << ", " << root->p.y << ")\n";

			if (!root->U.empty())
				cout << "Segments with p as upper endpoint:\n";
			// for (auto l: root->U)
			for (int i = 0; i < root->U.size(); ++i)
			{
				lineSegment l = root->U[i];
				printf("(%f, %f), (%f, %f)\n", l.sx, l.sy, l.ex, l.ey);
			}

			if (!root->C.empty())
				cout << "Segments with p as interior point:\n";
			// for (auto l: root->C)
			for (int i = 0; i < root->C.size(); ++i)
			{
				lineSegment l = root->C[i];
				printf("(%f, %f), (%f, %f)\n", l.sx, l.sy, l.ex, l.ey);
			}

			if (!root->L.empty())
				cout << "Segments with p as lower endpoint:\n";
			// for (auto l: root->L)
			for (int i = 0; i < root->L.size(); ++i)
			{
				lineSegment l = root->L[i];
				printf("(%f, %f), (%f, %f)\n", l.sx, l.sy, l.ex, l.ey);
			}

			cout << endl << endl;

			eventPreOrder(root->left);
			eventPreOrder(root->right);
		}
	}

	// #########################################################################################

};



#endif













