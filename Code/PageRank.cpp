#include<iostream>
#include<math.h>
#include<omp.h>

#define N 4
using std::cout;
using std::cin;
using std::endl;


void init_WebGraph(double WebMatrix[][N])
{
    WebMatrix[0][0] = 0;
    WebMatrix[0][1] = 0;
    WebMatrix[0][2] = 1/2;
    WebMatrix[0][3] = 1/N;
    WebMatrix[1][0] = 1/3;
    WebMatrix[1][1] = 0;
    WebMatrix[1][2] = 0;
    WebMatrix[1][3] = 1/N;
    WebMatrix[2][0] = 1/3;
    WebMatrix[2][1] = 1/1;
    WebMatrix[2][2] = 0;
    WebMatrix[2][3] = 1/N;
    WebMatrix[3][0] = 1/3;
    WebMatrix[3][1] = 0;
    WebMatrix[3][2] = 1/2;
    WebMatrix[3][3] = 1/N;

}


void init_PageRank(double pageRank[])
{
    for (size_t i = 0; i < N; i++)
    {
        pageRank[i] = 1/N;
    }
}

void prod(double productMat[N],double WebMatrix[][N], double pageRank[N])
{
    for (size_t i = 0; i < N; i++)
    {   
        productMat[i] = 0;
        for (size_t j = 0; j < N; j++)
        {
            productMat[i] += WebMatrix[i][j]*pageRank[j];
        }
    }
}


int main(){

    double WebMatrix[N][N];                        // WebMatrix is state transition Matrix expressed in terms of Probablities of going from ith node to jth node
    double pageRank[N], pageRank_new[N];
    init_WebGraph(WebMatrix);
    init_PageRank(pageRank);
    cout << WebMatrix[1][1] << endl;

    double t = 0.0, c = 0.85;

    double error = 1.0, TOL = 0.00001;

    while(error > TOL)
    {   
        error = 0;
        for (size_t i = 0; i < N; i++)
        {
            double productMat[N];
            prod(productMat,WebMatrix,pageRank);
            pageRank_new[i] = (1 - c)/N + c * productMat[i];

            error += pow(pageRank_new[i] - pageRank[i],2);      
        }
        error = sqrt(error/N);

        for (size_t i = 0; i < N; i++)
        {
            pageRank[i] = pageRank_new[i]; 
        }
    }
    for (size_t i = 0; i < N; i++)
    {
        cout<<pageRank[i]<<" ";
    }
    cout<<endl;
    
    return 0;
}

