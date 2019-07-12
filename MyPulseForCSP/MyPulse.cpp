#include "MyPulse.h"

MyPulse::MyPulse(ID src, ID dst, AdjList<ID, Weight, Resource> &adjList, List<Resource> &max_capacity)
	: src(src), dst(dst), adjList(adjList), max_capacity(max_capacity) {
	node_num = int(adjList.size());
	resource_num = int(max_capacity.size());
	dual_bound = INF;
	primal_bound = 0;
	visited_label.resize(node_num);
}

bool MyPulse::checkDominance(ID curr, Weight cost, List<Resource>& consumptions) {
	return true;
}

bool MyPulse::checkFeasibility(ID curr, List<Resource> &consumptions) {
	for (ID rsc_id = 0; rsc_id < resource_num; ++rsc_id) {
		if (consumptions[rsc_id] + lower_bound_consumptions[curr][rsc_id] > max_capacity[rsc_id]) { return false; }
	}
	return true;
}

bool MyPulse::checkBounds(ID curr, Weight cost) {
	if (cost + lower_bound_cost[curr] >= primal_bound) { return false; }
	return true;
}

void MyPulse::pulseProcedure(ID curr, Weight cost, List<Resource> &consumptions, Path<ID, Weight> &path) {
	if (!checkDominance(curr, cost, consumptions) || !checkFeasibility(curr, consumptions) || !checkBounds(curr, cost)) { return; }
	path.nodes.push_back(curr);
	Path<ID, Weight> opt_path;
	for (auto next : adjList[curr]) {
		if (visited_label[next.dst].vis) { continue; }
		Path<ID, Weight> new_path(path);
		Weight new_cost = cost + next.weight;
		List<Resource> new_consumptions(consumptions);
		for (ID rsc_id = 0; rsc_id < resource_num; ++rsc_id) { new_consumptions[rsc_id] += next.consumptions[rsc_id]; }
		
		visited_label[next.dst].vis = true;
		new_path.distance += next.weight;
		pulseProcedure(next.dst, new_cost, new_consumptions, new_path);
		visited_label[next.dst].vis = false;
		new_path.distance -= next.weight;
	
		if (!new_path.nodes.empty() && new_path.nodes.back() == dst && (opt_path.nodes.empty() || new_path.distance < opt_path.distance)) {
			opt_path = new_path;
		}
	}
	if (path.nodes.back() != dst) { path = opt_path; }
	if (!path.nodes.empty() && path.nodes.back() == dst && path.distance < primal_bound) { primal_bound = path.distance; }
}

void MyPulse::reset_label() {
	for (auto each : visited_label) {
		each.vis = false;
	}
}

void MyPulse::initialization() {
	opt_path = Path<ID, Weight>(0, List<ID>());
	opt_path.nodes.reserve(node_num);
	cumulative_cost = 0;
	cumulative_consumptions.resize(resource_num, 0);

	reverseGraph.resize(node_num);
	for (ID id = 0; id < node_num; ++id) {
		for (auto each : adjList[id]) {
			reverseGraph[each.dst].emplace_back(id, each.weight, each.consumptions);
		}
	}
	dijkstra(dst);
}

void MyPulse::dijkstra(ID start) {

}

void MyPulse::run() {
	std::cout<<"---------initialization begin---------\n";
	initialization();
	std::cout<<"---------initialization end---------\n";
	
	visited_label[src].vis = true;
	pulseProcedure(src, cumulative_cost, cumulative_consumptions, opt_path);

	std::cout << "min cost: " << opt_path.distance << std::endl;

	std::cout << "optimal path: ";
	for (ID i : opt_path.nodes) { std::cout << i << " "; }
	std::cout << std::endl;
}





























