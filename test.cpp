#include <random>
#include <iostream>
#include<time.h>
using namespace std;

int main()
{

    uniform_int_distribution<int> distribution(1,1000);

    default_random_engine generator(time(0));
    for (size_t i = 0; i < 10; i++)
    {
        int newNodeIndex = distribution(generator);
        cout<<newNodeIndex<<endl;
    }

    return 0;

}