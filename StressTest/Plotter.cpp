#include<iostream>
#include<fstream>
#include<math.h>
using namespace std;

int main()
{
	//********************************************************************************
	//Plots for tree data structure
	//********************************************************************************

	fstream DomFile("./Hierarchical-locks-Tree/output/DomLock.txt");
	fstream ILFile("./Hierarchical-locks-Tree/output/Intention.txt");
	ofstream TreeOutput("Figure_5_a_c_e.txt");
	ofstream Distribution("Figure_6_a-f");

	TreeOutput<<"First column represent execution time(in second) with DomLock and \n Second column represent execution time(in second) for Intention locks\n";

	TreeOutput<<"X-axis is the number of locked nodes from 2 to 256 in power of 2's\n";
	TreeOutput<<"Y-axis is the execution time in second\n";

	Distribution<<"X-axis is the value of distribution and Y-axis is execution time in second\n";
	//Iterate three times to compute values for small medium and large critical section
	for(int i=0;i<3;i++)
	{	
		TreeOutput<<"Plots for ";
		
		if(i==0)
		TreeOutput<<"small critical section";
		if(i==1)
		TreeOutput<<"medium critical section";
		if(i==2)
		TreeOutput<<"large critical section";

		TreeOutput<<endl;	
		//iterate for different number of nodes to be locked from 1 to 128
		for(int j=0;j<8;j++)
		{	

			if(i==1)
			{
				Distribution<<"Plot for distribution with "<<pow(2,j+1)<<" nodes"<<endl<<"DomLock -- Intention Locks \n";
			}
			//iterate for different distribution from 1 to 16
			double DomAvg = 0,ILAvg = 0;
			for(int k=0;k<5;k++)
			{
				double DomVal,ILVal;
				DomFile>>DomVal;
				ILFile>>ILVal;
				if(i==1)
				{
					Distribution<<DomVal<<" "<<ILVal<<"\n";
				}

				DomAvg = DomAvg + DomVal;
				ILAvg = ILAvg + ILVal;

			}	
			TreeOutput<<(double)DomAvg/5<<" "<<(double)ILAvg/5<<"\n";
			if(i==1)
			Distribution<<endl;
		}

		TreeOutput<<endl;
	}
	TreeOutput.close();
	DomFile.close();
	ILFile.close();

	//********************************************************************************
	//Plots for directed graphs
	//********************************************************************************
	
	fstream DomFileGraph("./Hierarchical-locks-Graph/output/DomLock.txt");
	fstream ILFileGraph("./Hierarchical-locks-Graph/output/Intention.txt");
	ofstream GraphOutput("Figure_5_b_d_f.txt");

	GraphOutput<<"First column represent execution time with DomLock and \n Second column represent execution time for Intention locks\n";

	GraphOutput<<"X-axis is the number of locked nodes from 2 to 64 in power of 2's\n";
	GraphOutput<<"Y-axis is the execution time in second\n";
	//Iterate 3 times to compute values for small medium and large critical section
	for(int i=0;i<3;i++)
	{	
		GraphOutput<<"Plots for ";

		if(i==0)
		GraphOutput<<"small critical section";
		if(i==1)
		GraphOutput<<"medium critical section";
		if(i==2)
		GraphOutput<<"large critical section";

		GraphOutput<<endl;	
		//iterate for different number of nodes to be locked from 1 to 128
		for(int j=0;j<6;j++)
		{
			//iterate for different distribution from 1 to 16
			double DomAvg = 0,ILAvg = 0;
			for(int k=0;k<5;k++)
			{
				double DomVal,ILVal;
				DomFileGraph>>DomVal;
				ILFileGraph>>ILVal;
				DomAvg = DomAvg + DomVal;
				ILAvg = ILAvg + ILVal;

			}	
			GraphOutput<<(double)DomAvg/5<<" "<<(double)ILAvg/5<<"\n";
		}

		GraphOutput<<endl;
	}
	GraphOutput.close();
	DomFileGraph.close();
	ILFileGraph.close();
}
