#include <fstream>
#include <iostream>
#include <unordered_map>
#include <random>
#include <vector>
#include <string>
#include <cstring>
#include <math.h>
#include <omp.h>

using namespace std;

void writeFile(int PageRank[], int N)
{
    fstream fout;
    fout.open("PageRanksMCMC.txt", ios::out);

    if(fout.is_open())
    {   
        for (size_t i = 0; i < N; i++)
        {
            fout<<PageRank[i]<<endl;            
        }
    }

    fout.close();
}

void Split(char str[], int key[])
{
    // Returns first token 
    char *token = strtok(str, "\t");
    key[0] = atoi(token);
    token = strtok(NULL, "\t");
    key[1] = atoi(token);
}

unordered_map<int, vector<int>> init_WebGraph(string filename)
{   
    unordered_map<int, vector<int>> WebGraph;
    fstream fin;
    fin.open(filename, ios::in);

    if (fin.is_open())
    {
        string line;
        int key[2];
        int count = 0;
        while (getline(fin, line))
        {   
            char temp[200];
            strcpy(temp, line.c_str());
            Split(temp, key);
            WebGraph[key[0]].push_back(key[1]);
        }
        fin.close();
    }
    else
    {
        cout<<"File Read Error: No file / cannot read";
        exit(0);
    }
    
    return WebGraph;
}


int main()
{
    //Initializing the Web Graph from the Data
    unordered_map<int, vector<int>> WebGraph;           // Stores the Outgoing Links for each node 
    WebGraph = init_WebGraph("test.txt");
    // int NNodes = WebGraph.size();
    int NNodes = 4;

    double PageRank[NNodes] = {0};
    
    //RandomWalk
    int m = 2;
    double c = 0.85;
    int MaxWalks = 10;
    // int NumWalks = m*NNodes;
    int NumWalks = 1000;
    
    default_random_engine generator(42);
    uniform_int_distribution<int> AllNodesDistr(1,NNodes);
    uniform_real_distribution<double> ProbDistr(0,1);

    // #pragma omp parallel for num_threads(2) reduction(+:PageRank)
    for (int i = 0; i < NumWalks; i++)
    {
        int WalkCount = 0;

        int currNode = AllNodesDistr(generator);
        // int currNode = i/m;
        // int currNode = 2;

        default_random_engine rand_generator(time(0));

        cout<<"-------"<<endl<<"  "<<i<<"  "<<endl<<"-------"<<endl;
        cout<<"The starting node = "<<currNode << endl;
        
        while(true)
        {
            if (WebGraph[currNode].size() > 0)
            {
                double RandNum =  ProbDistr(rand_generator);
                int newNodeIndex;
                if(RandNum <= c)
                {
                    int sizeOutgoing = WebGraph[currNode].size();
                    uniform_int_distribution<int> OutgoingDistribution(0,sizeOutgoing-1);
                    newNodeIndex = OutgoingDistribution(rand_generator);
                    currNode = WebGraph[currNode][newNodeIndex];
                }
                else
                {
                    currNode = AllNodesDistr(generator);
                }

                // cout<<currNode<<endl;

                WalkCount++;
                if (WalkCount >= MaxWalks)
                    break;
            }
            else
            {
                //Stop Random Walk
                // cout<<"\nEnding because no outgoing links. Current Node ="<<currNode<<"\n";
                break;
            }
        }
        PageRank[currNode-1] += 1;
        cout<<"The ending node = "<<currNode<<endl;
        cout<<"The number of steps done = "<<WalkCount<<endl;
    }

    for (size_t i = 0; i < NNodes; i++)
    {
        cout<<PageRank[i]<<" ";
    }
    // writeFile(PageRank, NNodes);
    return 0;
}

