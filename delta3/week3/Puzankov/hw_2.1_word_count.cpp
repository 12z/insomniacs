#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tbb/tbb.h>

unsigned count_words(std::ifstream &input, tbb::task_group &g)
{
	std::string line = "";
	tbb::enumerable_thread_specific<unsigned> sum;
	while (!input.eof()) {
		std::getline(input, line);
		g.run([=, &sum] {
			std::string new_string = "";
			std::stringstream ss;
			ss.str(line);
			while (ss >> new_string) {
				sum.local() += 1;
			}
		});
	}
	g.wait();
	return sum.combine(std::plus<unsigned>());
}

int main()
{
	std::ifstream input("input.txt");
	if (!input) {
		return -1;
	}
	tbb::task_group g;
	unsigned result = count_words(input, g);
	input.close();
	std::cout << "number of words: " << result << std::endl;
	system("pause");
	return 0;
}
