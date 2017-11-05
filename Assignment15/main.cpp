#include <iostream>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <tuple>
using namespace std;

using Coordinates = std::tuple<int, int, int>;
using Triangulation = std::map<int, std::pair<Coordinates, Coordinates>>;

/**
* Parses the given inuput file. The expected format is:
* NumberOfTriangle Point1 Point2 Point3 Neighbor1 Neighbor2 Neighbor3
*/
Triangulation parseFile(ifstream &file){
	Triangulation triangulation;
	int triNmr, a,b,c, x,y,z;
	while(file.good()) {
		file >> triNmr >> a >> b >> c >> x >> y >> z;
		Coordinates triangle = std::make_tuple(a,b,c);
		Coordinates neighbors = std::make_tuple(x,y,z);
		triangulation.insert(std::make_pair(triNmr, std::make_pair(triangle, neighbors)));
	}
	return triangulation;
}

/**
* Calculates, whether the line between two points makes edge of the polygon
*/
bool isEdgeOfPolygon(int a, int b, int last) {
	return (a-1 ==b) || (b-1 == a) || (a == 0 && b == last-1) || (b == 0 && a == last-1);
}

/**
* Finds (and removes) triangles, that are defiinitely outside the polygon. 
* Returns true, if there is found such a triangle.
*/
bool findMatch(std::set<int> &outsides, Triangulation &triangulation, int last) {
	std::set<int> newlyAdded;

	for(auto &triangleInfo : triangulation ) {
		int number = triangleInfo.first;
		Coordinates triangle, neighbors;
		int a,b,c ,x,y,z;
		std::tie(triangle, neighbors) = triangleInfo.second;
		std::tie(a,b,c) = triangle;
		std::tie(x,y,z) = neighbors;

		if ((!isEdgeOfPolygon(a,b,last) && outsides.find(z) != outsides.end()) || 
			(!isEdgeOfPolygon(b,c,last) && outsides.find(x) != outsides.end()) ||
			(!isEdgeOfPolygon(c,a,last) && outsides.find(y) != outsides.end()) ) {

			outsides.insert(number);
			newlyAdded.insert(number);
		}
	}

	//clean triangulation to contain only triangles, that are not (maybe) not outside
	for (int triangle : newlyAdded) {
		triangulation.erase(triangle);
	}

	//newlyAdded == 0 indicates that no new outisde triangle has been found
	return newlyAdded.size() != 0;
}

/**
* Gradually builds the set of triangles that are outside the polygon
*/
std::set<int> getOutsides(Triangulation &triangulation, int last) {
	std::set<int> outsides = {-1};

	while(findMatch(outsides, triangulation, last)) {};

	return outsides;
}

/**
* Prints results in expected format
*/
void printResults(std::ostream& output, const std::set<int> &results, int nrOfTriangles) {
	for (int i = 0; i < nrOfTriangles; ++i) {
		if(results.find(i) != results.end()) {
			output << i << " " << 0 << std::endl;
		}else{
			output << i << " " << 1 << std::endl;
		}
	}
}

/**
* Read and parse the file with filename $name + ".triangles"
* Counts the set of outside triangles and prints them in 
* expected format into file $name + ".myresults"
*/
void solve(const std::string &name) {
	ifstream input;
	input.open(name + ".triangles");
	int points, triangles;
	input >> points >> triangles;
	auto triangulation = parseFile(input);
	auto result = getOutsides(triangulation, points);
	input.close();
	ofstream output (name + ".myresults");
	printResults(output, result, triangles);
	output.close();
}

int main(int argc, char **argv) {
	if (argc <= 1 ) {
		std::cout << "Bad usage\n";
		return 1;
	}
	solve(argv[1]);
	return 0;
}