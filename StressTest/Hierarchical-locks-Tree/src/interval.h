

#ifndef SB7_INTERVAL_H_
#define SB7_INTERVAL_H_

#include <vector>
#include <stdlib.h>
using namespace std;
#define SIZE 20
class interval{

public: 
	int pre, post, mode;
	interval( int a, int b, int m){
	pre = a; post = b; mode = m;
	}
};

class multiInterval{

	public:
		vector<interval> lockedIntervals;
};

//**************************************************************************************************************
class IntervalCheck{
public:
interval* Array[SIZE];
multiInterval Array2[SIZE];
pthread_rwlock_t ArrayLock[SIZE];

IntervalCheck()
{
	for(int i = 0;i<SIZE; i++)
	{
		
		Array[i] = NULL;
		pthread_rwlock_t ArrayLock[i];
	}



}

bool IsOverlap(interval *inv, int m, int threadID)
{
//cout<<"m=1";
pthread_rwlock_wrlock(&ArrayLock[0]);
 if(m == 1)
{	
for(int i=0; i< SIZE; i++)
	{
		//pthread_rwlock_wrlock(&ArrayLock[i]);
		//	printf("read lock %d %d",i,threadID);
		if(Array[i] != NULL)
		{		
			
			if((Array[i]->pre >= inv->pre && Array[i]->post <= inv->post) ||
			(Array[i]->pre <= inv->pre && Array[i]->post >= inv->post))
			{
			//for(int j = i; j>=0; j--)
			pthread_rwlock_unlock(&ArrayLock[0]);
			return true;
			}
		}
		
		//pthread_rwlock_unlock(&ArrayLock[i]);
	}

//pthread_rwlock_unlock(&ArrayLock[threadID]);
Insert(inv, threadID);
pthread_rwlock_unlock(&ArrayLock[0]);
//printf("inserted by %d",threadID);
//for(int j = SIZE-1; j>=0; j--)
//{
//if(j!= threadID)
//pthread_rwlock_unlock(&ArrayLock[j]);
//}
return false;

} 
}


void print(){

	for(int i=0; i< SIZE; i++){
		if(Array2[i].lockedIntervals.size() > 0){
			cout << "\t" << i <<" size : " << Array2[i].lockedIntervals.size() << endl;
			for(int j=0;j<Array2[i].lockedIntervals.size();j++){
				cout << "\t\t" << Array2[i].lockedIntervals[j].pre << "  " << Array2[i].lockedIntervals[j].post << endl;
			}
		}
	}
}


bool IsOverlap2(interval inv, int m, int threadID)
{
//cout<<"m=1";
pthread_rwlock_wrlock(&ArrayLock[0]);
// cout << "----Before-----" << endl;
// print();
 if(m == 1)
{	
for(int i=0; i< SIZE; i++)
	{
		//pthread_rwlock_wrlock(&ArrayLock[i]);
		//	printf("read lock %d %d",i,threadID);
		if(i != threadID )
		{		
			for(int j=0;j<Array2[i].lockedIntervals.size();j++){
				if((Array2[i].lockedIntervals[j].pre >= inv.pre && Array2[i].lockedIntervals[j].post <= inv.post) ||
				(Array2[i].lockedIntervals[j].pre <= inv.pre && Array2[i].lockedIntervals[j].post >= inv.post)) {
			
				//for(int j = i; j>=0; j--)
					pthread_rwlock_unlock(&ArrayLock[0]);
					return true;
				}
			}
		}
		
		//pthread_rwlock_unlock(&ArrayLock[i]);
	}
//pthread_rwlock_unlock(&ArrayLock[threadID]);
Insert2(inv, threadID);
// cout << "----After-----" << endl;
// print();
pthread_rwlock_unlock(&ArrayLock[0]);
//printf("inserted by %d",threadID);
//for(int j = SIZE-1; j>=0; j--)
//{
//if(j!= threadID)
//pthread_rwlock_unlock(&ArrayLock[j]);
//}
return false;

} 
}


void Insert2(interval inv, int index){

	Array2[index].lockedIntervals.push_back(inv);
}

void Insert(interval *inv, int index)
{

	//pthread_rwlock_wrlock(&ArrayLock[index]);	
	Array[index] = inv;
	//pthread_rwlock_unlock(&ArrayLock[index]);
	
}


void Delete2(interval inv, int index){

	pthread_rwlock_wrlock(&ArrayLock[0]);
	int i=0;
	int in = 0;
		// cout << "----Before-----" << endl;
		// print();

		for(i=0;i<Array2[index].lockedIntervals.size();i++){

			if(inv.pre == Array2[index].lockedIntervals[i].pre && 
				inv.post == Array2[index].lockedIntervals[i].post){
				in = 1;
				break;
			}
		}

		if(in == 1)	
			Array2[index].lockedIntervals.erase(Array2[index].lockedIntervals.begin() + i);

		// cout << "----After-----" << endl;
		// print();
		pthread_rwlock_unlock(&ArrayLock[0]);
	
}



void Delete(int index)
{//index=0;
	pthread_rwlock_wrlock(&ArrayLock[0]);
	
	Array[index] = NULL;
	pthread_rwlock_unlock(&ArrayLock[0]);
	

}

};

#endif
