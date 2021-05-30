#include <random>
#include <iostream>
#include <time.h>
#include <omp.h>
using namespace std;

int main()
{
    int sum[10] = {};

    #pragma omp parallel for num_threads(13)
    for (size_t i = 0; i < 100; i++)
    {
        for (int j=0; j<10; j++)
        {
            sum[j] += 1;
        }
    }
    
    for (size_t i = 0; i < 10; i++)
    {
        cout<<sum[i]<<" "<<endl ;
    }
    

    return 0;

}