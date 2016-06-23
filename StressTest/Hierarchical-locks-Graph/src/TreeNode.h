#ifndef TREENODE_H
#define TREENODE_H
#include <pthread.h>
#include <vector>
using namespace std;

class TreeNode {
public:
	
	bool IsExplored;
	bool active;
	bool parentUpdated;
	int data;
	pthread_mutex_t Lock;
	pthread_rwlock_t rwlock;
	int IsIx;
	int refCounter;
	int preNumber;
	int postNumber;
	vector <TreeNode* >neighbour;
	class TreeNode *right;
	class TreeNode *left;
	class TreeNode *parent;
	vector <TreeNode* >parents;
	TreeNode(int);
};

TreeNode::TreeNode(int key)
{
  int rc = pthread_rwlock_init(&rwlock, NULL);
  
	IsExplored = false;
	bool active = false;
	parentUpdated = false;
	data=key;
  	IsIx=0;
  	refCounter=0;
}




#endif
