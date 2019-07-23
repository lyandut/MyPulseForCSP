# MyPulseForCSP

CSP: Constrained Shortest Path Problem

C++ implementation of the pulse algorithm as published in: "Lozano, L. and Medaglia, A. L. (2013). On an exact method for the constrained shortest path problem. Computers & Operations Research. 40 (1):378-384."

This paper is available at: http://dx.doi.org/10.1016/j.cor.2012.07.008.



## ToDo List

| #                                                            | 作用                                 | 效果                                                         |
| :----------------------------------------------------------- | ------------------------------------ | ------------------------------------------------------------ |
| - [x] 多线程                                                 | 并行 dijkstra 算下界，提速初始化阶段 | 大算例(SantosTestbed)速度能提升50%左右；小算例(Zhu_Wilhelm)效果不佳，部分速度下降。 |
| - [ ] 调整邻接矩阵顺序                                       | 优化分支顺序，使后续搜索可能提前剪枝 | todo…                                                        |
| - [ ] 初始化阶段 dijkstra 算完路由下界后检查，符合资源约束直接返回最短路。 | 特例：最短路 = 资源约束最短路        | todo…                                                        |

