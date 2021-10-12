#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;
char s[20030];

int solve(int pos, char c, int len) {
    ///插入操作
    for (int i = len; i > pos; i--)
        s[i] = s[i - 1];
    s[pos] = c;
    len++;
    ///消除操作
    for (int i = 0; i < len - 2; i++)///扫描字符串
    {
        if (s[i] == s[i + 1] && s[i + 1] == s[i + 2]) {
            int r, sub;
            for (r = i + 2; r < len && s[r] == s[i]; r++);
            sub = r - i;
            for (int j = i; j < len; j++)
                s[j] = s[j + sub];///sub可能等于10000，所以数组要开20000
            len -= sub;
            i = -1;///重新扫描
        }
    }
    return len;
}


int zuma() {
    cin.getline(s, 10005);///因为初始珠子数量可能为0，所以可能读入/n
    int len = strlen(s);
    int m;
    cin >> m;
    while (m--) {
        int pos;
        char c;
        scanf("%d %c", &pos, &c);
        len = solve(pos, c, len);
        if (len)
            printf("%s\n", s);
        else
            printf("-\n");
    }
    return 0;
}