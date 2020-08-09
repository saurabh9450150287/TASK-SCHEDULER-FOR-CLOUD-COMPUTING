#include "myfile.h"
#include<bits/stdc++.h>
using namespace std;

int N;

vector<vector<int> > computation_cost;

vector<list <pair<int,double >> > GenerateRandomGraphs(int N,double alpha,int out_degree,double ccr,double beta,int processors,int **comp_cost) 
{ 
   vector<list <pair<int,double >> > graph(N+1);
   int ecnt=0;
   int rootV=floor(sqrt(N));
   int height=rootV/alpha;
   int arr[height];
   int a=N;
   for(int i=0;i<height;i++)
   {
   arr[i]=1;
   a--;
   }
   srand(time(NULL));
   while(a)
   {
   	int inc=((rand())%height+height)%height;
   	arr[inc]++;
   	a--;
   }
   //for(int i=0;i<height;i++)
   //cout<<"i="<<i<<"  "<<"arr[i]="<<arr[i]<<endl;
  // cout<<"done1\n";
   int nodes=0;
   int communication_cost=0;
   int edges=0;
   for (int i = 0; i < height; i++)
    {//cout<<"i="<<i<<"nodes="<<nodes<<endl;
      int new_nodes = arr[i];
      for (int j = 0; j < nodes; j++)
        {
		for (int k = 0; k < new_nodes; k++)
        { int a=rand()%100;
		//  cout<<"CHANCE="<<a<<endl;
          if ( a < 60&&graph[j].size()<out_degree)
           {
           	int comm_cost=rand()%100;
           	graph[j].push_back(make_pair(k+nodes,comm_cost));
           //	cout<<"j="<<j<<"k="<<k+nodes<<"comm_cost="<<comm_cost<<endl;
           	edges++;
           	communication_cost+=comm_cost;
		   }
		}
	  }
      nodes += new_nodes;
    }
   // cout<<"EDGES="<<edges<<endl;
    //cout<<"done2\n";
    double average_comm_cost=(double)(communication_cost)/edges;  
    double average_comp_cost=average_comm_cost/ccr;
    int range=floor(2*average_comp_cost);
    
    for(int i=0;i<N;i++)
    {
    	int wi=floor(((rand())%range+range)%range);
    	for(int j=0;j<processors;j++)
    	{
    		int lv=wi*(1.0-beta/2),uv=wi*(1.0+beta/2);
    		
    		comp_cost[i][j] = max(1,(rand() % (uv - lv + 1)) + lv);
		}
	}
  for(int i=0;i<N;i++)
  {
  	vector<int> v;
  	for(int j=0;j<processors;j++)
  	{
  	v.push_back(comp_cost[i][j]);	
	}
	computation_cost.push_back(v);
  }
	//cout<<"OKAY\n";
      return graph;
}


vector<vector<int> >   return_comp_cost()
{
	return computation_cost;
}


vector<list <pair<int,double >> > generator(int N,int processors,int out_degree,double alpha,double ccr,double beta)
{
  int *comp_cost[N];
  for(int i=0;i<N;i++)
  {
  	comp_cost[i]=(int*)malloc(sizeof(int)*processors);
  }
 // adj[1000][1000]=1;
  return GenerateRandomGraphs(N,alpha,out_degree,ccr,beta,processors,comp_cost);
  //return graph;
  //return 0;
}

