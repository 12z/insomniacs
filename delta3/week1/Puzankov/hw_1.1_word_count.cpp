#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>

std::mutex queue_mutex;
std::mutex result_mutex;
std::condition_variable cvar;
std::atomic<bool> done;

void count_words(std::queue<std::string> &lines, unsigned &result)
{
	std::string new_string = "";
	std::stringstream ss;
	while (!done || !lines.empty()) {
		std::unique_lock<std::mutex> locked(queue_mutex);
		while (lines.empty()) {
			cvar.wait(locked);
			if (done && lines.empty()) return;
		}
		ss.clear();
		ss.str(lines.front());
		lines.pop();
		while (ss >> new_string) {
			std::lock_guard<std::mutex> locked(result_mutex);
			result++;
		}
	}
}

void read_line(std::ifstream &input, std::queue<std::string> &lines)
{
	std::string line = "";
	while (!input.eof()) {
		std::lock_guard<std::mutex> locked(queue_mutex);
		std::getline(input, line);
		lines.push(line);
		cvar.notify_one();
	}
	done = true;
	cvar.notify_all();
}

int main()
{
	std::ifstream input("input.txt");
	if (!input) {
		return -1;
	}
	unsigned result = 0;
	std::queue<std::string> lines;
	std::vector<std::thread> threads;
	threads.push_back(std::thread(read_line, std::ref(input), std::ref(lines)));
	unsigned nthreads = std::thread::hardware_concurrency() - 1;
	for (unsigned i = 0; i < nthreads; ++i) {
		threads.push_back(std::thread(count_words, std::ref(lines), std::ref(result)));
	}
	for (auto& t : threads) {
		t.join();
	}
	input.close();
	std::cout << "number of words: " << result << std::endl;
	system("pause");
	return 0;
}
