#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include <time.h>

#define N 100000
std::mutex result_lock;

void calc(std::vector<int> a, std::vector<int> b, int first, int last, int &res)
{
	int tmp = 0;
	for (int i = first; i < last; i++) {
		tmp += a[i] * b[i];
	}

	std::lock_guard<std::mutex> locked(result_lock);
	res += tmp;
}


int main()
{
	srand((unsigned)time(0));
	std::vector<int> v1, v2;
	for (int i = 0; i < N; i++) {
		v1.push_back(rand() % 100);
		v2.push_back(rand() % 100);
	}
	unsigned int nthreads = std::thread::hardware_concurrency();
	std::vector <std::thread> threads;
	int result = 0;
	int step = v1.size() / nthreads;
	int first = 0;
	int last = step + v1.size() % nthreads;

	for (unsigned i = 0; i < nthreads; i++) {
		threads.push_back(std::thread(calc, v1, v2, first, last, std::ref(result)));
		first = last;
		last = first + step;
	}
	for (auto& t : threads) {
		t.join();
	}

	std::cout << "Result: " << result << std::endl;
	system("pause");
	return 0;
}
