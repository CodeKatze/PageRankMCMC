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


void Split(char str[], int key[])
{
    // Returns first token 
    char *token = strtok(str, "\t");
    key[0] = atoi(token);
    token = strtok(NULL, "\t");
    key[1] = atoi(token);
}

unordered_map<int, vector<int>> init_WebGraph()
{   
    unordered_map<int, vector<int>> WebGraph;
    fstream fin;
    fin.open("graphData.txt", ios::in);

    if (fin.is_open())
    {
        string line;
        int key[2];
        int count = 0;
        while (getline(fin, line))
        {   
            // int key = line
            char temp[200];
            strcpy(temp, line.c_str());
            Split(temp, key);
            // cout << key[0] << " -> " << key[1] << endl << flush;
            WebGraph[key[0]].push_back(key[1]);
        }

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
    unordered_map<int, vector<int>> WebGraph;           // Outgoing Links
    WebGraph = init_WebGraph();
    int NNodes = 281903;

    
    //RandomWalk
    int MaxWalks = 100;
    uniform_int_distribution<int> distribution(1,NNodes);

    for (int i = 0; i < 10; i++)
    {
        default_random_engine generator(i);
        int WalkCount = 0;

        // int currNode = distribution(generator);
        int currNode = 3;
        cout<<"The starting node for "<<i<<" = "<<currNode<<endl;
        default_random_engine generator1(time(0));
        
        while(true)
        {
            if (WebGraph[currNode].size() > 0)
            {
                int sizeOutgoing = WebGraph[currNode].size();
                uniform_int_distribution<int> OutgoingDistribution(0,sizeOutgoing-1);

                int newNodeIndex = OutgoingDistribution(generator1);
                newNodeIndex = OutgoingDistribution(generator1);
                currNode = WebGraph[currNode][newNodeIndex];
                cout<<currNode<<endl;

                WalkCount++;
                if (WalkCount >= MaxWalks)
                    break;
            }
            else
            {
                //Stop Random Walk
                cout<<"\nNo outgoing nodes. Ending \n";
                break;
            }
        }

        cout<<"The ending node for "<<i<<" = "<<currNode<<endl;
        cout<<"The number of walks done by "<<i<<" - "<<WalkCount<<endl;
    }
   
    return 0;
}

