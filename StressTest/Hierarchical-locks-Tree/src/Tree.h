#include"TreeNode.h"
#include<map>
#include<vector>
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <pthread.h>
#include<algorithm>
#include<unistd.h>//For usleep()
#include"lock.h"
#include "interval.h"
#define MINMAX_MODE 10

using namespace std;


extern IntervalCheck ICheck;
extern TreeNode** Array;
extern char CSSize;
TreeNode * NodeArray;
class Tree
{
	public:
	pthread_mutex_t rootLock;
	int size;
	TreeNode *head,*nextNode;
	int counter;//dfs counting
	vector<TreeNode *> NodeVector;
	map<int,TreeNode *> NodeRef;  //map of <key,address> for each tree node.
	map<int,string> PathRef;  //Path to  a nade.
  
  
	//Member functions************
	//Constructor
	Tree();
  
	//Preorder traversal
	void preorder(TreeNode *);

	//Insert new node into tree
	void insert(TreeNode *ptr);
  
	//Set pointer to a for next insertion operation
	void setNextNode();
  
	//Save direct address of each Node
	void setNodeRef(TreeNode *ptr);
  
	//Save path from head to node
	void setPathToNode(int n);
  
	//Print reference map
	void printMap();
  
	//Print path map
	void printPath();
  
	//Create Tree
	void CreateTree(int);

	//Concurrent function
	void DummyTask(int node);
  
	//Depth first search
	void DFS(TreeNode *ptr);

	//Updated parent intervals according to intervals of child nodes
	bool UpdateParent(TreeNode *parent, TreeNode *ptr);

	//New interval numbering algorithm
	void ModifiedDFS(TreeNode *ptr);

	TreeNode* findDominator(int min, int max, TreeNode* header);

	interval computeMinMax(int* NodeArray, TreeNode* ptr, int NoOfRequestedNodes);

	vector<TreeNode*> insertInOrder(vector<TreeNode*> subDominators, TreeNode* subDom);


	

	void NullOperation()
	{
		int itr;
		if(CSSize == 's')
			itr = 1000;
		else if(CSSize == 'm')
			itr = 10000;
		else if(CSSize == 'l')
			itr = 100000;
		
		for(int i=0;i<itr;i++)
		{		
			int x = 5,y;
			y = x;
		}
	}
  
  
};//end of class "Tree"





//constructor
Tree::Tree()
{
	head = NULL;
	nextNode = NULL;
	counter = 0;
  	size = 0;
}


//This function creates Tree with number of nodes as parameter
void Tree::CreateTree(int n)
{
	for(int i=1;i<=n;i++)
	{
		TreeNode *newNode=new TreeNode(i);
  
		this->insert(newNode);
	}
}

//This function saves path from root to perticular node: l=letf pointer r=right pointer
//stores path in string e.g. node 5=lr i.e "root->right->left" starts from end of string.
void Tree::setPathToNode(int n)
{
  
	for(int i=1;i<=n;i++)
	{
		string path="";
		int no=i;
		while(no>1)
		{
			if(no%2==0)
			{
				path=path+"l";
				no=no/2;
			}
			else
			{
				path=path+"r";
				no=no/2;
			}
      
		}
		//cout<<path<<"\n";
		PathRef.insert(std::pair<int,string>(i,path));
	}
}



//This function saves direct reference to nodes in tree
//e.g. node 5= #12h13
void Tree::setNodeRef(TreeNode *ptr)
{
  
	NodeRef.insert(std::pair<int,TreeNode *>(ptr->data,ptr));
	//  NodeVector.push_back(ptr);
}



//This function inserts new node in tree
void Tree::insert(TreeNode *ptr)
{
	// NodeVector.push_back(head);// save node address in vector
  
	Array[ptr->data]=ptr;
	setNodeRef(ptr);   //Save reference to each inserted node
	//setPathToNode(ptr); //Save path from head to inserted node
	//Insertion operation :
	if(nextNode == NULL)//if tree is empty
	{
		nextNode=ptr;
		head=ptr;
		// cout<<" "<<nextNode->data;
	}
	else if(nextNode->left == NULL)
	{
		ptr->parent = nextNode;
		nextNode->left = ptr;
		//cout<<" "<<nextNode->left->data;
	}
	else if(nextNode->right == NULL)
	{
		ptr->parent=nextNode;
		nextNode->right=ptr;
		//cout<<" "<<nextNode->right->data;
	}

	setNextNode();
    
	size++;
}



//This function updates pointer to a node for next insert operation
void Tree::setNextNode()
{
	if(nextNode->left != NULL && nextNode->right != NULL)
	{
		while(nextNode->parent && nextNode->parent->right == nextNode && nextNode != head)
		nextNode = nextNode->parent;

		if(nextNode!= head)
		{
		nextNode=nextNode->parent;
		nextNode=nextNode->right;
		while(nextNode->left)
		nextNode=nextNode->left;
		}
		else
		{
			while(nextNode->left)
			nextNode=nextNode->left;
		}
    
    
	}
}

//Prints references to each node
void Tree::printMap()
{
	extern TreeNode** Array;
	//for (std::map<int, TreeNode *>::iterator it=NodeRef.begin(); it!=NodeRef.end(); ++it)
	//  std::cout << it->first << " => " << it->second->data << '\n';

	for(int i=1;i<=10000;i++)
	{
	TreeNode *p = Array[i];
	cout<< p->data <<"\n";
	}
}

//Prints path in lr string form
void Tree::printPath()
{
  

	for (std::map<int, string>::iterator it=PathRef.begin(); it!=PathRef.end(); ++it)
	std::cout << it->first << " => " << it->second << '\n';

}


void Tree::preorder(TreeNode *p)
{
	if(p)
	{
		cout<<" "<<p->data<<" "<<p->preNumber<<" "<<p->postNumber<<"\n";
		if(p->left)		
		preorder(p->left);
		if(p->right)		
		preorder(p->right);
	}
}

void Tree::DFS(TreeNode* ptr)
{
	counter++;
	ptr->preNumber = counter;
	if(ptr->left != NULL)
	{
    
		DFS( ptr->left);
	}
	if(ptr->right != NULL)
	{
		DFS(ptr->right);  
	}
	counter++;
	ptr->postNumber = counter;
	
}


bool Tree::UpdateParent(TreeNode *par, TreeNode *ptr)
{
	if(par == NULL || (par->preNumber == ptr->preNumber && par->postNumber == ptr->postNumber))
	return true;

	if(par->preNumber == 0 && par->postNumber == 0)
	{
		par->preNumber = ptr->preNumber;
		par->postNumber = ptr->postNumber;	
	}
	else 
	{	
		if(par->preNumber > ptr->preNumber)
		par->preNumber = ptr->preNumber;
		if(par->postNumber < ptr->postNumber)
		par->postNumber = ptr->postNumber;	
		
	}
	
	if(par->parentUpdated == true)
	UpdateParent(par->parent, par);
	return true;

}


//Modified DFS algorithm for assigning intervals to data structure nodes.

void Tree::ModifiedDFS(TreeNode* ptr)
{

	// check if root is NULL
	if(ptr == NULL)
	return;

	//send updates to parent node if the node is already explored
	if(ptr->IsExplored)
	{	
		//send updates to parent node if the node is already explored
		bool ret = this->UpdateParent(ptr->parent, ptr);
		ptr->parentUpdated = (ptr->parentUpdated || ret);
		return;
	}

	//if node is a leaf node or node is a part of some cycle, consider it as leaf node and assign intervals
	if((ptr->left == NULL && ptr->right == NULL) || ptr->active == true)
	{
		counter++;
		ptr->preNumber = counter;
		ptr->postNumber = counter;
	}
	else
	{
		ptr->active = true;
		//call recursively modifiedDfS() function for all the child nodes. 
		//As we use binary tree, here for left and right pointers only.
		if(ptr->left !=NULL && (ptr->left)->IsExplored == false)
		ModifiedDFS(ptr->left);
		if(ptr->right !=NULL && (ptr->right)->IsExplored == false)
		ModifiedDFS(ptr->right);
	}

	ptr->IsExplored = true;
	ptr->active = false;
	bool ret = this->UpdateParent(ptr->parent, ptr);
	ptr->parentUpdated = (ptr->parentUpdated || ret);
	return;


}//end of modifiedDFS();



//This function used by ModifiedDFS() to forward the node interval information to parent


//this function is a dummy task that can get called parallelly.
void Tree::DummyTask(int node)
{
  	int threadID = node;//temporary porvision to produce thread ID;
	//node = (node % 30) +8000;



	int node2 = (rand()%size)+1; //second node to access.
	//node2 = node + 1;
	// cout<<" \n A am in dummy task working of node:"<<node;
  
	extern int caseParameter;
	extern int NoOfRequestedNode;
	extern int distribution;
	int accessType = 1;
  

	int NodeArray[NoOfRequestedNode];
	int NoOfLeafNodes = (size/2)+1;
	int SlotSize = NoOfLeafNodes/distribution;
	int SlotNo = rand()%distribution;
	//Coarse grain locking**********************************************case 1
	if(caseParameter == 1)
	{
		Lock(&rootLock);
		//usleep((rand()%1000)*1000);
		for(int i = 0;i < NoOfRequestedNode;i++)
		{
			NodeArray[i] = (NoOfLeafNodes-1)+SlotSize*SlotNo+((rand()%SlotSize)+1);
			if(NodeArray[i] >= 10001)
				NodeArray[i] = 10000;
		}
		NullOperation();
		UnLock(&rootLock);
		//cout<<" \n A am in dummy task working of node:"<<node;
  	}
  	//Fine grained locking through path traversal from root to node.************************* case 2
  	else if(caseParameter == 2)
  	{
  		std::map<int,string>::iterator it;
  		it = PathRef.find(node);
  		string path=it->second;
  		TreeNode *ptr = head;
  		for(int i = path.length()-1; i >=0; i--)
  		{
			if(path[i]== 'l')
			{
				ptr = ptr->left;
			}
			else
			{
				ptr = ptr->right;
			}
		}
		Lock(&ptr->Lock);
		//usleep((rand()%1000)*1000);
		usleep(1000);
		//cout<<" coarse \n";
		UnLock(&ptr->Lock);
  
	} 
	// Fine grained locking with direct reference to node ********************** case 3
  
	else if( caseParameter == 3)
	{
		//TreeNode *NodePtr = NodeRef.find(node)->second;
		//TreeNode *NodePtr = NodeVector[node-1];
		TreeNode *NodePtr = Array[node];
		Lock(&NodePtr->Lock);
		// usleep((rand()%1000)*1000);
		usleep(1000);
		//cout<<" Direct reference";
		UnLock(&NodePtr->Lock);

	}
	// Intention locking ***************************************case 4  

	else if( caseParameter == 4)
	{
		std::map<int,string>::iterator it;
		TreeNode *ptr[NoOfRequestedNode];
		int counter=0;//keeps track of *ptr array indexes
		for(int i = 0;i < NoOfRequestedNode;i++)
		{	
			NodeArray[i] = (NoOfLeafNodes-1)+SlotSize*SlotNo+((rand()%SlotSize)+1);
			if(NodeArray[i] >= size+1)
				NodeArray[i] = size;
		}
		// sort nodes in increasing order to lock  them in order to avoid deadlock
		sort(NodeArray, NodeArray + NoOfRequestedNode);
		
		
		for(int j = 0;j < NoOfRequestedNode;j++)
		{
			if(j==0 || j>0&&NodeArray[j] != NodeArray[j-1])
			{
				it = PathRef.find(NodeArray[j]);
				//if(it == PathRef.end())
				//cout<<"\n"<<ReqNode;
				string path=it->second;
				ptr[counter] = head;
		   
		     
				//if(accessType == 0)//read request
				//{
				for(int i = path.length()-1; i >=0; i--)
				{ 
					if(accessType == 0)
					ISLock(ptr[counter]);
					else
					IXLock(ptr[counter]);
					if(path[i]== 'l')
					{
						ptr[counter] = ptr[counter]->left;
					}
					else
					{
						ptr[counter] = ptr[counter]->right;
					}
				}
	
				if(accessType == 0)
				SLock(ptr[counter]);
				else
				XLock(ptr[counter]);
				counter++;
			}
		}
			//Call dummay null operation
			NullOperation();
			//usleep((rand()%1000)*1000);
			//usleep(1000);
			//cout<<" coarse \n";
		
		for(int i = 0;i < counter;i++)
		{
			//TreeNode *ptr = NodeArray[i];
			IUnLock(ptr[i]);
			ptr[i] = ptr[i]-> parent;
			while(ptr[i] != NULL)
			{ 
				IUnLock(ptr[i]);
				ptr[i] = ptr[i]->parent;
	      
			}
		}
	}
  
  // Multi-Dominator locking ***************************************case 5
	else if( caseParameter == 5)
	{//printf("\nhere tid %d\n",threadID);
		
		int min=0, max=0;
		for(int i = 0;i < NoOfRequestedNode;i++)
		{
			NodeArray[i] = (NoOfLeafNodes-1)+SlotSize*SlotNo+((rand()%SlotSize)+1);
			if(NodeArray[i] >= size+1)
				NodeArray[i] = size;
			//cout<<"\n nodes are"<<NodeArray[i]<<"\n"; 
			if(min == 0 || Array[NodeArray[i]]->preNumber < min)
			{
				min = Array[NodeArray[i]]->preNumber;
				//cout<<NodeArray[i]<<"\n";
			}
			if(max == 0 || Array[NodeArray[i]]->postNumber > max)
			{
				max = 	Array[NodeArray[i]]->postNumber;
			}
			
			
		}

		//cout <<"\n min and max \n"<< min <<" "<<max;
		


	//accessType = 1;


		TreeNode *ptr = findDominator(min, max, head);
		TreeNode *nodePtr1 = Array[node];
		TreeNode *nodePtr2 = Array[node2];


		vector<TreeNode*> subDominators;
		vector<TreeNode*> domChildren;
		int level = 0;

		subDominators.push_back(ptr);
		level++;

		while(level < 5){

			while(!subDominators.empty()){
				
				TreeNode* mother = subDominators[0];
				subDominators.erase(subDominators.begin());
				if(mother->left != NULL)
					domChildren.push_back(mother->left);
				if(mother->right != NULL)
					domChildren.push_back(mother->right);

			}

			int fineGrain = 0;
			while(!domChildren.empty()){

				interval req = computeMinMax(NodeArray, domChildren[0], NoOfRequestedNode);

				TreeNode* subDom;
				if(!(req.pre == -1 && req.post == -1)){
					subDom = findDominator(req.pre, req.post, domChildren[0]);
					subDominators =  insertInOrder(subDominators, subDom);
					fineGrain = 1;
				}

				domChildren.erase(domChildren.begin());
			}

			if(fineGrain == 0){
				//cout << "Finest level of locking is reached  @"<< level << endl;
				break;
			}
			level++;

		}

		for(int i=0;i<subDominators.size();i++){

			interval inv(subDominators[i]->preNumber,subDominators[i]->postNumber,accessType);
			// cout << "Trying to lock " << inv.pre << "-" << inv.post << " by thread " << threadID<< endl; 
			xy:	if(!ICheck.IsOverlap2(inv, 1, threadID))
			{	
				pthread_rwlock_wrlock(&subDominators[i]->rwlock);
				// cout << "\tLocked " << inv.pre << "-" << inv.post << " by thread " << threadID<< endl; 

			}
			else { 				
				// cout << "\tNo Locked " << inv.pre << "-" << inv.post << " by thread " << threadID<< endl; 
				usleep(10);goto xy;}
			}		
				//ICheck.Insert(inv, threadID);
				
				//Call dummay null operation
				NullOperation();
		
		for(int i=subDominators.size()-1;i>=0;i--){
					
			interval inv(subDominators[i]->preNumber,subDominators[i]->postNumber,accessType);
			// cout << "Trying to unlock " << inv.pre << "-" << inv.post << " by thread " << threadID<< endl; 

			ICheck.Delete2(inv, threadID);
			// cout << "\tUnlocked " << inv.pre << "-" << inv.post << " by thread " << threadID<< endl; 
			pthread_rwlock_unlock(&subDominators[i]->rwlock);
			
		}

   	
	}
  
    
	//Interval trees implementation ****************************case 6
   
	else if( caseParameter == 6)
	{ 
		//accessType = 0;
		TreeNode *NodePtr = Array[node];
		if(accessType == 0)
		RdLock(NodePtr);
		else
		WrLock(NodePtr);
		// usleep((rand()%1000)*1000);
		usleep(1000);
		RdWrUnLock(NodePtr);

	}
    	//cout<<" \n A am in dummy task **DONE** of node:"<<node;
}








TreeNode* Tree::findDominator(int min, int max, TreeNode* header){

		TreeNode *ptr = header;
		while( ptr->preNumber <= min && ptr->postNumber >= max)
		{
			if(ptr->left != NULL && ptr->left->preNumber <= min && ptr->left->postNumber >= max){   
				ptr = ptr->left;
      		}
  			else if ( ptr->right != NULL && ptr->right->preNumber <= min && ptr->right->postNumber >= max){  
				ptr = ptr->right;
			}
  			else{
				break;
			}
		}
		return ptr;
}






interval Tree::computeMinMax(int* NodeArray, TreeNode* ptr, int NoOfRequestedNodes){

			
	int min = 0;
	int max = 0;		

	for(int i = 0;i < NoOfRequestedNodes;i++){

		if((ptr->preNumber <= Array[NodeArray[i]]->preNumber  &&  Array[NodeArray[i]]->preNumber <= ptr->postNumber)   ||
				(ptr->preNumber <= Array[NodeArray[i]]->postNumber  &&  Array[NodeArray[i]]->postNumber <= ptr->postNumber)){ 

			if(min == 0 || Array[NodeArray[i]]->preNumber < min){
				min = Array[NodeArray[i]]->preNumber;
			}
			if(max == 0 || Array[NodeArray[i]]->postNumber > max){
				max = 	Array[NodeArray[i]]->postNumber;
			}
		}
	}

	if(min == 0 && max == 0)
		return interval(-1, -1, MINMAX_MODE);
	return interval(min, max, MINMAX_MODE);

}


vector<TreeNode*> Tree::insertInOrder(vector<TreeNode*> subDominators, TreeNode* subDom){

	int i;
	for(i=0;i<subDominators.size();i++){
		if(subDominators[i]->preNumber >= subDom->preNumber){
			break;
		}
	}
	subDominators.insert(subDominators.begin()+i, subDom);
	return subDominators;

}