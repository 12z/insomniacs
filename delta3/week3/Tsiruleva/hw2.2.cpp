#include <iostream>
#include <tbb/enumerable_thread_specific.h>
#include <tbb/parallel_for.h>
#include <vector>
#include <functional>

#define N 100000

int main()
{
    std::vector<int> v1, v2;
    for (int i = 0; i < N; i++)
    {
        v1.push_back(rand()%100);
        v2.push_back(rand()%100);
    }
    tbb::enumerable_thread_specific <int> sum;
    tbb::parallel_for (0, N, [&](int i)
    {
        sum.local() += v1[i] * v2[i];
    });

    long res = sum.combine(std::plus<int>());
    std::cout << "Result: " << res << " \n";
    return 0;
}