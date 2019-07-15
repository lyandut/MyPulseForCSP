#include "rcspp.h"
#include "MyPulse.h"
#include <ctime>

template<typename ID, typename Weight, typename Resource>
bool resourceConstrainedShortestPath(Path<ID, Weight> &path, const AdjList<ID, Weight, Resource> &adjList, ID src, ID dst, const List<Resource> &capacity) {
	
	MyPulse myPulse(src, dst, adjList, capacity);

	double duration;
	std::clock_t start = std::clock();
	myPulse.run();
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "duration: " << duration << std::endl;

	if (myPulse.opt_path.nodes.empty() || myPulse.opt_path.nodes.back() != dst) { return false; }
	path = myPulse.opt_path;
	return true;
}

template bool resourceConstrainedShortestPath<ID, Weight, Resource>(Path<ID, Weight> &path, const AdjList<ID, Weight, Resource> &adjList, ID src, ID dst, const List<Resource> &capacity);