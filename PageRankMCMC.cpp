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
    int b = 2; 
    int a = WebGraph[b].size();
    cout<<"Size  = "<<a<<endl;

    // for (size_t i = 0; i < a; i++)
    // {
    //     cout<<WebGraph[b][i]<<endl;
    // }
    
    //RandomWalk
    default_random_engine generator(10);
    uniform_int_distribution<int> distribution(1,NNodes);

    int currNode = distribution(generator);
    cout<<currNode;
    
    // while()
    // {
    //     if WebGraph[currNode].size()>0
    //     {
    //         // update curr
    //         // stopping conds for stopping walk
    //     }
    //     else
    //     {

    //     }
        
    // }
   
    return 0;
}

