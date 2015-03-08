#include <thread>
#include <mutex>
#include <iostream>
#include <vector>

#define N 100000
std::mutex mtx;

void mul(std::vector<int> a, std::vector<int> b, int start, int end, int &res)
{
    int temp_res = 0;
    for (int i = start; i < end; i++)
        temp_res += a[i] * b[i];

    std::lock_guard<std::mutex> locked(mtx);
    res += temp_res;
}


int main()
{
    std::vector<int> v1, v2;
    for (int i = 0; i < N; i++)
    {
        v1.push_back(rand()%100);
        v2.push_back(rand()%100);
    }
    unsigned int nthreads = std::thread::hardware_concurrency();
    std::vector <std::thread> threads;
    int res = 0;
    // split vector by threads
    int range = v1.size()/nthreads;
    int start = 0;
    // the first thread do more or equal job than others
    int end = range + v1.size()%nthreads;
    for (int i = 0; i < nthreads; i++)
    {
        threads.push_back(std::thread(mul, v1, v2, start, end, std::ref(res)));
        start = end;
        end = start + range;
    }
    for( auto& t: threads )
        t.join();

    std::cout << "Result: " << res << " \n";
    return 0;
}