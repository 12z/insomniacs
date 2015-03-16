#include <iostream>
#include <vector>
#include <tbb/tbb.h>
#include <time.h>

#define N 100000

int main()
{
	srand((unsigned)time(0));
	std::vector<unsigned> v1, v2;
	for (unsigned i = 0; i < N; i++) {
		v1.push_back(rand() % 100);
		v2.push_back(rand() % 100);
	}
	tbb::enumerable_thread_specific<unsigned> sum;
	tbb::parallel_for(0, N, [&](unsigned i) {
		sum.local() += v1[i] * v2[i];
	});

	std::cout << "Result: " << sum.combine(std::plus<unsigned>()) << std::endl;
	system("pause");
	return 0;
}