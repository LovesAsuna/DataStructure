#include <iostream>
#include <string>
#include "graph/GraphMatrix.h"

using namespace std;

struct village {
    // 标识符
    int id{};
    // 到过的村庄数
    int count{};
    // 路径
    string path;

    bool operator>=(village &v) const {
        return this->count >= v.count;
    }

    bool operator<=(village &v) const {
        return this->count <= v.count;
    }

    friend ostream &operator<<(ostream &o, village &v) {
        o << v.id;
        return o;
    }

};

int tsp() {
    int n, m;
    cin >> n >> m;
    if (n == 1 || m == 0) {
        cout << 1;
        return 0;
    }
    // 0度村庄
    auto *zero = new village[n];
    // 0度村庄计数值
    int zeroNum = 0;
    GraphMatrix<village, int> graph;
    // 插入村庄顶点
    for (int i = 0; i < n; i++) {
        village v = {i, 1, to_string(i)};
        graph.insert(v);
    }
    // 插入连边
    for (int i = 0; i < m; i++) {
        int start, end;
        cin >> start >> end;
        graph.insert(1, 0, start - 1, end - 1);
    }

    // 记录所有入度为0的点
    for (int i = 0; i < graph.n; i++) {
        if (graph.inDegree(i) == 0) {
            zero[zeroNum++] = graph.vertex(i);
        }
    }

    int max = 0;
    Queue<village> Q;
    Q.init();
    // 遍历所有入度为0的点
    for (int i = 0; i < zeroNum; i++) {
        // 取出顶点
        village &res = zero[i];
        // 遍历此顶点的所有邻居结点
        // 初始化
        Q.enqueue(res);
        // 反复地
        while (!Q.isEmpty()) {
            village v = Q.dequeue();
            // 取出队列首顶点
            // 并考察v的每一邻居u
            for (int u = graph.firstNbr(v.id); -1 < u; u = graph.nextNbr(v.id, u)) {
                // 更新结点数据(将到过的村庄数+1)
                village &uVillage = graph.vertex(u);

                // 只要v.count + 1即可打印全部路径
                if (uVillage.count <= v.count) {
                    uVillage.count = v.count + 1;
                    res.path = res.path + uVillage.path;
                    if (uVillage.count > max) {
                        max = uVillage.count;
                    }
                }
                Q.enqueue(uVillage);
            }
        }
        Q.clear();
    }
    cout << max << endl << endl;
    for (int i = 0; i < zeroNum; i++) {
        cout << zero[i].path << endl;
    }
    return 0;
}