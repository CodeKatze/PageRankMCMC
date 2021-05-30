#include <fstream>
#include <iostream>
#include <unordered_map>
#include <random>
#include <vector>
#include <string>
#include <cstring>
#include <math.h>
#include <chrono>
#ifdef _OPENMP
    #include <omp.h>
#endif

using namespace std;

void writeFile(double PageRank[], int N)
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


int main(int argc, char* argv[])
{
    int thread_count = 1;
    // read the number of threads to be used for parallelization 
    if (argc == 2)
    {
      thread_count = strtol(argv[1], NULL, 10);
    }
    else
    {
      printf("\n A command line argument other than name of the executable is required!");
      return 1;
    }


    string FILENAME = "graphData.txt";                   // File containing the data
    bool VERBOSE = false;                                // true to print results

    // Important flags to choose algorithm (REFER to README for flag settings for each algorithm)
    bool STOP_AT_DANGLING = true;                        // upon encountering a dangling node -> Stop or randomly restart at another node 
    bool RANDOM_INIT = true;                             // start walk at random node or start m random walks @ each of the NNodes
    int USE_VISITS = 1;                                   // 0 - Use Ending Node, 1 & 2 - Use Visited node 

    // Parameters
    int NNodes = 281903;                                 // Number of Nodes in the data
    // int NNodes = 4;                                      // Number of Nodes in the data
    double c = 0.85;                                     // Damping parameter
    int m = 50;                                         // number of walks that start from each node
    int NumWalks = m*NNodes;                             

    //Initializing the Web Graph from the Data
    unordered_map<int, vector<int>> WebGraph;            // Stores the Outgoing Links for each node 
    WebGraph = init_WebGraph(FILENAME); 

    double PageRank[NNodes] = {0};                       // PageRank score for each page
    double Visits[NNodes] = {0};                         // Number of times each page is visited across all the walks
    
    //Random Number Generators
    default_random_engine generator(time(0));
    uniform_int_distribution<int> AllNodesDistr(1,NNodes);
    uniform_real_distribution<double> ProbDistr(0,1);

    auto time_start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for num_threads(thread_count) reduction(+:PageRank)
    for (int i = 0; i < NumWalks; i++)
    {
        int StepCount = 0;
        int currNode;
        if(RANDOM_INIT == true) // Initialize the starting node randomly
        {
            currNode = AllNodesDistr(generator);
        }
        else                    // Initialize it uniformly
        {
            currNode = (i/m)+1;
        }

        if(VERBOSE)
        {
            cout<<"-------"<<endl<<"  "<<i<<"  "<<endl<<"-------"<<endl;
            cout<<"The starting node = "<<currNode << endl;
        }
        
        // Start the Random Walk
        while(true)
        {
            double RandNum =  ProbDistr(generator);
            int newNodeIndex;
            if(RandNum <= c)    // Continue the Walk with a certain probability
            {
                if (WebGraph[currNode].size() > 0)  // if currNode has outgoing nodes
                {
                    int sizeOutgoing = WebGraph[currNode].size();
                    uniform_int_distribution<int> OutgoingDistribution(0,sizeOutgoing-1);
                    newNodeIndex = OutgoingDistribution(generator);
                    currNode = WebGraph[currNode][newNodeIndex];
                    StepCount++;
                }
                else                                // if currNode is a dangling node
                {
                    if (STOP_AT_DANGLING  == true)      // Stop Random Walk at dangling node
                    {
                        break;
                    }
                    else                                // Jump to a random node and try again
                    {
                        currNode = AllNodesDistr(generator);
                        StepCount++;
                    }
                }
            }
            else                // Break from the loop with a certai probability 
            {
                break;
            }
            
            Visits[currNode-1] += 1;
        }
        PageRank[currNode-1] += 1;
        Visits[currNode-1] += 1;

        if(VERBOSE)
        {
            cout<<"The ending node = "<<currNode<<endl;
            cout<<"The number of steps done = "<<StepCount<<endl;
        }
    }
    auto time_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long double> time_elapsed = time_end - time_start;
    cout<<"Time Taken = "<<time_elapsed.count()<<" seconds";

    if(USE_VISITS == 1)         // Divide by Total Visited Pages
    {
        double TotalVisits = 0;
        for (size_t i = 0; i < NNodes; i++)
        {
            TotalVisits += Visits[i];
        }
        for (size_t i = 0; i < NNodes; i++)
        {
            PageRank[i] = Visits[i]/TotalVisits;
        }
    }
    else if(USE_VISITS == 2)    // Multiply by (1-c)/(mn)
    {
        for (size_t i = 0; i < NNodes; i++)
        {
            PageRank[i] = Visits[i]*(1-c)/NumWalks;
        }

    }
    else                        // Use pagerank
    {
        for (size_t i = 0; i < NNodes; i++)
        {
            PageRank[i] = PageRank[i]/NumWalks;
        }
    }

    // for (size_t i = 0; i < NNodes; i++)
    // {
    //     cout<<PageRank[i]<<", ";
    // }

    writeFile(PageRank, NNodes);
    return 0;
}

