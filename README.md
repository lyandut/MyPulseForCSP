# MyPulseForCSP

CSP: Constrained Shortest Path Problem

C++ implementation of the pulse algorithm as published in: "Lozano, L. and Medaglia, A. L. (2013). On an exact method for the constrained shortest path problem. Computers & Operations Research. 40 (1):378-384."

This paper is available at: http://dx.doi.org/10.1016/j.cor.2012.07.008.



## ToDo List

| #                                                            | 目的                                 | 求解效果                                                     |
| :----------------------------------------------------------- | ------------------------------------ | ------------------------------------------------------------ |
| :white_check_mark: ​多线程                                    | 并行 dijkstra 算下界，提速初始化阶段 | Zhu_Wilhelm 效果不佳，部分算例速度下降；SantosTestbed 速度能提升50%左右。 |
| :white_check_mark: 调整邻接矩阵顺序（优先考虑距离当前节点近的） | 优化分支顺序，使后续搜索可能提前剪枝 | Zhu_Wilhelm 效果不明显；SantosTestbed 速度较多线程版本继续有小幅度提升。 |
| :white_large_square: 调整邻接矩阵顺序（优先考虑距离终点近的） | 优化分支顺序，使后续搜索可能提前剪枝 | Zhu_Wilhelm 效果不明显；SantosTestbed 速度表现不稳定，个别算例会一直递归... |
| :white_large_square: 初始化阶段 dijkstra 算完路由下界后检查，符合资源约束直接返回最短路。 | 特例：最短路 = 资源约束最短路        | todo…                                                        |



## 算例 & 结果

- Instance/  ···  算例文件
  - Zhu_Wilhelm_Testbed/  ···  12个算例（最大规模500个点，5000±条边，10种资源）
  - Zhu_Wilhelm_DIMACS/  ···  基于 Zhu_Wilhelm_Testbed 生成的 1200 个算例
  - SantosTestbed/  ···  包含1050个算例，文件过大未同步（最大规模40000个点，800000条边，1种资源）
- Solution/  ···  计算结果