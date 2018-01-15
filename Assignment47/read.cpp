#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <iostream>
#include <vector>
#include <fstream>

using Range = std::pair<int, int>;

enum Color
{
	Black,
	White,
	Gray
};

/**
Decides whether given area should be divided. Some area is divided, if it is not uniformly colored.
*/
bool shoudDivide(uint8_t* image, Range rangeW, Range rangeH, int totalWidth ){
	Color foundTillNow = image[rangeH.first*totalWidth + rangeW.first] == 0 ? Black : White;

	for (int y = rangeH.first; y < rangeH.second; ++y) {
		for (int x = rangeW.first; x < rangeW.second; ++x) {
			Color foundNow = image[y*totalWidth + x] == 0 ? Black : White;
			if (foundNow != foundTillNow)
				return true;
		}
	}
	return false;
}

/**
Node representing the base structure of the quadtree
*/
class Node {
public:
	Node(uint8_t* image, Range rangeW, Range rangeH, int totalWidth):_rangeW(rangeW), _rangeH(rangeH) {
		if( shoudDivide(image, rangeW, rangeH, totalWidth) ) {
			_color = Gray;
			int xMiddle = rangeW.first + (rangeW.second - rangeW.first) / 2;
			int yMiddle = rangeH.first + (rangeH.second - rangeH.first) / 2;
			_childs.emplace_back(image, std::make_pair(rangeW.first, xMiddle), std::make_pair(rangeH.first, yMiddle), totalWidth);
			_childs.emplace_back(image, std::make_pair(rangeW.first, xMiddle), std::make_pair(yMiddle, rangeH.second), totalWidth);
			_childs.emplace_back(image, std::make_pair(xMiddle, rangeW.second), std::make_pair(yMiddle, rangeH.second), totalWidth);
			_childs.emplace_back(image, std::make_pair(xMiddle, rangeW.second), std::make_pair(rangeH.first, yMiddle), totalWidth);
		} else {
			_color = image[rangeH.first*totalWidth + rangeW.first] == 0 ? Black : White;
		}
	}

	std::vector<Node> _childs;
	Range _rangeW;
	Range _rangeH;
	Color _color;
};

/**
Prints quadtree in required format.
*/
void preorderPrint(const Node& node, std::ostream &out = std::cout) {
	switch (node._color) {
		case Black: out << "f";
		break;
		case White: out << "e";
		break;
		case Gray: out << "s";
		for (auto &child : node._childs) {
			preorderPrint(child, out);
		}
		break;
	}
}

/**
Processes input file into a quadtree using stbi, a single-file public domain libraries for C/C++
*/
void readToQtree(const char *name, std::ostream &output = std::cout) {
    int width, height, bpp;

    uint8_t* image = stbi_load(name, &width, &height, &bpp, 1);

    std::cout<< "Processing file " << name << " with: W:" << width << ", H:" << height << std::endl;

    Node root(image, {0, width}, {0, height}, width);
    preorderPrint(root, output);
    output << std::endl;
    stbi_image_free(image);

}

/**
Traverses sitring until it reaches a parent node. Returns a number of nodes it traversed through.
*/
int rollUp(const std::string &full, int from, std::string &appendTo) {
	if (full[from] != 's') {
		appendTo += full[from];
		return 1;
	}
	appendTo += 's';
	int sum = 1;
	for (int i : {1,2,3,4}) {
		sum += rollUp(full, from + sum, appendTo);
	}
	return sum;
}

/**
Counts intersection of two strings representing two quadTrees
*/
std::string intersect(const std::string &fst, const std::string &snd) {
	int pointInFst = 0, pointInSnd = 0;
	std::string result, trash;

	while (pointInFst < fst.size() || pointInSnd < snd.size()) {
		if (fst[pointInFst] == snd[pointInSnd]) {
			result += fst[pointInFst] ;
			pointInSnd++; pointInFst++;
		} else if (fst[pointInFst] == 'e' || snd[pointInSnd]=='e') {
			result += 'e';
			pointInFst += rollUp(fst, pointInFst, trash);
			pointInSnd += rollUp(snd, pointInSnd, trash);
		} else if (snd[pointInSnd] == 'f' ) {
			pointInFst += rollUp(fst, pointInFst, result);
			pointInSnd += rollUp(snd, pointInSnd, trash);
		} else if (fst[pointInFst] == 'f' ) {
			pointInFst += rollUp(fst, pointInFst, trash);
			pointInSnd += rollUp(snd, pointInSnd, result);
		}
	}
	return result;
}

/**
Counts union of two strings representing two quadTrees
*/
std::string unionQ(const std::string &fst, const std::string &snd) {
	int pointInFst = 0, pointInSnd = 0;
	std::string result, trash;

	while (pointInFst < fst.size() || pointInSnd < snd.size()) {
		if (fst[pointInFst] == snd[pointInSnd]) {
			result += fst[pointInFst] ;
			pointInSnd++; pointInFst++;
		} else if (fst[pointInFst] == 'f' || snd[pointInSnd]=='f') {
			result += 'f';
			pointInFst += rollUp(fst, pointInFst, trash);
			pointInSnd += rollUp(snd, pointInSnd, trash);
		} else if (snd[pointInSnd] == 'e' ) {
			pointInFst += rollUp(fst, pointInFst, result);
			pointInSnd += rollUp(snd, pointInSnd, trash);
		} else if (fst[pointInFst] == 'e' ) {
			pointInFst += rollUp(fst, pointInFst, trash);
			pointInSnd += rollUp(snd, pointInSnd, result);
		}
	}
	return result;
}

/**
Cleans up the resulting string, as for example "seeee" can be reduced to "e", but intersection/union
does not handle this.
*/
void cleanUp(std::string &tree) {

	while(int pos = tree.find("seeee") != std::string::npos) {
		tree.replace(pos-1, 5, "e");
	}

	while(int pos = tree.find("sffff") != std::string::npos) {
		tree.replace(pos-1, 5, "f");
	}
}

/**
Main: identify function based on the input.
*/
int main(int argc, char *argv[]) {

	if (argc == 3) {

		std::ofstream output{argv[2]};
		readToQtree(argv[1], output);
		return 0;

	} else if(argc == 5) {

		std::string input1, input2;
		std::ifstream in1{argv[2]};
		std::ifstream in2{argv[3]};
		std::getline (in1, input1);
		std::getline (in2, input2);
		std::ofstream output{argv[4]};

		std::string option(argv[1]), result;
		if (option == "-intersect" || option == "intersect" ) {
			result = intersect(input1, input2);
		}else if (option == "-union" || option == "union") {
			result = unionQ(input1, input2);
		} else{
			std::cout<< "Invalid option: " << option << std::endl;
			return 1;
		}

		cleanUp(result);
		output << result << std::endl;

	} else {
		std::cout << "Bad usage\nUsage:\n<input> <output> for serialized quadtree\n" 
					<< "-union/intersect <input1> <input2> <output> for union/intersection\n";
		return 1;
	}

	return 0;
}

