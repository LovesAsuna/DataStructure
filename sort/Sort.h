#ifndef CPLUSPLUS_SORT_H
#define CPLUSPLUS_SORT_H

#include "../linearlist/Vector.h"
#include "../heap/ComplHeap.h"

class Sort {
public:
    // 对向量v作直接插入排序
    template<typename T>
    static void insertionSort(T *v, int n) {
        for (Rank i = 1; i <= n - 1; ++i) {
            if (v[i] < v[i - 1]) {
                // 复制为监视哨
                T back = v[i];
                Rank j;
                for (j = i - 1; back < v[j] && j >= 0; --j) {
                    // 记录后移
                    v[j + 1] = v[j];
                }
                // 插入到正确位置
                v[j + 1] = back;
            }
        }
    }

    // 对向量v作冒泡排序
    template<typename T>
    static T *bubbleSort(T *v, int n) {
        Rank i = 1;
        bool exchange = true;
        while (i < n && exchange) {
            //标志置为false,假定未交换
            exchange = false;
            for (Rank j = n - 1; j >= i; j--) {
                // 逆序
                if (v[j - 1] > v[j]) {
                    // 交换
                    swap(v[j - 1], v[j]);
                    // 标志置为true,有交换
                    exchange = true;
                }
            }
            i++;
        }
        return v;
    }

    // 对向量v作直接选择排序
    template<typename T>
    static void selectSort(Vector<T> &v) {
        int n = v.size();
        while (0 < --n) {
            Rank mx = n;
            for (Rank i = 0; i < n; i++) {
                if (v[mx] < v[i]) {
                    mx = i;
                }
            }
            // 将[hi]与[lo, hi]中的最大者交换
            swap(v[mx], v[n]);
        }

    }

    template<typename T>
    static T *heapSort(T *v, int n) {
        ComplHeap<T> heap(v, n);
        while (!heap.isEmpty()) {
            v[--n] = heap.delMax();
        }
        return v;
    }

    template<typename T>
    static void quicksort(Vector<T> &v, Rank start, Rank end) {
        if (start < end) {
            // 给数组分区并获得基准元素的位置
            Rank p = partition(v, start, end);
            // 排序小于基准元素的分区
            quicksort(v, start, p - 1);
            // 排序大于基准元素的分区
            quicksort(v, p + 1, end);
        }
    }

private:
    template<typename T>
    static int partition(Vector<T> &v, Rank start, Rank end) {
        // 釆用要分区的子范围中的第一个元素作为基准元素
        T pivotValue = v[start];
        Rank pivotPosition = start;
        // 重新排列分区子范围中除基准元素之外的从start到end的兀素
        for (Rank pos = start + 1; pos <= end; pos++) {
            if (v[pos] < pivotValue) {
                // v[scan]是“当前”项目 //使用这个当前项目和刚好位于基准元素右侧的元素交换
                swap(v[pivotPosition + 1], v[pos]);
                // 使用当前项目和基准元素交换
                swap(v[pivotPosition], v[pivotPosition + 1]);
                // 调整基准元素的位置使基准元素项目保持不变
                pivotPosition++;
            }
        }
        return pivotPosition;
    }
};


#endif
