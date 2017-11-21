#include <iostream>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <tuple>
#include <vector>
#include <chrono>
using namespace std;

class Solution
{
public:
	Solution(int min, int max) : _anymin(min), _min(min), _anymax(max), _max(max) {};
	Solution(int anymin, int min, int anymax, int max) : _anymin(anymin), _min(min), _anymax(anymax), _max(max) {};
	int anymin() { return _anymin; }
	int min() { return _min; }
	int anymax() { return _anymax; }
	int max() { return _max; }
private:
	int _anymin, _min, _anymax, _max;
};

/**
 * O(n^2) solution which test all possible pairs, e.g. (ai, aj): i < j
 */
std::tuple<int, int, int> brutForce(const std::vector<int> &inputs) {
	int buy = 0, sell = 0, max = 0;

	for (int buyI = 0; buyI < inputs.size() - 1; ++buyI ) {
		for (int sellI = buyI + 1; sellI < inputs.size(); ++sellI ) {
			if (inputs[sellI] - inputs[buyI] > max) {
				buy = inputs[buyI];
				sell = inputs[sellI];
				max = sell - buy;
			}
		}
	}

	return {max, buy, sell};
}

/**
 * Finds best solution for range begin-end, using the best solution
 * from righ half and from left half. Additionally, check possible solution 
 * "in the middle" of divided intervals.
 */
Solution solve(const std::vector<int> &inputs, int begin, int end) {
	if (begin >= end) {
		return {0, 0};
	}
	if (begin + 1 == end) {
		return {inputs[begin],inputs[begin]};
	}

	int middle = (end - begin)/2;
	auto left = solve(inputs, begin, begin + middle);
	auto right = solve(inputs, begin + middle , end);

	int anymin, anymax;

	int leftEarn = left.max() - left.min();
	int rigthEarn = right.max() - right.min();
	int middleEarn = right.anymax() - left.anymin();

	int max = std::max({leftEarn, rigthEarn, middleEarn});

	anymin = std::min(right.anymin(), left.anymin());
	anymax = std::max(right.anymax(), left.anymax());

	if (max == leftEarn)
			return {anymin, left.min(), anymax, left.max()};
	else if (max == rigthEarn)
			return {anymin, right.min(), anymax, right.max()};
	else if (max == middleEarn)
			return {anymin, left.anymin(), anymax, right.anymax()};

	return {0,0};
}

/**
 * O(n) solution which uses divide-and-conquery approach. Uses additional class
 * Solution for more intuitive code.
 */
std::tuple<int, int, int> cleverSolution(const std::vector<int>& inputs) {
	auto solution = solve(inputs, 0, inputs.size());
	int sum = solution.max() - solution.min();

	return {sum, solution.min(), solution.max()};
}

/**
* Prints results in expected format
*/
void printResults(std::ostream& output, const std::tuple<int, int, int> &results) {
	output <<  std::get<0>(results) << " " << std::get<1>(results) << " " << std::get<2>(results) << std::endl;
}

/**
 * Implements Compare (timing) functionality using chrono library, with 
 * the diration in microseconds. However, to make the result more precise
 * each algorithm shoul run more times.
 */
void compare(const std::vector<int>& inputs) {
	auto t1 = std::chrono::high_resolution_clock::now();
    auto result1 = brutForce(inputs);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto result2 = cleverSolution(inputs);
    auto t3 = std::chrono::high_resolution_clock::now();
    if (std::get<0>(result1) != std::get<0>(result2)) {
    	std::cout << "Result is different!\n\n";
    	return;
    }
    auto slow = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();
    auto fast = std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count();
    std::cout << "O(n^2) solution took "
              << slow
              << " microseconds\n"
              << "O(n) solution took "
              << fast
              << " microseconds\n";
    if (slow == fast) {
    	std::cout << "No speedup \n\n";
    }else if (slow < fast) {
    	std::cout << "Slowdown: " << (slow == 0 ? "Not comparable" : std::to_string(fast/slow * 100)) << "%\n\n";
    }else {
      	std::cout << "Speedup: " << slow/fast * 100 << "%\n\n"; 
  	}
}

/**
 * Simple input reading.
 */
std::vector<int> read(const std::string &name) {
	ifstream input;
	input.open(name + ".txt");
	std::vector<int> values;
	int value;
	while(input.good()) {
		input >> value;
		values.push_back(value);
	}
	input.close();
	return values;
}

/**
 * Usage: 1. argument name of input file without .txt!
 * possible second argument "compare" to only compare fast and slow solution
 */
int main(int argc, char **argv) {
	if (argc <= 1 ) {
		std::cout << "Bad usage\n";
		return 1;
	}
	auto inputs = read(argv[1]);
	if (argc >= 3) {
		string argument(argv[2]);
		if (argument == "compare") {
			compare(inputs);
		}
	} else {
		auto result = cleverSolution(inputs);
		std::string name = argv[1];
		ofstream output (name +".myresults");
		printResults(output, result);
		output.close();
	}
	return 0;
}