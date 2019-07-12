#pragma once

#include <iostream>
#include "rcspp.h"
#include "MyConfig.h"

using namespace lyan;

struct Label {
	bool vis;
	Label() : vis(false) { }
};

class MyPulse {
private:
	int node_num, resource_num;
	ID src, dst;
	Path<ID, Weight> opt_path;
	List<Label> visited_label;
	List<Resource> max_capacity;
	List<Resource> cumulative_consumptions;
	AdjList<ID, Weight, Resource> adjList;
	AdjList<ID, Weight, Resource> reverseGraph;
	Weight cumulative_cost;
	Weight primal_bound, dual_bound;
	
	// `lower_bound_consumptions[n][i]` is the lower bounds on the consumption of resource `i` in node `n`.
	List<List<Resource>> lower_bound_consumptions;
	// `lower_bound_cost[n]` is the dual bound on the real minimum cost of node `n`.
	List<Weight> lower_bound_cost;
	
	void reset_label();
	void initialization();
	void dijkstra(ID start);

	void pulseProcedure(ID curr, Weight cost, List<Resource> &consumptions, Path<ID, Weight> &path);
	bool checkDominance(ID curr, Weight cost, List<Resource> &consumptions);
	bool checkFeasibility(ID curr, List<Resource> &consumptions);
	bool checkBounds(ID curr, Weight cost);

public:
	MyPulse(ID src, ID dst, AdjList<ID, Weight, Resource> &adjList, List<Resource> &max_capacity);
	void run();
};













