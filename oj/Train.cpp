#include <iostream>
#include <string>
#include "LinearList/Stack.h"

using namespace std;

// 1 2 3 4 5
// 3 1 2 4 5 不合理
// 3 2 1 4 5 合理

void print(Stack<int> &A, Stack<int> &tmp, Stack<int> &B) {
    cout << "A" << A << endl;
    cout << "S" << tmp << endl;
    cout << "B" << B << endl << endl;
}

int train() {
    int n, m;
    cin >> n >> m;
    if (n < 1 || m < 0) {
        cout << "No" << endl;
        exit(0);
    }
    Stack<int> tmp(m), A(n), B(n);
    int num[n];
    string op[2 * n];
    for (int i = 0; i < n; i++) {
        cin >> num[i];
        int value = n - i;
        A.push(value);
    }

    int i = 0;
    int opi = 0;
    while (!A.isEmpty()) {
        if (tmp.size() >= m) {
            cout << "No" << endl;
            exit(0);
        }
        // 如果中间栈没满
        // 获取A的顶部
        int Atop = A.top();
        // 如果A顶部小于num
        if (Atop < num[i]) {
            // 将顶部压入中间栈
            while (Atop < num[i]) {
                tmp.push(Atop);
                A.pop();
                Atop = A.top();
                op[opi++] = "push";
                print(A, tmp, B);
            }
            // 保持i的位置为了下次相等的判断
            continue;
        } else if (Atop == num[i]) {
            B.push(Atop);
            A.pop();
            op[opi++] = "push";
            op[opi++] = "pop";
            print(A, tmp, B);
        } else {
            // A的顶部大于num
            // 处理中间栈

            // 如果中间栈不为空
            if (!tmp.isEmpty()) {
                int tmpTop = tmp.top();
                // 如果顶部大于num，不合理
                if (tmpTop > num[i]) {
                    cout << "No" << endl;
                    exit(0);
                } else {
                    // 顶部小于或等于num
                    // 将中间栈元素压入B
                    while (tmpTop <= num[i] && !tmp.isEmpty()) {
                        B.push(tmpTop);
                        tmp.pop();
                        tmpTop = tmp.top();
                        i++;
                        op[opi++] = "pop";
                        print(A, tmp, B);
                    }
                    continue;
                }
            } else {
                op[opi++] = "push";
                tmp.push(Atop);
                A.pop();
                print(A, tmp, B);
            }

        }
        i++;
    }

    while (!tmp.isEmpty()) {
        int tmpTop = tmp.top();
        B.push(tmpTop);
        tmp.pop();
        op[opi++] = "pop";
        print(A, tmp, B);
    }

    for (int j = 0; j < 2 * n; j++) {
        cout << op[j] << endl;
    }
    return 0;
}