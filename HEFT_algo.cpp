#include "myfile.h"
#include<bits/stdc++.h>
using namespace std;

int *earliest_finish_time[25001];
int *earliest_start_time[25001];
int actual_finish_time[25001];
vector <int> parent[25001];
int mini_processor[25001];
int is_free[1001];
double upper_rank[25000];
double avg[25000];
int *data_graph[25000];
int *computation_time[25000];
int proc;
int tasks;
vector < pair <double, int> > upper_rank_of_tasks;

bool comp(pair <double, int> a,pair <double, int> b)
{
	return a.first<b.first;
}
struct task_duration
{	
    int start_time;
	int finish_time;
	int task_no;
};


vector <task_duration> processor_scheduler[25001];

double find_upper_rank(int i)
{
    double maxi = 0;
	double temp;
    
    for (int j = 0; j < proc; j++) {
    	//cout<<"i="<<i<<"j="<<j<<"data_graph="<<data_graph[i][j]<<endl;
        if(data_graph[i][j] != -1) 
		{
			temp = find_upper_rank(j);
            if (maxi < data_graph[i][j] + temp)
            maxi = data_graph[i][j] + temp;
        }

    }
    upper_rank[i] = avg[i] + maxi;
    return upper_rank[i];
}

int earliest_start_time_cal(int i, int j)
{
	int comm;
	int par;
	int max_time;
	int temp = 0;

	max_time = 0;

	for (int k = 0; k < parent[i].size(); k++) 
	{
		par = parent[i][k];
		if(mini_processor[par] == j) 
		{
			temp = actual_finish_time[par];
		}

		else 
		{
			temp = data_graph[par][i] + actual_finish_time[par];
		}

		max_time = max(max_time, temp);
	}

	return max_time;

}
			




int earliest_finish_time_cal(int i, int j)
{
		int start_time;
	//	cout<<"PROCESSOR="<<j<<endl;
		if(processor_scheduler[j].empty()) 
		{
			//cout<<"empt  "<<earliest_start_time[i][j] + computation_time[i][j]<<"\n";
			
			return earliest_start_time[i][j] + computation_time[i][j];
		}
		int flag;
		int endtime;
		flag = 1;
		for (int counter = earliest_start_time[i][j]; 1; counter++) 
		{
			flag = 1;
			endtime = counter + computation_time[i][j];
			for (int k = 0; k < processor_scheduler[j].size(); k++) 
			{
				if((counter <= processor_scheduler[j][k].start_time and endtime > processor_scheduler[j][k].start_time) or (counter < processor_scheduler[j][k].finish_time and endtime >= processor_scheduler[j][k].finish_time) ) 
				{
					flag = 0;
					break;
				}
			}
			

			if(flag == 1) 
			{
			//	cout<<"counter="<<counter<<endl;
				return endtime;
			}

		}


}


void heft()
{
	int mini;
	mini = INT_MAX;
	int t;
	int mini_earliest_finish_time;
	int length = -1;

	task_duration temp;
	task_duration temp2;
	t = upper_rank_of_tasks[0].second;

	for (int i = 0; i < proc; i++) 
	{
		earliest_start_time[upper_rank_of_tasks[0].second][i] = 0;
		if(mini > computation_time[t][i]) 
		{
			mini = computation_time[t][i];
			mini_processor[t] = i;
		}

	}

	temp.start_time = 0;
	temp.finish_time = mini;
	temp.task_no = t;
   // cout<<"task="<<t<<"processor="<<mini_processor[t]<<"start_time="<<temp.start_time<<"finist_time="<<temp.finish_time<<endl;

	processor_scheduler[mini_processor[t]].push_back(temp);

	actual_finish_time[upper_rank_of_tasks[0].second] = mini;
	int i;
	for (int h = 1; h < tasks; h++) 
	{
		i = upper_rank_of_tasks[h].second;

		for (int j = 0; j < proc; j++) 
		{
			earliest_start_time[i][j] = earliest_start_time_cal(i, j);
			//cout<<"i="<<i<<"j="<<j<<"EST="<<earliest_start_time[i][j]<<endl;
		}
		
		mini_earliest_finish_time = INT_MAX;
		int processor_no = 0;

		for (int j = 0; j < proc; j++) 
		{
			earliest_finish_time[i][j] = earliest_finish_time_cal(i, j);
			if(earliest_finish_time[i][j] < mini_earliest_finish_time) 
			{
				mini_earliest_finish_time = earliest_finish_time[i][j];
				processor_no = j;
			}
		}	
		mini_processor[i] = processor_no;		
		temp2.start_time = mini_earliest_finish_time - computation_time[i][processor_no];
		temp2.finish_time = mini_earliest_finish_time;
		actual_finish_time[i] = mini_earliest_finish_time;
		temp2.task_no = i;
		length = max(mini_earliest_finish_time, length);
		processor_scheduler[processor_no].push_back(temp2);
	//	cout<<"task="<<i<<"processor="<<processor_no<<"start_time="<<temp2.start_time<<"finist_time="<<temp2.finish_time<<endl;

	}
	cout<<setw(49)<<"____________________________________________________________________________________________\n";
	cout<<"|"<<setw(10)<<"Task  "<<setw(2)<<"|"<<setw(10)<<"Rank  "<<setw(2)<<"|"<<setw(20)<<"  Start Time    "<<setw(2)<<"|"<<setw(20)<<"    Finish Time    "<<setw(2)<<"|"<<setw(20)<<"Processor No.  "<<setw(3)<<"|"<<endl;
	cout<<setw(49)<<"|___________|___________|_____________________|_____________________|______________________|\n";
	for (int i = 0; i < proc; i++) 
	{
		for (int j = 0; j < processor_scheduler[i].size(); j++) 
		{
			cout<<"|"<<setw(10)<<processor_scheduler[i][j].task_no<<setw(2)<<"|"<<setw(10)<<upper_rank[processor_scheduler[i][j].task_no]<<setw(2)<<"|"<<setw(20)<<processor_scheduler[i][j].start_time<<setw(2)<<"|"<<setw(20)<<processor_scheduler[i][j].finish_time<<setw(2)<<"|"<<setw(20)<<i<<setw(3)<<"|"<<endl;
			//cout << "Task="<<processor_scheduler[i][j].task_no<<" rank => " << upper_rank[processor_scheduler[i][j].task_no] << "start time " << processor_scheduler[i][j].start_time<<"Finish Time = "<<processor_scheduler[i][j].finish_time<<endl;
		}
	//	cout << endl;
	}
	cout<<setw(49)<<"|___________|___________|_____________________|_____________________|______________________|\n";
	cout <<"Schedule Length="<<length<<endl;	
}


	
int main()
{
    for(int i=0;i<25001;i++)
    {
    	data_graph[i]= (int*)calloc(25000,4);
    	computation_time[i]=(int *)calloc(1000,4);
    	earliest_finish_time[i]=(int *)calloc(1000,4);
		earliest_start_time[i]=(int *)calloc(1000,4);
	}
	//data_graph[0][1000]=10;
    float ans;
    int p;
	double temp;
	int edges_count;
	//memset(data_graph, -1, sizeof(data_graph));
	for(int i=0;i<1000;i++)
	{
		for(int j=0;j<1000;j++)
		 data_graph[i][j]=-1;
	}
//	cout<<data_graph[0][100]<<endl;
	cout <<"Enter number of tasks : \n";
	
	cin>>tasks;
	
	cout<< "Enter number of pocessors\n";
	
    cin>>proc;
    
    int out_degree;
    
    double alpha,ccr,beta;
    
    cout<<"Enter Out Degree Of Nodes\n";
    
    cin>>out_degree;
    
    cout<<"Enter Shape Parameter Of Graph (ALPHA)\n";
    
    cin>>alpha;
    
    cout<<"Enter Communication to Computation Cost Ratio (CCR) \n";
    
    cin>>ccr;
    
    cout<<"Enter Range Percentage of Computation Cost on Processors (BETA)\n";
    
    cin>>beta;
    
    vector<list <pair<int,double >> > graph=generator(tasks,proc,out_degree,alpha,ccr,beta);
    
    
  for(int i=1;i<=tasks;i++)
  {
  	
  	list<pair<int,double> > :: iterator it;
  	//cout<<"SIZE="<<graph[i].size()<<endl;
  	for(it=graph[i].begin();it!=graph[i].end();++it)
  	{
  		int j=(*it).first;
  		int cost=(*it).second;
  	//	cout<<"i="<<i<<"j="<<j<<"cost="<<cost<<endl;
  		data_graph[i][j]=cost;
  	//	cout<<"ASSIGNED\n";
	}
	//cout<<"i="<<i<<endl;
  }
  //	cout<<"FINE\n";  
  	
    vector<vector<int> > v= return_comp_cost();
    
    for(int i=0;i<tasks;i++)
    {
    	for(int j=0;j<proc;j++)
    	{
    		computation_time[i][j]=v[i][j];
		}
	}
/*	for(int i=0;i<tasks;i++)
    {
    	for(int j=0;j<proc;j++)
    	{
    		cout<<computation_time[i][j]<<"  ";
		}
		cout<<endl;
	}*/
/*	for(int i=0;i<tasks;i++)
    {
    	for(int j=0;j<tasks;j++)
    	{
    		cout<<data_graph[i][j]<<"  ";
		}
		cout<<endl;
	}*/
	for (int i = 0; i < tasks; i++) 
	{
		temp = 0;
    	for (int j = 0; j < proc; j++) 
		{
			temp += computation_time[i][j];
		}
		avg[i] = (double) temp / (double) proc;
	}
	for(int i=0;i<tasks;i++)
	{
		for(int j=0;j<tasks;j++)
		{
			if(data_graph[i][j]!=-1)
			{
				parent[j].push_back(i);
			}
		}
	}
	for (int i = 0; i < tasks; i++)
	{
        find_upper_rank(i);
        //cout<<"i="<<i<<"upper_rank="<<upper_rank[i]<<endl;
    }

	

    for (int i = 0; i < tasks; i++) 
	{
		upper_rank_of_tasks.push_back(make_pair(upper_rank[i], i));
    }

	sort(upper_rank_of_tasks.begin(), upper_rank_of_tasks.end(),comp);
    

	heft();

    return 0;

}

