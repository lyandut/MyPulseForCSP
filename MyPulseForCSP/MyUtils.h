#pragma once

#include <io.h>

void renameInstance() {
	std::string folder = Zhu_Wilhelm_FOLDER + "*.*";
	_finddata_t file_data;
	auto handler = _findfirst(folder.c_str(), &file_data);
	if (handler == -1) {
		std::cout << folder << " not found!" << std::endl;
		return;
	}
	while (_findnext(handler, &file_data) == 0) {
		if (strcmp(file_data.name, ".") == 0 || strcmp(file_data.name, "..") == 0) { continue; }
		if (strstr(file_data.name, Zhu_Wilhelm_TYPE.c_str())) { continue; }

		std::string filename = Zhu_Wilhelm_FOLDER + file_data.name;
		std::ifstream file(filename);
		if (!file.is_open()) {
			std::cout << filename << " open failed!" << std::endl;
			return;
		}
		std::string line;
		std::istringstream iss;
		std::string first_str;

		int node_num, edge_num, resource_num, tmp, max_capacity;
		ID src, dst;
		List<Resource> capacity;
		std::getline(file, line); iss.str(line);
		iss >> first_str;
		bool testedFlag = first_str.compare("num_nodes") ? true : false;

		if (testedFlag) {
			node_num = std::stoi(first_str);
			iss >> edge_num >> resource_num;
			src = 0; dst = node_num - 1;
			std::getline(file, line); iss.clear(); iss.str(line);
			capacity.resize(resource_num);
			for (int i = 0; i < resource_num; ++i) { iss >> capacity[i]; }
		}
		else {
			std::getline(file, line); iss.clear(); iss.str(line);
			iss >> node_num >> edge_num >> src >> dst;
			std::getline(file, line);
			std::getline(file, line); iss.clear(); iss.str(line);
			while (iss >> tmp) { capacity.push_back(tmp); }
			resource_num = int(capacity.size());
		}
		max_capacity = *std::max_element(capacity.begin(), capacity.end());
		file.close();

		std::string old_file_name = Zhu_Wilhelm_FOLDER + file_data.name;
		std::string new_file_name = Zhu_Wilhelm_FOLDER + Zhu_Wilhelm_TYPE
			+ ".n" + std::to_string(node_num) + "e" + std::to_string(edge_num) 
			+ "c" + std::to_string(max_capacity) + "." + file_data.name;
		std::cout << old_file_name << std::endl;
		std::cout << new_file_name << std::endl;

		if (rename(old_file_name.c_str(), new_file_name.c_str()) == -1) { 
			std::cout << old_file_name << " rename failed!" << std::endl; 
		}
	}
	_findclose(handler);
}
