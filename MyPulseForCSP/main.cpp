#include <iostream>
#include <iomanip> 
#include <sstream>
#include <fstream>
#include <ctime>
#include <windows.h>
#include "rcspp.h"
#include "MyPulse.h"
#include "MyConfig.h"
#include "MyUtils.h"

using namespace lyan;

bool reader(std::string filename, AdjList<ID, Weight, Resource> &adjList, List<Resource> &max_capacity, ID &src, ID &dst) {
	std::ifstream file(filename);
	if (!file.is_open()) { 
		std::cout << filename << " open failed!" << std::endl;
		return false; 
	}
	std::string line;
	std::istringstream iss;
	std::string first_str;

	int node_num, edge_num, resource_num, tmp;
	std::getline(file, line); iss.str(line);
	iss >> first_str;
	bool testedFlag = first_str.compare("num_nodes") ? true : false;

	if (testedFlag) { 
		node_num = std::stoi(first_str);
		iss >> edge_num >> resource_num;
		src = 0; dst = node_num - 1;
		std::getline(file, line); iss.clear(); iss.str(line);
		max_capacity.resize(resource_num);
		for (int i = 0; i < resource_num; ++i) { iss >> max_capacity[i]; }
	}
	else {
		std::getline(file, line); iss.clear(); iss.str(line);
		iss >> node_num >> edge_num >> src >> dst;
		std::getline(file, line);
		std::getline(file, line); iss.clear(); iss.str(line);
		max_capacity.reserve(MAX_RSC_NUM);
		while (iss >> tmp) { max_capacity.push_back(tmp); }
		resource_num = int(max_capacity.size());
	}
	
	std::getline(file, line);
	adjList.resize(node_num);
	ID tail, head; 
	Weight distance;
	List<Resource>  rcsConsumedList(resource_num);
	for (int i = 0; i < edge_num; ++i) {
		std::getline(file, line); iss.clear(); iss.str(line);
		iss >> tail >> head >> distance;
		for (int j = 0; j < resource_num; ++j) { iss >> rcsConsumedList[j]; }
		if (testedFlag) { adjList[tail - 1].emplace_back(head - 1, distance, rcsConsumedList); }
		else { adjList[tail].emplace_back(head, distance, rcsConsumedList); }
	}
	return true;
}

void runAllInstance() {
	std::ofstream logFile(SOLUTION_FILE, std::ios::app);
	std::string folder = INSTANCE_FOLDER + "*.*";
	_finddata_t file_data;
	auto handler = _findfirst(folder.c_str(), &file_data);
	if (handler == -1) {
		std::cout << INSTANCE_FOLDER << " not found!" << std::endl;
		return;
	}
	while (_findnext(handler, &file_data) == 0) {
		if (strcmp(file_data.name, ".") == 0 || strcmp(file_data.name, "..") == 0) { continue; }

		std::string filename = INSTANCE_FOLDER + file_data.name;

		AdjList<ID, Weight, Resource> adjList;
		List<Resource> max_capacity;
		ID src, dst;
		if (!reader(filename, adjList, max_capacity, src, dst)) { continue; }

		MyPulse myPulse(src, dst, adjList, max_capacity);

		LARGE_INTEGER nFreq;
		LARGE_INTEGER t1;
		LARGE_INTEGER t2;
		double duration;
		QueryPerformanceFrequency(&nFreq);
		//std::clock_t start = std::clock();
		QueryPerformanceCounter(&t1);
		myPulse.run();
		QueryPerformanceCounter(&t2);
		//duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		duration = (t2.QuadPart - t1.QuadPart) / (double)nFreq.QuadPart;

		std::cout << file_data.name << " duration: "  << duration << std::endl;

		// record solution
		std::ostringstream log;
		log << file_data.name << "," << myPulse.opt_path.distance << "," 
			<< std::setiosflags(std::ios::fixed) << std::setprecision(5) << duration << ",";
		for (ID i : myPulse.opt_path.nodes) { log << i << " "; }
		log << std::endl;

		logFile.seekp(0, std::ios::end);
		if (logFile.tellp() <= 0) { logFile << "Instance,OptCost,Duration,Solution" << std::endl; }
		logFile << log.str();
		
	}
	_findclose(handler);
	logFile.close();
}


int main(int argc, char *argv[]) {

	if (argc == 1) { 
		runAllInstance(); 
		//renameInstance();
	} 
	else {
		std::string instance = argv[1];
		std::string filename = std::string("../Instance/Zhu_Wilhelm_Testbed/") + instance;

		Path<ID, Weight> opt_path;
		AdjList<ID, Weight, Resource> adjList;
		List<Resource> max_capacity;
		ID src, dst;
		reader(filename, adjList, max_capacity, src, dst);
		
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
