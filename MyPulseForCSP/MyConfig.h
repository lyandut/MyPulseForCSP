#pragma once

#include <string>
#include <queue>
#include <map>

namespace lyan {
	using ID = int;
	using Weight = int;
	using Resource = int;
	
	const int INF = 0x3f3f3f3f;
	const int MAX_RSC_NUM = 10;
	const std::string Zhu_Wilhelm_TYPE   = "Zhu_Wilhelm_Testbed";
	const std::string Zhu_Wilhelm_FOLDER = "D:\\My.C++\\MyPulseForCSP\\Instance\\" + Zhu_Wilhelm_TYPE +"\\";
	const std::string Santos_FOLDER      = "D:\\My.C++\\MyPulseForCSP\\Instance\\SantosTestbed\\";
	const std::string SOLUTION_FILE      = "D:\\My.C++\\MyPulseForCSP\\Solution\\log.csv";
	
	template <typename T>
	using PriorityQueue = std::priority_queue<T>;

	static std::map<std::string, Weight> resultMap = {
		{ "rcsp1.txt", 131 },{ "rcsp2.txt", 131 },{ "rcsp3.txt", 2 },{ "rcsp4.txt", 2 },
		{ "rcsp5.txt", 100 },{ "rcsp6.txt", 100 },{ "rcsp7.txt", 6 },{ "rcsp8.txt", 14 },
		{ "rcsp9.txt", 420 },{ "rcsp10.txt", 420 },{ "rcsp11.txt", 6 },{ "rcsp12.txt", 6 },
		{ "rcsp13.txt", 448 },{ "rcsp14.txt", INF },{ "rcsp15.txt", 9 },{ "rcsp16.txt", 17 },
		{ "rcsp17.txt", 652 },{ "rcsp18.txt", 652 },{ "rcsp19.txt", 6 },{ "rcsp20.txt", 6 },
		{ "rcsp21.txt", 858 },{ "rcsp22.txt", 858 },{ "rcsp23.txt", 4 },{ "rcsp24.txt", 5 }
	};
}