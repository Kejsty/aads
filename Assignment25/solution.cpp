#include <iostream>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <chrono>
#include <queue>
using namespace std;

struct Node
{
	int number;
	int parent;;
	std::vector<int> childs;
	int opt;
	bool selected;
	Node (int number) : number(number), parent(-1), childs(), opt(-1), selected(false){}
};

/**
* Prints results in expected format
*/
void printResults(std::ostream& output, const std::set<int> &results) {
	output << results.size() << std::endl;
	for (auto val : results) {
		output << val << std::endl;
	}
}


/**
 * Simple input reading.
 */
std::vector<std::pair<int, int>> read(const std::string &name) {
	ifstream input;
	input.open(name);

	std::vector<std::pair<int, int>> values;
	int fst, snd, all;
	input >> all;
	values.reserve(all);
	input >> fst >> snd;
	while(input.good()) {
		values.push_back({fst, snd});
		input >> fst >> snd;
	}
	input.close();
	return values;
}

/**
 * Creates a tree from the input edges.
 */
std::vector<Node> parse (const std::string &name, bool inform = false) {
	std::vector<Node> nodes;

	auto edges = read(name);

	const size_t verticesCount = edges.size() + 1;
	nodes.reserve(verticesCount);
	if(inform) 
		std::cout << "Input size: " << verticesCount << std::endl;

	for(int i = 0; i < verticesCount; ++i) {
		nodes.emplace_back(i);
	}

	for (auto &edge : edges) {
		nodes[edge.first].childs.push_back(edge.second);
		nodes[edge.second].childs.push_back(edge.first);
	}

	//each vertex is added once = O(V+E)
	std::queue<std::pair<int, int>> investigate;
	investigate.push({0, -1});

	//executed O(V+E) times
	while(!investigate.empty()) {
		int myID, parentID;
		std::tie(myID, parentID) = investigate.front();
		investigate.pop();
		nodes[myID].parent = parentID;

		int position = -1;
		std::vector<int>& myChilds = nodes[myID].childs;
		for (int i = 0; i < myChilds.size(); ++i ) {
			if (myChilds[i] == parentID) {
				position = i;
			}else{
				investigate.push({myChilds[i], myID});
			}
		}
		if (position >= 0) {
			//remove parent ID
			myChilds[position] = myChilds.back();
			myChilds.pop_back();
		}

	}
	return nodes;
}

/**
 * Counts optimal value for node. After counting the value is store, so
 * the value is counted only once.
 */
int countOpt(std::vector<Node> &nodes, int index) {
	if (nodes[index].opt > -1) {
		return nodes[index].opt;
	}

	int withMe = 0, wihtoutMe = 0;

	withMe = 1;
	for (auto childID : nodes[index].childs ) {
		for (auto &grandChild : nodes[childID].childs ) {
			withMe += countOpt(nodes, grandChild);
		}
		wihtoutMe += countOpt(nodes, childID);
	}

	if (withMe > wihtoutMe) {
		nodes[index].selected = true;
	}
	nodes[index].opt = std::max(withMe, wihtoutMe);
	return nodes[index].opt;
}

/**
 * Incrementaly builds the set of selected nodes.
 */
void buildSet(std::set<int> &set, std::vector<Node> &nodes, int rootID) {
	if (nodes[rootID].selected) {
		set.insert(rootID);
		for (auto &childID : nodes[rootID].childs ) {
			for (auto &grandChild : nodes[childID].childs ) {
				buildSet(set,nodes, grandChild);
			}
		}
	}else{
		for (auto &child : nodes[rootID].childs ) {
			buildSet(set, nodes, child);
		}
	}
}

std::set<int> solve(const std::string &name, bool inform = false, bool shouldBuildSet = true) {
	//We count only the time that solving algorithm takes, woth time to parse
	auto t1 = std::chrono::high_resolution_clock::now();
	auto nodes = parse(name, inform);

	//root is a node wihtout parent;
	Node rootID = 0;
	int number = countOpt(nodes, 0);
	for (auto &node : nodes) {
	}

	std::set<int> result;
    if (shouldBuildSet) {
    	//build set for printing it out.
		buildSet(result, nodes, 0);
    }
	auto t2 = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();

	if (inform)
		std::cout << "Solving took "
			<< (shouldBuildSet ? "with building Set " : "without building Set ")
         	<< time
          	<< " microseconds\n";
	return result;
}

std::string removeSuffix(std::string name) {
	auto index = name.find_last_of(".");
	return name.substr(0, index);
}

int main(int argc, char **argv) {
	if (argc <= 1 ) {
		std::cout << "Bad usage\n";
		return 1;
	}

	if (argc == 2) {
		auto result = solve(argv[1]);
		std::cout << result.size() << std::endl;
		return 0;
	}

	std::string option(argv[1]);
	if (option == "-v") {
		auto result = solve(argv[2]);
		printResults(std::cout, result);
	}else if (option == "-time") {
		solve(argv[2], true);
		solve(argv[2], true, false);
	}else if (option == "-test") {
		auto result = solve(argv[2]);
		ofstream output (removeSuffix(argv[2]) +".myresults");
		printResults(output, result);
		output.close();
	}
	return 0;
}