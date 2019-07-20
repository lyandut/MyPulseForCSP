#include <iostream>
#include <iomanip> 
#include <sstream>
#include <fstream>
#include <ctime>
#include <cmath>
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

void runZhuWilhelm() {
	std::ofstream logFile(SOLUTION_FILE, std::ios::app);
	std::string folder = Zhu_Wilhelm_FOLDER + "*.*";
	_finddata_t file_data;
	auto handler = _findfirst(folder.c_str(), &file_data);
	if (handler == -1) {
		std::cout << Zhu_Wilhelm_FOLDER << " not found!" << std::endl;
		return;
	}
	while (_findnext(handler, &file_data) == 0) {
		if (strcmp(file_data.name, ".") == 0 || strcmp(file_data.name, "..") == 0) { continue; }

		std::string filename = Zhu_Wilhelm_FOLDER + file_data.name;

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
		std::cout << file_data.name << " duration: " << duration << std::endl;

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

// SantosTestbed
void runSantosTestbed(double P) {
	std::ofstream logFile(SOLUTION_FILE, std::ios::app);
	std::string file1_name = Santos_FOLDER + "File1.txt";
	std::string file2_name = Santos_FOLDER + "File2.TXT";
	std::string file3_name = Santos_FOLDER + "File3.TXT";
	std::ifstream file1(file1_name); std::ifstream file2(file2_name); std::ifstream file3(file3_name);
	if (!file1.is_open() || !file2.is_open() || !file3.is_open()) {
		std::cout << "files open failed!" << std::endl;
		return;
	}

	int network_id, network_id_2, network_id_3;
	file2 >> network_id_2;
	file3 >> network_id_3;
	while (file1 >> network_id) { 
		// Read File1.txt 
		// pathA: lower_bound_cost, pathB: lower_bound_consumptions.
		int node_num, edge_num, sum_time_pathA, sum_time_pathB;
		ID src = 1, dst;
		file1 >> node_num >> edge_num >> sum_time_pathA >> sum_time_pathB >> dst;
		List<Resource> max_capacity(1, round(P*(sum_time_pathA - sum_time_pathB)) + sum_time_pathB);

		// Read File2.TXT
		int index = 1;
		List<ID> head(node_num + 2, 0);
		while (network_id_2 == network_id) {
			file2 >> head[index++];
			if (!(file2 >> network_id_2)) { break; }
		}

		// Read File3.TXT
		ID to; Weight cost; Resource time;
		AdjList<ID, Weight, Resource> adjList(node_num + 1);
		int i = 1, i_edge_num = head[i + 1] - head[i];
		while (network_id_3 == network_id) {
			file3 >> to >> cost >> time;
			while (i_edge_num == 0) {
				i += 1;
				i_edge_num = head[i + 1] - head[i];
			}
			adjList[i].emplace_back(to, cost, List<Resource>(1, time));
			i_edge_num--;
			if (!(file3 >> network_id_3)) { break; }
		}

		std::string instance_name = "SantosTestbed" + std::to_string(network_id) 
			+ ".p" + std::to_string(P) + "n" + std::to_string(node_num) 
			+ "e" + std::to_string(edge_num) + "c" + std::to_string(max_capacity[0]);

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
		std::cout << instance_name << " duration: " << duration << std::endl;

		// record solution
		std::ostringstream log;
		log << instance_name << "," << P << "," << myPulse.opt_path.distance << ","
			<< std::setiosflags(std::ios::fixed) << std::setprecision(5) << duration << ",";
		for (ID i : myPulse.opt_path.nodes) { log << i << " "; }
		log << std::endl;

		logFile.seekp(0, std::ios::end);
		if (logFile.tellp() <= 0) { logFile << "Instance,P,OptCost,Duration,Solution" << std::endl; }
		logFile << log.str();
	}
	logFile.close();
}

int main(int argc, char *argv[]) {

	if (argc == 1) { 
		//renameInstance();
		//runZhuWilhelm();
		runSantosTestbed(0.1); // (p = 0.1, 0.2, 0.4, 0.6, and 0.8)
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
