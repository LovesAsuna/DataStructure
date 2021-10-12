#include <iostream>
#include <stdlib.h>

using namespace std;

#define L 500005
int nums[L];

/*把常用的变量变成全局的减少参数传递*/

int compare(const void *a, const void *b) {
    int *pa = (int *) a;
    int *pb = (int *) b;
    return (*pa) - (*pb);  //从小到大排序
}

int binarySearch1(int target, int lo, int hi) {
    while (lo < hi) {
        // int mid = lo + (hi - lo) / 2;
        int mid = lo + ((hi - lo) >> 1);
        // 若target <= nums[mid]，hi = mid，在[lo, mid)中找
        // 若target > nums[mid]，lo = mid + 1，在(mid, hi)中
        nums[mid] >= target ? hi = mid : lo = mid + 1;
    }
    return lo; // 循环结束时，lo为>=target的最小下标，故lo-1为<target的最大下标
} // 有多个元素命中target时，保证返回下标最大的；查找失败时返回<target的最大下标

int binarySearch2(int target, int lo, int hi) {
    while (lo < hi) {
        // int mid = lo + (hi - lo) / 2;
        int mid = lo + ((hi - lo) >> 1);
        nums[mid] > target ? hi = mid : lo = mid + 1;
    }
    return --lo; // 循环结束时，lo为>target的最小下标，故lo-1为<=target的最大下标
} // 有多个元素命中target时，保证返回下标最大的；查找失败时返回<=target的最大下标


int range(int length, int a, int b) {
    int loRange = binarySearch1(a, 0, length);
    int hiRange = binarySearch2(b, 0, length);
    return hiRange - loRange + 1;
}

int rangeSearch() {
    int length, queryTimes;
    scanf("%d %d\n", &length, &queryTimes);
    //cin >> length >> queryTimes;
    //int nums[length];
    for (int i = 0; i < length; i++) {
        //cin >> nums[i];
        scanf("%d", &nums[i]);
    }

    qsort(nums, length, sizeof(int), compare);

    for (int query = 1; query <= queryTimes; query++) {
        int a, b;
        //cin >> a >> b;
        scanf("%d %d", &a, &b);
        //cout << range(length, a, b) << endl;
        printf("%d\n", range(length, a, b));
    }
}