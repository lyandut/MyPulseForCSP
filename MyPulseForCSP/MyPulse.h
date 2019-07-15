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

struct Label {
public:
	Weight cost;
	List<Resource> resource;
};

class MyPulse {
private:
	int node_num, resource_num;
	ID src, dst;
	AdjList<ID, Weight, Resource> adjList;
	List<Resource> max_capacity;
	Weight primal_bound;
	
	Path<ID, Weight> opt_path;
	
	AdjList<ID, Weight, Resource> reverseGraph;
	/* 
	*  Pruning by dominance
	* `dominance_labels[n][0]` is overwritten every time that a partial path exhibits a lower cost than the one stored, 
	* `dominance_labels[n][1]` is overwritten every time that a partial path exhibits a lower resource consumption than the one stored,
	* `dominance_labels[n][2]` is replaced randomly.
	*/
	List<List<Label>> dominance_labels;
	/* 
	*  Pruning by infeasibility
	* `lower_bound_consumptions[n][r]` is the lower bounds on the consumption of resource `r` in node `n`.
	*/
	List<List<Resource>> lower_bound_consumptions;
	/* 
	*  Pruning by bounds
	* `lower_bound_cost[n]` is the dual bound on the real minimum cost of node `n`.
	*/
	List<Weight> lower_bound_cost;
	
	void initialization();
	void dijkstra(ID s, ID rsc_id);

	void pulseProcedure(ID curr, Weight cost, List<Resource> &consumptions, Path<ID, Weight> &path);
	
	// return true if `r2` is dominated by `r1`.
	bool compareResources(List<Resource>& r1, List<Resource>& r2);
	void changeLabels(ID curr, Weight cost, List<Resource> &consumptions);
	
	bool checkDominance(ID curr, Weight cost, List<Resource> &consumptions);
	bool checkFeasibility(ID curr, List<Resource> &consumptions);
	bool checkBounds(ID curr, Weight cost);

public:
	MyPulse(ID src, ID dst, AdjList<ID, Weight, Resource> &adjList, List<Resource> &max_capacity);
	void run(Weight result);
};
