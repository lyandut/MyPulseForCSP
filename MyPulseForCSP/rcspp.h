#include <vector>


template<typename T>
using List = std::vector<T>;

template<typename ID, typename Weight, typename Resource>
struct AdjNode {
  ID dst; // dst of the edge.
  Weight weight; // length of the edge.
  List<Resource> consumptions;
  AdjNode() { }
  AdjNode(ID d, Weight w, List<Resource> &c) : dst(d), weight(w), consumptions(c) { }
};

// `adjList[n][i]` is the `i`_th adjacent node of node `n`.
template<typename ID, typename Weight, typename Resource>
using AdjList = List<List<AdjNode<ID, Weight, Resource>>>;

template<typename ID, typename Weight>
struct Path {
    Weight distance;
    List<ID> nodes; // `nodes[i]` is the `i`_th node visited in the tour.
	Path() { }
	Path(Weight d, List<ID> &n) : distance(d), nodes(n) { }
};


// the total resource consumption on the resulting `path` from `src` to `dst` should be less than `capacity`.
// return false if there is no feasible path between `src` and `dst`.
template<typename ID, typename Weight, typename Resource>
bool resourceConstrainedShortestPath(Path<ID, Weight> &path, const AdjList<ID, Weight, Resource> &adjList, ID src, ID dst, List<Resource> capacity);
