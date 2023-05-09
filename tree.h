#ifndef _tree_h_
#define _tree_h_

#include <iostream>
#include <string>
#include <queue>

using namespace std;

struct Node {
	int numOfKeys;
	Node* parent;
	string* keys;
	Node** next;
	Node(int numOfPointers);
	~Node();
};

class Tree {
	int m, rootMax, nodeMin;
	Node* root;
public:
	Tree(int dimension);
	Tree(const Tree& t) = delete;

	void insert(string s);
	void overflow(Node*& tmp, int i);
	void fracture(Node*& nodeStart, string s);
	bool isInTree(string s);
	bool insertBool(string s);
	int howManyLesserThen(string s);
	void remove(string s);
	bool removeBool(string s);

	friend bool overflowRightPossible(Node*& tmp, Tree& t);
	friend bool overflowLeftPossible(Node*& tmp, Tree& t);
	friend bool isRoot(Node*& n, Tree& t);
	friend void breakRoot(Node* root, string s, Node*& carry, int& indEndOfWhile, Tree& t);
	friend string breakNode(Node*& lNode, Node*& rNode, string s, Node*& carry, int& indCarry,
		int& indEndOfWhile, int first, int second, int third, Tree& t, int breakRight);
	friend Node* findStringInTree(Tree& t, string s, int& indexOfStringInNode);
	friend void removeFromLeaf(Node*& tmp, Tree& t, string s);
	friend bool borrowRightPossible(Node* tmp, Tree& t);
	friend bool borrowLeftPossible(Node* tmp, Tree& t);
	friend bool borrowRRPosible(Node* tmp, Tree& t);
	friend bool borrowLLPosible(Node* tmp, Tree& t);
	friend Node* compression(Node* tmp, Tree& t, int& indEndOfLoop);
	friend Node* compress3in2(Node* firstNode, Node* secondNode, Node* thirdNode, Tree& t, int& indEndOfLoop);
	friend Node* compress2in1(Node* lNode, Node* rNode, Tree& t, int& indEndOfLoop);

	friend ostream& operator<<(ostream& os, const Tree& t);

	Tree& operator=(const Tree& t) = delete;

	~Tree();
};

#endif