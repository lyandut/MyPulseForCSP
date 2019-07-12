#include <sstream>
#include <fstream>
#include <ctime>
#include "MyPulse.h"
#include "MyConfig.h"

using namespace lyan;

void reader(std::string &filename, AdjList<ID, Weight, Resource> &adjList, List<Resource> &max_capacity) {
	std::ifstream file(filename);
	if (!file.is_open()) { 
		std::cout << filename << " open failed!" << std::endl;
		return; 
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
}


int main(int args, char *argv[]) {
	//std::string filename = argv[1];
	std::string filename = "../Instance/Zhu-Wilhelm-Testbed/rcsp7.txt";
	AdjList<ID, Weight, Resource> adjList;
	List<Resource> max_capacity;
	reader(filename, adjList, max_capacity);

	int node_num = int(adjList.size());
	int resource_num = int(max_capacity.size());
	MyPulse myPulse(0, node_num - 1, adjList, max_capacity);
	////model.G.show_graph(); model.print_para();

	//double duration;
	//std::clock_t start = std::clock();
	//model.espprc();
	//duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	//std::cout << "duration: " << duration << std::endl;
	
	system("PAUSE");
	return 0;
}
