#ifndef CPLUSPLUS_VECTOR_H
#define CPLUSPLUS_VECTOR_H

#include <iostream>

typedef int Rank;

using namespace std;

const int defaultCapacity = 10;

template<typename T>
class Vector {
protected:
    // 存放数组
    T *_elem;
    // 最大可容纳那表项的项数
    Rank

















    _capacity{};
    // 当前向量的大小
    Rank _size{};
protected:
    // 向量空间不足时扩容
    void expand() {
        // 尚未满员时，不需要扩容
        if (_size < _capacity) {
            return;
        }
        // 不低于最小容量
        _capacity = max(_capacity, defaultCapacity);
        T *oldElem = _elem;
        // 容量加倍
        _elem = new T[_capacity <<= 1];
        // 复制原向量内容
        for (int i = 0; i < _size; i++) {
            // T为基本类型，或已重载赋值操作符'='
            _elem[i] = oldElem[i];
        }
        delete[]oldElem;
    }

    void shrink() {
        // 不致收缩到DEFAULT_CAPACITY以下
        if (_capacity < defaultCapacity << 1) {
            return;
        }
        // 以25%为界
        if (_size << 2 > _capacity) {
            return;
        }
        T *oldElem = _elem;
        // 容量减半
        _elem = new T[_capacity >>= 1];
        // 复制原向量内容
        for (int i = 0; i < _size; i++) {
            // T为基本类型，或已重载赋值操作符'='
            _elem[i] = oldElem[i];
        }
        // 释放原空间
        delete[]oldElem;
    }

    virtual void merge(int lo, int mi, int hi) {
        // lo为第1有序区的第1个元素，i指向第1个元素, mid为第1有序区的最后1个元素
        // mid+1为第2有序区第1个元素，j指向第1个元素
        int i = lo, j = mi + 1, k = 0;
        // temp数组暂存合并的有序序列
        T *temp = new T[hi - lo + 1];
        while (i <= mi && j <= hi) {
            // 较小的先存入temp中
            if (_elem[i] <= _elem[j]) {
                temp[k++] = _elem[i++];
            } else {
                temp[k++] = _elem[j++];
            }
        }
        // 若比较完之后，第一个有序区仍有剩余，则直接复制到t数组中
        while (i <= mi) {
            temp[k++] = _elem[i++];
        }
        // 同上
        while (j <= hi) {
            temp[k++] = _elem[j++];
        }
        // 将排好序的存回data中lo到hi这区间
        for (i = lo, k = 0; i <= hi; i++, k++) {
            _elem[i] = temp[k];
        }
        // 释放内存，由于指向的是数组，必须用delete []
        delete[]temp;
    }

public:
    // 容量为c、规模为s、所有元素初始为v
    explicit Vector(int c = defaultCapacity, int s = 0) {
        _elem = new T[_capacity = c];
    }

    // 数组区间复制
    Vector(T const *A, Rank lo, Rank hi) {
        copyFrom(A, lo, hi);
    }

    // 向量区间复制
    Vector(Vector<T> const &V, Rank lo, Rank hi) {
        copyFrom(V._elem, lo, hi);
    }

    // 向量整体复制
    Vector(Vector<T> &V) {
        copyFrom(V._elem, 0, V.size());
    }

    void copyFrom(T const *A, Rank lo, Rank hi) {
        // 分配空间
        _elem = new T[_capacity = 2 * (hi - lo)];
        // 规模清零
        _size = 0;
        while (lo < hi) {
            // A[lo, hi)内的元素逐一复制至_elem[0, hi-lo)
            _elem[_size++] = A[lo++];
        }
    }

    ~Vector() {
        delete[]_elem;
    }

    int size() const {
        return _size;
    }

    // 无序向量整体查找
    Rank find(T const &e) const {
        return find(e, 0, _size);
    }

    // 查找位于区间[lo, hi)的值为e的元素的秩, 0 <= lo < hi <= _size
    Rank find(T const &e, Rank lo, Rank hi) const {
        // 在命中多个元素时可返回秩最大者
        // 逆向查找
        while ((lo < hi--) && (e != _elem[hi]));
        // hi < lo意味着失败; 否则hi即命中元素的秩
        // 失败时，返回lo - 1
        return hi;
    }

    // 有序向量整体查找
    Rank search(T const &e) const {
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }

    // 在有序向量的区间[lo, hi)内，确定不大于e的最后一个节点的秩, 0 <= lo < hi <= _size
    Rank search(T const &e, Rank lo, Rank hi) const {
        return binSearch(_elem, e, lo, hi);
    }

    template<typename VST>
    void traverse(VST &visit) {
        for (int i = 0; i < _size; i++) {
            visit(_elem[i]);
        }
    }

    // 删除重复元素，返回被删除的元素数目
    int deduplicate() {
        // 记录原规模
        int oldSize = _size;
        // 从_elem[1]开始
        Rank i = 1;
        // 自
        // 前向后逐一考察各元素_elem[i]
        while (i < _size) {
            // 往前缀中寻找雷同者
            // 若无雷同考察后继
            // 否则删除雷同者
            find(_elem[i], 0, i) < 0 ? i++ : remove(i);
        }
        // 向量规模变化量，即删除元素总数
        return oldSize - _size;
    }

    int uniquify() {
        // 各对互异“相邻”元素的秩
        Rank i = 0, j = 0;
        // 逐一扫描，直至末元素
        while (++j < _size) {
            // 跳过雷同者; 发现不同元素时，向前移至紧邻于前者右侧
            if (_elem[i] != _elem[j]) {
                _elem[++i] = _elem[j];
            }
        }
        // 直接截除尾部多余元素
        _size = ++i;
        shrink();
        // 向量规模变化量，即被删除元素总数
        return j - i;
    }

    // 在有序向量区间[lo, hi) 内查找元素e
    Rank binSearch(T const &e, Rank lo, Rank hi) {
        // 每步迭代可能要做两次比较判断，有三个分支
        while (lo < hi) {
            // 以中点为轴点
            Rank mi = (lo + hi) >> 1;
            (e < _elem[mi]) ? hi = mi : lo = mi + 1;
            // 出口时，_elem[lo = hi]为大于e的最小元素
        }
        // 故lo - 1即不大于e的元素的最大秩
        return --lo;
    }

    // 将e作为秩r元素插入，0 <= r <= size
    Rank insert(Rank r, T const &e) {
        // 如有必要，扩容
        expand();
        //依次后移，空出第r号位置
        for (int i = _size; i > r; i--) {
            _elem[i] = _elem[i - 1];
        }
        // 置入新元素，更新容量
        _elem[r] = e;
        _size++;
        return r;
    }

    // 默认作为末元素插入
    Rank insert(T const &e) {
        return insert(_size, e);
    }

    // 删除区间[lo, hi), 0 <= lo <= hi <= size
    int remove(Rank lo, Rank hi) {
        // 出于效率考虑，单独处理退化情况
        if (lo == hi) {
            return 0;
        }
        while (hi < _size) {
            _elem[lo++] = _elem[hi++];
        }
        _size = lo;
        // 更新规模，若有必要则缩容
        shrink();
        return hi - lo;
    }

    // 删除向量中秩为r的元素，0 <= r < size
    T remove(Rank r) {
        // 备份被删除元素
        T e = _elem[r];
        if (r < 0) {
            return e;
        }
        // 调用区间删除算法
        remove(r, r + 1);
        return e;
    }

    bool isEmpty() const {
        return _size == 0;
    }

    bool isFull() const {
        return _size == _capacity;
    }

    virtual void mergeSort(int lo, int hi) {
        // 单元区间自然有序
        if (lo >= hi) {
            return;
        }
        int mi = lo + (hi - lo) / 2;
        // 对前半段排序
        mergeSort(lo, mi);
        // 对后半段排序
        mergeSort(mi + 1, hi);
        // 归并
        merge(lo, mi, hi);
    }

    // 重载赋值操作符，以便直接克隆向量
    Vector<T> &operator=(Vector<T> const &V) {
        // 释放原有内容
        if (_elem) {
            delete[] _elem;
        }
        // 整体复制
        copyFrom(V._elem, 0, V.size());
        // 返回当前对象的引用，以便链式赋值
        return *this;
    }

    // 0 <= r < _size
    T const &operator[](Rank index) const {
        return _elem[index];
    }

    // 0 <= r < _size
    T &operator[](Rank index) {
        return _elem[index];
    }

    int operator<=(Vector<T> &v) {
        int index = 0;
        while (_elem[index] == v._elem[index]) {
            index++;
        }
        if (_elem[index] > v._elem[index]) {
            return 1;
        } else if (_elem[index] < v._elem[index]) {
            return -1;
        } else {
            return 0;
        }
    }
};

#endif
