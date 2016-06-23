#include"TreeNode.h"
#include<map>
#include <unordered_map>
#include<math.h>
#include<vector>
#include<set>
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <pthread.h>
#include<algorithm>
#include<unistd.h>//For usleep()
#include"lock.h"
#include "interval.h"
using namespace std;


extern IntervalCheck ICheck;
extern TreeNode** Array;
extern char CSSize;
int leafNodes;
class Tree
{
	public:
	pthread_mutex_t rootLock;
	int size;
	TreeNode *head,*nextNode;
	int counter;//dfs counting
	map<int,string> PathRef;  //Path to  a nade.
  	map<int,TreeNode *> NodeRef;  //map of <key,address> for each tree node.
  
	//Member functions************
	//Constructor
	Tree();
  
	//Preorder traversal
	void preorder(TreeNode *);
	
	//Discover all parents of a node
	void findAllParents(TreeNode *ptr, vector<int>& V,int);

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

	
	//create directed graph
	void CreateGraph(int);

	//Null Operation, which gets executedd by each thread
	void NullOperation();

	//Structural updates like Insertion and deletion
	void StructuralUpdate(int ThreadID);

	//Find set of affected parents ranges because of the structural updation
	void FindAffectedParents(TreeNode *ptr, std::unordered_map <int,bool>& M, int low, int high,int d);
	
  	TreeNode* findDominator(int min, int max, TreeNode* header);

	interval computeMinMax(int* NodeArray, TreeNode* ptr, int NoOfRequestedNodes);

	vector<TreeNode*> insertInOrder(vector<TreeNode*> subDominators, TreeNode* subDom);

  
};//end of class "Tree"


//************************************************************************************
//This function is used as a task to be executed after locking all desired nodes 
//************************************************************************************
void Tree::NullOperation()
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

//************************************************************************************
//constructor
//************************************************************************************
Tree::Tree()
{
	head = NULL;
	nextNode = NULL;
	counter = 0;
  	size = 0;
}

//************************************************************************************
//This function creates Tree with number of nodes as parameter
//************************************************************************************
void Tree::CreateTree(int n)
{
	for(int i=1;i<=n;i++)
	{
		TreeNode *newNode=new TreeNode(i);
  
		this->insert(newNode);
	}
}



//************************************************************************************
//This function saves path from root to perticular node: l=letf pointer r=right pointer
//stores path in string e.g. node 5=lr i.e "root->right->left" starts from end of string.
//************************************************************************************
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


//************************************************************************************
//This function saves direct reference to nodes in tree
//e.g. node 5= #12h13
//************************************************************************************
void Tree::setNodeRef(TreeNode *ptr)
{
  
	NodeRef.insert(std::pair<int,TreeNode *>(ptr->data,ptr));
	//  NodeVector.push_back(ptr);
}


//************************************************************************************
//This function inserts new node in tree
//************************************************************************************
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


//************************************************************************************
//This function updates pointer to a node for next insert operation
//************************************************************************************
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


//************************************************************************************
//Prints references to each node
//************************************************************************************
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


//************************************************************************************
//Prints path in lr string form
//************************************************************************************
void Tree::printPath()
{
  

	for (std::map<int, string>::iterator it=PathRef.begin(); it!=PathRef.end(); ++it)
	std::cout << it->first << " => " << it->second << '\n';

}


//************************************************************************************
//Prints preorder sequence for the binary tree
//************************************************************************************

void Tree::preorder(TreeNode *p)
{
	if(p)
	{
		cout<<" "<<p->data<<" "<<p->preNumber<<" "<<p->postNumber<<"\n";
		for(int i=0;i<p->neighbour.size();i++)		
		{if(p->neighbour[i])		
		preorder(p->neighbour[i]);}
		//if(p->right)		
		//preorder(p->right);
	}
}


//**************************************************************************************
//This function finds all parents  of vertex pointed by *ptr and stores them in vector V
//**************************************************************************************
void Tree::findAllParents(TreeNode *p, vector<int>& V, int d)
{
	if(p == NULL)
	return;

	if((p->parents).size() > 0)
	{	
		if ( std::find(V.begin(), V.end(), p->data) == V.end() && p->data != d)				
		{
		//cout<<"Pushing \n"<<p->data;
		V.push_back(p->data);
		}		
		for(int i=0;i<p->parents.size();i++)		
		{
			if(p->parents[i])
			{	
				int x = (p->parents[i])->data;	
			if ( std::find(V.begin(), V.end(), x) == V.end() )
			findAllParents(p->parents[i],V, d);}
		}
		//if(p->right)		
		//preorder(p->right);
	}
}


//***********************************************************************************
//This function finds all parents ranges affected because of the structural updation
//***********************************************************************************
void Tree::FindAffectedParents(TreeNode *p, std::unordered_map <int,bool>& M, int low, int high, int d)
{
	if(p == NULL || (p->preNumber < low && p->postNumber >high))
	return;

	if((p->parents).size() > 0)
	{	
		if( M.find(p->data) == M.end() && !(p->preNumber < low && p->postNumber >high) && p->data!= d)				
		{
		 M[p->data] = true;
		
		}		
		for(int i=0;i<p->parents.size();i++)		
		{
			if(p->parents[i])
			{	
				int x = (p->parents[i])->data;	
			if ( M.find(p->data) == M.end())
			FindAffectedParents(p->parents[i],M, low, high,d);}
		}
		
	}

}


//***********************************************************************************
//Function that inserts or deletes the edges from the data structure
//***********************************************************************************
void Tree::StructuralUpdate(int threadID)
{

	extern int caseParameter;
	extern int NoOfRequestedNode;
	extern int distribution;
	// if type is 0- insertion if its 1- deletion	
	int type = rand()%2;
	TreeNode *temp;
	int NoOfLeafNodes = leafNodes;
	
	int SlotSize = NoOfLeafNodes/distribution;
	//vector collects all parents of a node that will get locked in intention mode	
	vector<int>VParent;
	int SlotNo = rand()%distribution;
	
	// NTBL is a pointer to a Node To Be Lock in either insertion or deletion opetration  
	TreeNode * NTBL = NULL;
	//Choose a source node to add new node
	int source = (size - NoOfLeafNodes)+SlotSize*SlotNo+((rand()%SlotSize));
	source = 1 + source%(size-1);
	
	if(type == 0)//insertion
	{
		temp=new TreeNode(size + (rand()%200));
		NTBL = Array[source];
	}
	else if(type == 1)//deletion
	{
		if(Array[source]->parents.size()!= 0)
		{		
			int nthpar = rand()% Array[source]->parents.size();
			TreeNode* par = Array[source]->parents[nthpar];
			NTBL = par;
		}
		else
			NTBL = Array[source];	
	}

	
	//*********domLock Structural updates locking phase *****************
	if(caseParameter == 5)
	{
		interval *inv = new interval(NTBL->preNumber,NTBL->postNumber,1);
		xy:	if(!ICheck.IsOverlap(inv, 1, threadID))
		{	
			pthread_rwlock_wrlock(&NTBL->rwlock);
	
			//ICheck.Insert(inv, threadID);
		}
		else { usleep(10);goto xy;}

	}
	else if(caseParameter ==4)//*********IL locking phase *******
	{
		findAllParents(NTBL,VParent,NTBL->data);
		
		VParent.push_back(NTBL->data);
		
		sort(VParent.begin(), VParent.begin()+ VParent.size());//sorting for deadlock avoidance.

		for(int x =0; x<VParent.size();x++)
		{
			if(Array[VParent[x]] == NTBL)
				XLock(NTBL);
			else				
			IXLock(Array[VParent[x]]);
		}
		
		
	}


	if(type == 0)//insertion of a new node.
	{
		NTBL->neighbour.push_back(temp);
		temp->parents.push_back(NTBL);
	}
	else if(type == 1)//deletion of a node from parent.
	{	
		TreeNode *v = Array[source];
		v->parents.erase(std::remove(v->parents.begin(), v->parents.end(), NTBL), v->parents.end());
		NTBL->neighbour.erase(std::remove(NTBL->neighbour.begin(), NTBL->neighbour.end(), v), NTBL->neighbour.end());
	}
	
	//Unlock phase

	if(caseParameter == 5) //DomLock unlock
	{
		ICheck.Delete(threadID);
		pthread_rwlock_unlock(&NTBL->rwlock);

	}
	else if(caseParameter == 4) //Intention unlock
	{
		//IUnLock(Array[source]);
		for(int x =0; x<VParent.size();x++)
		{
		IUnLock(Array[VParent[x]]);
		}
	}

	
}//End of StructuralUpdate()

//************************************************************************************
//DFS algorithm for interval numbering 
//************************************************************************************
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
	
}//End of DFS

//************************************************************************************
//Function ModifiedDFS() uses this function to update the parents interval value
//************************************************************************************
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
		if(par->preNumber > ptr->preNumber && ptr->preNumber!=0)
		par->preNumber = ptr->preNumber;
		if(par->postNumber < ptr->postNumber &&ptr->postNumber!=0)
		par->postNumber = ptr->postNumber;	
		
	}
	//Special case for cycles
	if(par->parentUpdated == true)
	for(int i=0;i<par->parents.size();i++)
	UpdateParent(par->parents[i], par);
	return true;

}//EndOf UpdateParents()

//************************************************************************************
//Modified DFS algorithm for assigning intervals to data structure nodes.
//************************************************************************************
void Tree::ModifiedDFS(TreeNode* ptr)
{
bool ret = false;
	// check if root is NULL
	if(ptr == NULL)
	return;

	//send updates to parent node if the node is already explored
	if(ptr->IsExplored)
	{	//cout<<ptr->data<<" is already done"<<endl;
		//send updates to parent node if the node is already explored
		for(int i=0;i<ptr->parents.size();i++)
		ret = UpdateParent(ptr->parents[i], ptr);
		ptr->parentUpdated = (ptr->parentUpdated || ret);
		return;
	}

	//if node is a leaf node or node is a part of some cycle, consider it as leaf node and assign intervals
	if(ptr->neighbour.size() == 0  || ptr->active == true)
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
		for(int i=0;i<ptr->neighbour.size();i++)
		{		
			//if(ptr->neighbour[i] !=NULL )
			ModifiedDFS(ptr->neighbour[i]);
			
		}
	}

	ptr->IsExplored = true;
	ptr->active = false;
	
	//propagate interval information to all parents	
	for(int i=0;i<ptr->parents.size();i++)
	ret = UpdateParent(ptr->parents[i], ptr);
	ptr->parentUpdated = (ptr->parentUpdated || ret);
	return;


}//end of modifiedDFS();



//************************************************************************************
//This function generates random directed-graph
//************************************************************************************
void Tree::CreateGraph( int n)
{	size = n;
	int MAX_NODES = n;
	int ht = log2(MAX_NODES);

	//random height between "log(n)/2" to "logn + log(n)/2"
	int height = 1+(ht/2) + rand()%ht;
	//int height = ht;	
	int BHT = height;
	//cout<<"MAX HEIGHT"<<height<<endl;
	int base = 0;
	
	//Array stores the max number of nodes present per level
	int NodesAtHeight[height];
	
	int sum = 1;
	n=n-1;
	//for loop specifies the number of nodes present per level
	NodesAtHeight[0] = 1;	
	int newht = 0;
	//cout<<"Height 0:"<<NodesAtHeight[0]<<endl;
	int height_checker=0;	
	int rem = n-1;
	for(int i=1; i<height; i++)
	{
		if(i==height-1)
		{
			NodesAtHeight[i] = rem;
			newht = i;
			//cout<<"Height "<<i<<":"<<NodesAtHeight[i]<<endl;
		}
		if(rem>0)
		{
			int exp = 1 + 2 * NodesAtHeight[i-1] + rand()%(5);
			NodesAtHeight[i] = min(rem,exp);
			rem = rem - NodesAtHeight[i];
			newht = i;
			//cout<<"Height "<<i<<":"<<NodesAtHeight[i]<<endl;
			if(rem == 0)
			break;
		}
	}


	leafNodes = NodesAtHeight[newht];
	height = newht + 1;
	//cout<<"leaf nodes"<<leafNodes<<endl;




	


	
	
	/**Generate the graph vertices, store their addresses to Array of vertiecs
	assign vertex index from 1 to N****/
	for(int i = 1; i<=MAX_NODES;i++)
	{
		TreeNode *temp=new TreeNode(i);
		Array[temp->data] = temp;
	}
	head = Array[1];

	sum = 1;//offset
	int backupsum = sum;
	int vertex_u = 1;
	

	for(int i = 2; i < 2+NodesAtHeight[1]; i++)
	{
	//	cout<<"Attach "<<1<<" to "<<i<<endl;
	
		//Attach u->v i.e. Array[1]->Array[i]
		Array[1]->neighbour.push_back(Array[i]);
		Array[i]->parents.push_back(Array[1]);
	}
	vertex_u = 2;	
	for(int i = 1; i < height-1; i++)
	{	backupsum = sum;
		sum+=NodesAtHeight[i];
		for(int j = 0; j< NodesAtHeight[i]; j++)
		{
			int outdegree = 1+ min(i*rand()%10,rand() % NodesAtHeight[i+1]);
			while(outdegree > 0)
			{
				int vertex_v = sum + 1 + rand() % NodesAtHeight[i+1];
				//cout<<"Attach "<<vertex_u<<" to "<<vertex_v<<endl;				
				//Attach u -> v 
				//connect disconnected node to the data structure
				if(Array[vertex_u]->parents.size()==0)
				{
					int temp_par = 1 + rand() % vertex_u;
					//cout<<"Attach "<<temp_par<<" to "<<vertex_u<<endl;
					Array[temp_par]->neighbour.push_back(Array[vertex_u]);
					Array[vertex_u]->parents.push_back(Array[temp_par]);
				}				
				Array[vertex_u]->neighbour.push_back(Array[vertex_v]);
				Array[vertex_v]->parents.push_back(Array[vertex_u]);				
				
				outdegree--;
			}
			vertex_u++;
		}
		

	}

	
	/*for(int j = 0; j< NodesAtHeight[height -1] - 1 ; j++)
	{
		int vertex = sum + 1 + j;
		if(Array[vertex]->parents.size()==0)
		{
			int temp_par = backupsum +1 + rand() % NodesAtHeight[height -2];
			//cout<<"Attach "<<temp_par<<" to "<<vertex_u<<endl;
			Array[temp_par]->neighbour.push_back(Array[vertex]);
			Array[vertex]->parents.push_back(Array[temp_par]);		
		}
	}*/
	
//cout<<"GRAPH GENERATED head vector "<< head->neighbour[1]->neighbour.size();
	
}//end of CreateGraph()





//************************************************************************************
//this function is a dummy task that can get called parallelly.
//************************************************************************************
void Tree::DummyTask(int node)
{
  	int threadID = node;//temporary provision to produce thread ID;
	//node = (node % 30) +8000;



	int node2 = (rand()%size)+1; //second node to access.
	//node2 = node + 1;
	//cout<<" \n A am in dummy task working of node:"<<node;
  
	extern int caseParameter;
	extern int NoOfRequestedNode;
	extern int distribution;

	int accessType = 1;
  

	int NodeArray[NoOfRequestedNode];
	//int NoOfLeafNodes = (size/2)+1;
	int NoOfLeafNodes = leafNodes;

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
		
		std::vector <int> VParent;
		std::vector <int> ReqNode;
		int counter=0;//keeps track of *ptr array indexes
		
		for(int i = 0;i < NoOfRequestedNode;i++)
		{	
			NodeArray[i] = (size - NoOfLeafNodes)+SlotSize*SlotNo+((rand()%SlotSize));
			if(NodeArray[i] >= size+1)
				NodeArray[i] = size-5;
			if(NodeArray[i] <=0)
				NodeArray[i] = 5;
		}
		
		// sort nodes in increasing order to lock  them in order to avoid deadlock
		sort(NodeArray, NodeArray + NoOfRequestedNode);
		
		
		//Changed this portion of code to find all parents of a node.
		for(int j = 0;j < NoOfRequestedNode;j++)
		{
			if(j==0 || j>0 && NodeArray[j] != NodeArray[j-1])//to remove duplicate chosen random
			{
				ReqNode.push_back(NodeArray[j]);
				findAllParents(Array[NodeArray[j]],VParent,NodeArray[j]);
				

			}
		}

		sort(VParent.begin(), VParent.begin()+ VParent.size());
//cout<<"Parents\n";
//for(int g =0;g<VParent.size();g++)
//{
//cout<<VParent[g]<<endl;

//}

		//cout<<"Intention locking start\n";
		//Lock all parents in IX/IS mode
		for(int x =0; x<VParent.size();x++)
		{
			if(accessType == 0)
			ISLock(Array[VParent[x]]);
			else
			IXLock(Array[VParent[x]]);
		}
				
		//cout<<"Intention locking finish by: "<<threadID<<endl;
//cout<<"Actual nodes\n";		
//for(int g =0;g<ReqNode.size();g++)
//{
//cout<<ReqNode[g]<<endl;

//}
		for(int j = 0;j < ReqNode.size();j++)
		{
			
			if(accessType == 0){
			SLock(Array[ReqNode[j]]);}
			else{
			//printf("Trying %d for %d \n",threadID,ReqNode[j]);
			XLock(Array[ReqNode[j]]);
			//printf("locking done  %d for %d \n",threadID,ReqNode[j]);
			}
			
		}

		//	cout<<"Main locking finish\n";
		
		//Call dummay null operation
		NullOperation();
			
		//usleep((rand()%1000)*1000);
		//usleep(1000);
		//cout<<" coarse \n";
		

		//Unlock actual requested nodes
		for(int i = 0;i < ReqNode.size();i++)
		{
			//TreeNode *ptr = NodeArray[i];
			//printf("Unlocking %d for %d \n",threadID,ReqNode[i]);;
			IUnLock(Array[ReqNode[i]]);
			
		}
		
		//Unlocl extra nodes locked in intention mode
		for(int j = VParent.size() -1; j >=0; j--)
		{
			
			if(accessType == 0)
			IUnLock(Array[VParent[j]]);
			else
			IUnLock(Array[VParent[j]]);
			
		}

		//cout<<"Unlocking done by "<<threadID<<endl;
			
		
	}
  
  // Dominator locking ***************************************case 5
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
		


		TreeNode *ptr = findDominator(min, max, head);
		TreeNode *nodePtr1 = Array[node];
		TreeNode *nodePtr2 = Array[node2];


		vector<TreeNode*> subDominators;
		vector<TreeNode*> domChildren;
		int level = 0;

		subDominators.push_back(ptr);
		level++;

		while(level < 2){

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
				///cout << "Finest level of locking is reached " << endl;
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
}//end of DummyTask()





TreeNode* Tree::findDominator(int min, int max, TreeNode* header){

		TreeNode *ptr = header;
      	for(int i=0;i<ptr->neighbour.size();i++)
			{
				if(ptr->neighbour[i] != NULL && ptr->neighbour[i]->preNumber <= min && ptr->neighbour[i]->postNumber >= max)
				{   
					ptr = ptr->neighbour[i];
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







