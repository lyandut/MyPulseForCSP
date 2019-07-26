#include "MyPulse.h"
#include <thread>

MyPulse::MyPulse(ID src, ID dst, const AdjList<ID, Weight, Resource> &adjList, const List<Resource> &max_capacity)
	: src(src), dst(dst), adjList(adjList), max_capacity(max_capacity) {
	node_num = int(adjList.size());
	resource_num = int(max_capacity.size());
	primal_bound = INF;
	visited.resize(node_num, 0);
	adjSortedIndex.resize(node_num);
	for (ID id = 0; id < node_num; ++id) {
		adjSortedIndex[id].resize(adjList[id].size());
		for (ID index = 0; index < adjList[id].size(); ++index) { 
			adjSortedIndex[id][index] = index;
		}
	}
}

bool MyPulse::compareResources(List<Resource>& r1, List<Resource>& r2) {
	for (ID rsc_id = 0; rsc_id < resource_num; ++rsc_id) {
		if (r1[rsc_id] > r2[rsc_id]) { return true; }
	}
	return false;
}

void MyPulse::changeLabels(ID curr, Weight cost, List<Resource>& consumptions) {
	// Stores the best cost
	if (cost < dominance_labels[curr][0].cost) {
		dominance_labels[curr][0].cost = cost;
		dominance_labels[curr][0].resource = consumptions;
	}
	// Stores the best consumptions
	else if (compareResources(dominance_labels[curr][1].resource, consumptions)) {
		dominance_labels[curr][1].cost = cost;
		dominance_labels[curr][1].resource = consumptions;
	}
	// Replaces the third label
	else {
		dominance_labels[curr][2].cost = cost;
		dominance_labels[curr][2].resource = consumptions;
	}
}

//bool MyPulse::checkDominance(ID curr, Weight cost, List<Resource>& consumptions) {
//	if ((cost >= dominance_labels[curr][0].cost && !compareResources(dominance_labels[curr][0].resource, consumptions))
//	||  (cost >= dominance_labels[curr][1].cost && !compareResources(dominance_labels[curr][1].resource, consumptions))
//	||  (cost >= dominance_labels[curr][2].cost && !compareResources(dominance_labels[curr][2].resource, consumptions))
//	) { return false; }
//	return true;
//}

bool MyPulse::checkDominance(ID curr, Weight cost, List<Resource>& consumptions) {
	if (cost < dominance_labels[curr][0].cost || compareResources(dominance_labels[curr][0].resource, consumptions)
	&& cost < dominance_labels[curr][1].cost || compareResources(dominance_labels[curr][1].resource, consumptions)
	&& cost < dominance_labels[curr][2].cost || compareResources(dominance_labels[curr][2].resource, consumptions)
	) { return true; }
	return false;
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
	changeLabels(curr, cost, consumptions);
	path.nodes.push_back(curr);
	Path<ID, Weight> opt_path; opt_path.distance = INF;
	for (ID index : adjSortedIndex[curr]) {
		auto &next = adjList[curr][index];
		if (visited[next.dst]) { continue; }

		Weight new_cost = cost + next.weight;
		List<Resource> new_consumptions(consumptions);
		for (ID rsc_id = 0; rsc_id < resource_num; ++rsc_id) { new_consumptions[rsc_id] += next.consumptions[rsc_id]; }
		Path<ID, Weight> new_path(path);
		new_path.distance += next.weight;

		visited[next.dst] = 1;
		pulseProcedure(next.dst, new_cost, new_consumptions, new_path);
		visited[next.dst] = 0;
	
		if (!new_path.nodes.empty() && new_path.nodes.back() == dst && new_path.distance < opt_path.distance) { 
			opt_path = new_path; 
		}
	}
	if (path.nodes.back() != dst) { path = opt_path; }
	if (!path.nodes.empty() && path.nodes.back() == dst && path.distance < primal_bound) { 
		primal_bound = path.distance;
		//std::cout << "new primal bound: " << primal_bound << std::endl;
	}
}

void MyPulse::initialization() {
	opt_path.distance = 0;
	opt_path.nodes.reserve(node_num);

	reverseGraph.resize(node_num);
	for (ID id = 0; id < node_num; ++id) {
		for (auto &each : adjList[id]) {
			reverseGraph[each.dst].emplace_back(id, each.weight, each.consumptions);
		}
	}
	dominance_labels.resize(node_num, List<Label>(3, { INF, List<Resource>(resource_num, INF) }));
	lower_bound_cost.resize(node_num, 0);
	lower_bound_consumptions.resize(node_num, List<Resource>(resource_num, 0));

#if THREADS_DIJKSTRA
	List<std::thread> threads;
	for (ID i = -1; i < resource_num; ++i) { threads.emplace_back(&MyPulse::dijkstra, this, dst, i); }
	for (ID i = -1; i < resource_num; ++i) { threads[i + 1].join(); }
#else
	for (ID i = -1; i < resource_num; ++i) { dijkstra(dst, i); }
#endif

#if SORT_ADJLIST_CUR
	for (ID id = 0; id < node_num; ++id) {
		std::sort(adjSortedIndex[id].begin(), adjSortedIndex[id].end(),
			[id, this](const ID index1, const ID index2) {
				return adjList[id][index1].weight < adjList[id][index2].weight;
			}
		);
	}
#elif SORT_ADJLIST_DST
	for (ID id = 0; id < node_num; ++id) {
		std::sort(adjSortedIndex[id].begin(), adjSortedIndex[id].end(),
			[id, this](const ID index1, const ID index2) {
				ID node1 = adjList[id][index1].dst, node2 = adjList[id][index2].dst;
				return lower_bound_cost[node1] < lower_bound_cost[node2];
			}
		);
	}
#endif
}

void MyPulse::dijkstra(ID s, ID rsc_id) {
	List<ID> predecessor(node_num);
	List<Vertex> vertexes(node_num);
	for (ID i = 0; i < node_num; ++i) {
		vertexes[i].id = i;
		vertexes[i].dist = INF;
	}
	PriorityQueue<Vertex> queue;
	vertexes[s].dist = 0;
	queue.push(vertexes[s]);
	while (!queue.empty()) {
		Vertex minVertex = queue.top(); queue.pop();
		for(auto &e : reverseGraph[minVertex.id]) {
			Vertex &nextVertex = vertexes[e.dst];
			auto new_dist = rsc_id < 0 ? (minVertex.dist + e.weight) : (minVertex.dist + e.consumptions[rsc_id]);
			if (new_dist < nextVertex.dist) {
				nextVertex.dist = new_dist;
				predecessor[nextVertex.id] = minVertex.id;
				queue.push(nextVertex);
			}
		}
	}
	if (rsc_id < 0) {
		for (ID i = 0; i < node_num; ++i) { lower_bound_cost[i] = vertexes[i].dist; }
	}
	else {
		for (ID i = 0; i < node_num; ++i) { lower_bound_consumptions[i][rsc_id] = vertexes[i].dist; }
	}
}

void MyPulse::run() {
	//std::cout<<"---------initialization begin---------\n";
	initialization();
	//std::cout<<"---------initialization end---------\n";
	visited[src] = 1;
	pulseProcedure(src, 0, List<Resource>(resource_num, 0), opt_path);
}

void MyPulse::run(Weight result) {
	run();

	std::cout << "ref cost: " << result << std::endl;
	std::cout << "opt cost: " << opt_path.distance << std::endl;
	std::cout << "optimal path: ";
	for (ID i : opt_path.nodes) { std::cout << i << " "; }
	std::cout << std::endl;
}
