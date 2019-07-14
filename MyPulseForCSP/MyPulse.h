#pragma once

#include <iostream>
#include "rcspp.h"
#include "MyConfig.h"

using namespace lyan;

struct Vertex {
public:
	ID id;
	Weight dist;
	Vertex() { }
	Vertex(ID id, Weight dist) : id(id), dist(dist) { }
	bool operator < (const Vertex &v) const { return dist > v.dist; }
};

class MyPulse {
private:
	int node_num, resource_num;
	ID src, dst;
	AdjList<ID, Weight, Resource> adjList;
	List<Resource> max_capacity;
	Weight primal_bound;
	
	Path<ID, Weight> opt_path;
	Weight cumulative_cost;
	List<Resource> cumulative_consumptions;
	
	AdjList<ID, Weight, Resource> reverseGraph;
	// `lower_bound_consumptions[n][r]` is the lower bounds on the consumption of resource `r` in node `n`.
	List<List<Resource>> lower_bound_consumptions;
	// `lower_bound_cost[n]` is the dual bound on the real minimum cost of node `n`.
	List<Weight> lower_bound_cost;
	
	void initialization();
	void dijkstra(ID s, ID rsc_id);

	void pulseProcedure(ID curr, Weight cost, List<Resource> &consumptions, Path<ID, Weight> &path);
	bool checkDominance(ID curr, Weight cost, List<Resource> &consumptions);
	bool checkFeasibility(ID curr, List<Resource> &consumptions);
	bool checkBounds(ID curr, Weight cost);

public:
	MyPulse(ID src, ID dst, AdjList<ID, Weight, Resource> &adjList, List<Resource> &max_capacity);
	void run(Weight result);
};













