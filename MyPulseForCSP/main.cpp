#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include "rcspp.h"
#include "MyPulse.h"
#include "MyConfig.h"

using namespace lyan;

bool reader(std::string &filename, AdjList<ID, Weight, Resource> &adjList, List<Resource> &max_capacity) {
	std::ifstream file(filename);
	if (!file.is_open()) { 
		std::cout << filename << " open failed!" << std::endl;
		return false; 
	}
	std::string line;
	std::istringstream iss;
	
	int node_num, edge_num, resource_num;
	std::getline(file, line);
	iss.str(line);
	iss >> node_num >> edge_num >> resource_num;
	
	max_capacity.resize(resource_num);
	std::getline(file, line);
	iss.str(line);
	for (int i = 0; i < resource_num; ++i) { iss >> max_capacity[i]; }
	
	std::getline(file, line);
	
	adjList.resize(node_num);
	ID tail, head; 
	Weight distance;
	List<Resource>  rcsConsumedList(resource_num);
	for (int i = 0; i < edge_num; ++i) {
		if (line.size() < resource_num + 3) { break; }
		std::getline(file, line);
		iss.str(line);
		iss >> tail >> head >> distance;
		for (int j = 0; j < resource_num; ++j) { iss >> rcsConsumedList[j]; }
		adjList[tail - 1].emplace_back(head - 1, distance, rcsConsumedList);
	}
	return true;
}

void runAllInstance() {
	for (auto each : resultMap) {

		std::string instance(each.first);
		std::string filename = std::string("../Instance/Zhu-Wilhelm-Testbed/") + instance;

		AdjList<ID, Weight, Resource> adjList;
		List<Resource> max_capacity;
		if (!reader(filename, adjList, max_capacity)) { continue; }

		int node_num = int(adjList.size());
		MyPulse myPulse(0, node_num - 1, adjList, max_capacity);

		double duration;
		std::clock_t start = std::clock();
		myPulse.run(resultMap[instance]);
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		std::cout << instance + " duration: " << duration << std::endl;
	}
}


int main(int argc, char *argv[]) {

	if (argc == 1) { 
		runAllInstance(); 
	} 
	else {
		std::string instance = argv[1];
		std::string filename = std::string("../Instance/Zhu-Wilhelm-Testbed/") + instance;

		Path<ID, Weight> opt_path;
		AdjList<ID, Weight, Resource> adjList;
		List<Resource> max_capacity;
		reader(filename, adjList, max_capacity);
		ID src = 0, dst = int(adjList.size()) - 1;
		
		if (resourceConstrainedShortestPath(opt_path, adjList, src, dst, max_capacity)) {
			std::cout << "opt cost: " << opt_path.distance << std::endl;
			std::cout << "optimal path: ";
			for (ID i : opt_path.nodes) { std::cout << i << " "; }
			std::cout << std::endl;
		}
		else { std::cout << src << "->" << dst << " has no shortest path!" << std::endl; }
	}

	system("PAUSE");
	return 0;
}
