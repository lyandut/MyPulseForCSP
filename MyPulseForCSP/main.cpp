#include <sstream>
#include <fstream>
#include <ctime>
#include <string>
#include <map>
#include "MyPulse.h"
#include "MyConfig.h"

using namespace lyan;

static std::map<std::string, Weight> resultMap = { 
	{ "rcsp1.txt", 131 },{ "rcsp2.txt", 131 },{ "rcsp3.txt", 2 },{ "rcsp4.txt", 2 },
	{ "rcsp5.txt", 100 },{ "rcsp6.txt", 100 },{ "rcsp7.txt", 6 },{ "rcsp8.txt", 14 },
	{ "rcsp9.txt", 420 },{ "rcsp10.txt", 420 },{ "rcsp11.txt", 6 },{ "rcsp12.txt", 6 },
	{ "rcsp13.txt", 448 }, { "rcsp14.txt", INF }, { "rcsp15.txt", 9 }, { "rcsp16.txt", 17 },
	{ "rcsp17.txt", 652 }, { "rcsp18.txt", 652 }, { "rcsp19.txt", 6 }, { "rcsp20.txt", 6 },
	{ "rcsp21.txt", 858 }, { "rcsp22.txt", 858 }, { "rcsp23.txt", 4 }, { "rcsp24.txt", 5 }
};

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


int main(int args, char *argv[]) {
	
	runAllInstance();
	
	//std::string instance = argv[1];
	//std::string instance("rcsp23.txt");
	//std::string filename = std::string("../Instance/Zhu-Wilhelm-Testbed/") + instance;

	//AdjList<ID, Weight, Resource> adjList;
	//List<Resource> max_capacity;
	//reader(filename, adjList, max_capacity);

	//int node_num = int(adjList.size());
	//MyPulse myPulse(0, node_num - 1, adjList, max_capacity);

	//double duration;
	//std::clock_t start = std::clock();
	//myPulse.run(resultMap[instance]);
	//duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	//std::cout << instance + " duration: " << duration << std::endl;

	system("PAUSE");
	return 0;
}
