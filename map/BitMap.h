#ifndef CPLUSPLUS_BITMAP_H
#define CPLUSPLUS_BITMAP_H

#include <fstream>
#include <memory.h>

class Bitmap {
private:
    char *M;
    // 比特图所存放的空间M[]，容量为N*sizeof(char)*8比特
    int N;
protected:
    void init(int n) {
        M = new char[N = (n + 7) / 8];
        memset(M, 0, N);
    }

public:
    // 按指定或默认规模创建比特图（为测试暂时选用较小的默认值）
    explicit Bitmap(int n = 8) {
        init(n);
    }

    // 按指定或默认规模，从指定文件中读取比特图
    explicit Bitmap(const char *file, int n = 8) {
        init(n);
        FILE *fp = fopen(file, "r");
        fread(M, sizeof(char), N, fp);
        fclose(fp);
    }

    ~Bitmap() {
        delete[] M;
        M = nullptr;
    } //析构时释放比特图空间

    void set(int k) {
        expand(k);
        M[k >> 3] |= (0x80 >> (k & 0x07));
    }

    void clear(int k) {
        expand(k);
        M[k >> 3] &= ~(0x80 >> (k & 0x07));
    }

    bool test(int k) {
        expand(k);
        return M[k >> 3] & (0x80 >> (k & 0x07));
    }

    // 将位图整体导出至指定的文件，以便对此后的新位图批量初始化
    void dump(char const *file) {
        FILE *fp = fopen(file, "w");
        fwrite(M, sizeof(char), N, fp);
        fclose(fp);
    }

    // 将前n位转换为字符串——
    char *bits2string(int n) {
        // 此时可能被访问的最高位为bitmap[n - 1]
        expand(n - 1);
        char *s = new char[n + 1];
        // 字符串所占空间，由上层调用者负责释放
        s[n] = '\0';
        for (int i = 0; i < n; i++) {
            s[i] = test(i) ? '1' : '0';
        }
        // 返回字符串位置
        return s;
    }

    void expand(int k) { //若被访问的Bitmap[k]已出界，则需扩容
        if (k < 8 * N) return; //仍在界内，无需扩容
        int oldN = N;
        char *oldM = M;
        init(2 * k); //与向量类似，加倍策略
        memcpy_s(M, N, oldM, oldN);
        delete[] oldM; //原数据转移至新空间
    }

};

#endif
