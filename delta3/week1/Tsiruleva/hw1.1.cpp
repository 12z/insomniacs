#include <thread>
#include <iostream>
#include <mutex>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <condition_variable>

std::mutex mtx_io;
std::mutex mtx_count;
std::condition_variable cond;

void read_line(std::ifstream &f, std::queue<std::string> &q)
{
    while (!f.eof())
    {
        std::unique_lock<std::mutex> locked(mtx_io);
        std::string str;
        if (std::getline(f, str))
            q.push(str);
        else
            q.push("\0");
        cond.notify_all();
    }
}

void count(std::queue<std::string> &q, unsigned int &result)
{
    std::stringstream line;
    while (true)
    {
        {
            std::unique_lock<std::mutex> locked(mtx_io);
            cond.wait(locked, [&]() {return !q.empty();});
            // finish thread if reading file is ended and tasks queue is empty
            if (q.front().compare("\0") == 0)
                break;
            line.str(q.front());
            q.pop();
        }
        {
            std::lock_guard<std::mutex> locked(mtx_count);
            std::string word;
            while (line >> word)
                result++;
        }
        
    }
}

int main()
{
    std::ifstream file("test.txt");
    if (!file.is_open())
    {
        std::cout << "Unable to open file\n";
        return 1;
    }
    // list of strings
    std::queue<std::string> tasks;
    unsigned int total = 0;
    std::vector <std::thread> threads;
    // create one reader and some counters
    threads.push_back(std::thread(read_line, std::ref(file), std::ref(tasks)));
    unsigned nthreads = std::thread::hardware_concurrency() - 1;
    for (unsigned int i = 0; i < nthreads; i++)
        threads.push_back(std::thread(count, std::ref(tasks), std::ref(total)));
     for( auto& t: threads )
        t.join();

    file.close();
    std::cout << "There are " << total << " words in file\n";
    return 0;
}