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
	Node *up;
	std::vector<Node *> childs;
	int opt;
	bool selected;
	Node (int number) : number(number), up(nullptr), childs(), opt(-1), selected(false){}
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
	input.open(name + ".tree");

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
 * Painfully creates a tree from the input edges.
 */
std::map<int, std::unique_ptr<Node>> parse (const std::string &name) {
	std::map<int, std::unique_ptr<Node>> nodes;

	std::map<int, std::set<int>> successors;
	auto edges = read(name);

	for (auto &edge : edges) {
		successors[edge.first].insert(edge.second);
		successors[edge.second].insert(edge.first);
	}

	std::queue<std::pair<int, int>> investigate;
	investigate.push({successors.begin()->first, -1});

	while(!investigate.empty()) {
		int toCheck, parentID;
		std::tie(toCheck, parentID) = investigate.front();
		investigate.pop();
		auto &allSucc = successors[toCheck];

		Node *parent = nullptr;

		if (parentID != -1 ) {
			auto parentIt = nodes.find(parentID);
			parent = parentIt->second.get();
		}

		Node *me;
		auto meIt = nodes.insert({toCheck, std::make_unique<Node>(toCheck)}).first;
		me = meIt->second.get();

		if (parent != nullptr) {
			parent->childs.push_back(me);
		}
		me->up = parent;
		for (int myChild : allSucc) {
			if (!parent || myChild != parent->number ) {
				investigate.push({myChild, toCheck});
			}
		}
	}

	return nodes;
}

/**
 * Counts optimal value for node. After counting the value is store, so
 * the value is counted only once.
 */
int countOpt(Node *node) {
	if (node->opt > -1) {
		return node->opt;
	}

	int withMe = 0, wihtoutMe = 0;

	withMe = 1;
	for (auto &child : node->childs ) {
		for (auto &grandChild : child->childs ) {
			withMe += countOpt(grandChild);
		}
		wihtoutMe += countOpt(child);
	}

	if (withMe > wihtoutMe) {
		node->selected = true;
	}
	node->opt = std::max(withMe, wihtoutMe);
	return node->opt;
}

/**
 * Incrementaly builds the set of selected nodes.
 */
void buildSet(std::set<int> &set, Node *node) {
	if (node->selected) {
		set.insert(node->number);
		for (auto &child : node->childs ) {
			for (auto &grandChild : child->childs ) {
				buildSet(set, grandChild);
			}
		}
	}else{
		for (auto &child : node->childs ) {
			buildSet(set, child);
		}
	}
}

std::set<int> solve(const std::string &name) {
	auto nodes = parse(name);

	//root is a node wihtout parent;
	Node *root = nullptr;
	for (auto &node : nodes) {
		if (node.second->up == nullptr) {
			root = node.second.get();
			break;
		}
	}

	//We count only the time that solving algorithm takes, not parsing time.
	auto t1 = std::chrono::high_resolution_clock::now();
	int number = countOpt(root);
	auto t2 = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();
	std::cout << "Solving took "
              << time
              << " microseconds\n";

    //build set for printing it out.
	std::set<int> result;
	buildSet(result, root);
	return result;

}

int main(int argc, char **argv) {
	if (argc <= 1 ) {
		std::cout << "Bad usage\n";
		return 1;
	}
	auto result = solve(argv[1]);
	std::string name(argv[1]);
	ofstream output (name +".myresults");
	printResults(output, result);
	output.close();
	return 0;
}