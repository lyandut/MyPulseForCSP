#pragma once

#include <queue>

namespace lyan {
	using ID = int;
	using Weight = int;
	using Resource = int;
	const int INF = 0x3f3f3f3f;
	
	template <typename T>
	using PriorityQueue = std::priority_queue<T>;
}