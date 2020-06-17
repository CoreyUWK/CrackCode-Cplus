/*
 * Chap10SortingAndSearchingMain.cpp
 *
 *  Created on: Jun. 9, 2020
 *      Author: ckirsch
 */
#include "Chap10SortingAndSearchingMain.h"
#include <vector>
#include <iostream>
#include <cassert>
#include <stdlib.h>     /* srand, rand */
#include <algorithm>    // std::reverse
#include <unordered_map>
#include <deque>
#include <list>
#include <memory>
#include <bitset>
using namespace std;


void mergeSort(vector<int> &values, int low, int high)
{
    if (low >= high)
    {
        return;
    }

    int mid = low + (high - low) / 2;
    mergeSort(values, low, mid);
    mergeSort(values, mid + 1, high);

    // merge
    vector<int> tmp = values;
    int currentIdx = low;
    int lowIdx = low;
    int highIdx = mid + 1;
    while (lowIdx <= mid && highIdx <= high)
    {
        if (tmp[lowIdx] < tmp[highIdx])
        {
            values[currentIdx] = tmp[lowIdx++];
        }
        else
        {
            values[currentIdx] = tmp[highIdx++];
        }
        ++currentIdx;
    }
    // copy remaining
    while (lowIdx <= mid)
    {
        values[currentIdx++] = tmp[lowIdx++];
    }
}


int quickSortPartition(vector<int> &values, int left, int right)
{
    int mid = left + (right - left) / 2; // pick mid as pivot
    int pivot = values[mid];

    while (left <= right)
    {
        // Find element on left that should be on right
        while (values[left] < pivot) ++left;

        // Find element on right that should be on left
        while (values[right] > pivot) --right;

        // Swap elements and move left and right indices
        if (left <= right)
        {
            swap(values[left++], values[right--]);
        }
    }
    return left;
}
void quickSort(vector<int> &values, int left, int right)
{
    int index = quickSortPartition(values, left, right);
    if (left < index - 1)
    {
        // Sort left half
        quickSort(values, left, index - 1);
    }

    if (index < right)
    {
        // Sort right half
        quickSort(values, index, right);
    }
}


template <typename T>
int binarySearch(const T &values, int x)
{
    int low = 0;
    int high = values.size() - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (values[mid] < x)
        {
            low = mid + 1;
        }
        else if (values[mid] > x)
        {
            high = mid - 1;
        }
        else
        {
            return mid;
        }
    }

    return -1; // error
}

int binarySearchRecursive(const vector<int> &values, int x, int low, int high)
{
    if (low > high) return -1; // Error

    int mid = low + (high - low) / 2;
    if (values[mid] < x)
    {
        return binarySearchRecursive(values, x, mid + 1, high);
    }
    else if (values[mid] > x)
    {
        return binarySearchRecursive(values, x, low, mid - 1);
    }
    return values[mid];
}


void merge(vector<int> &a, const vector<int> &b, int countA, int countB)
{
    int index = a.size() - 1;
    while (index >= 0)
    {
        if (countA >= 0 && a[countA] > a[countB])
        {
            a[index--] = a[countA--];
        }
        else
        {
            a[index--] = b[countB--];
        }
    }
}


/* O(NlogN * MlogM), N = # of words, M = largest string size */
void groupAnagramSort(vector<string> &words)
{
    sort(words.begin(), words.end(),
            [](string a, string b) -> bool
            {
                sort(a.begin(), a.end());
                sort(b.begin(), b.end());
                return (a.compare(b) < 0) ? true : false;
            });
}

void groupAnagramHash(vector<string> &words)
{
    sort(words.begin(), words.end(), [](string a, string b) -> bool
            {
                int aCount = 0;
                for (auto c : a)
                {
                    aCount += (int)c;
                }

                int bCount = 0;
                for (auto c : b)
                {
                    bCount += (int)c;
                }

                return (aCount < bCount) ? true : false;
            });
}

void groupAnagramBucketSort(vector<string> &words)
{
    // Create a mapping of sorted word to list of anagrams
    unordered_map<string, vector<string>> mapSortToAnagrams;
    for (auto word : words)
    {
        string key = word;
        sort(key.begin(), key.end());
        mapSortToAnagrams[key].push_back(word);
    }

    words.clear();
    for (auto mapping : mapSortToAnagrams)
    {
        for (auto word : mapping.second)
        {
            words.push_back(word);
        }
    }
}

/* O(logn) */
int searchRotatedArray(const vector<int> &list, int x)
{
    if (list.empty())
    {
        return -1;
    }

    int rotationIdx = 0;
    if (list.front() > list.back())
    {
        // binary search to find rotation spot
        int first = 0;
        int last = list.size();
        int mid;
        while (first <= last)
        {
            mid = first + (last - first) / 2;
            if (list[mid] > list[first])
            {
                first = mid + 1;
            }
            else if (list[mid] < list[first])
            {
                last = mid - 1;
            }
            else
            {
                break;
            }
        }
        rotationIdx = mid;
    }

    if (x < list.front())
    {
        auto searchList = std::vector<int>(list.begin() + rotationIdx, list.end());
        return binarySearch<vector<int>>(searchList, x) + rotationIdx;
    }
    else
    {
        auto searchList = std::vector<int>(list.begin(), list.begin() + rotationIdx);
        return binarySearch<vector<int>>(searchList, x);
    }

    return -1;
}

/* O(logn) */
int searchRotatedArrayBook(const vector<int> &list, int x, int start, int end)
{
    if (start > end)
    {
        return -1;
    }

    int mid = start + (end - start) / 2;
    if (list[mid] == x)
    {
        return mid;
    }

    if (list[start] < list[mid])
    {
        if (list[start] <= x && x < list[mid])
        {
            return searchRotatedArrayBook(list, x, start, mid - 1);
        }
        else
        {
            return searchRotatedArrayBook(list, x, mid + 1, end);
        }
    }
    else if (list[start] > list[mid])
    {
        if (list[mid] < x && x <= list[end])
        {
            return searchRotatedArrayBook(list, x, mid + 1, end);
        }
        else
        {
            return searchRotatedArrayBook(list, x, start, mid - 1);
        }
    }
    else if (list[start] == list[mid])
    {
        if (list[mid] != list[end])
        {
            return searchRotatedArrayBook(list, x, mid + 1, end);
        }
        else
        {
            int result = searchRotatedArrayBook(list, x, start, mid - 1);
            if (-1 == result)
            {
                return searchRotatedArrayBook(list, x, mid + 1, end);
            }
            return result;
        }
    }
    return -1;
}

int elementAt(const vector<int> &list, uint i)
{
    if (i >= list.size()) return -1;
    return list[i];
}


/* O(logn + logn) = O(logn) */
int findLimited(const vector<int> &list, int x)
{
    if (list.empty()) return -1;

    // Find size of list
    int index = 1;
    while (elementAt(list, index) != -1 && elementAt(list, index) < x) index *= 2;
    if (elementAt(list, index) != -1 && elementAt(list, index) == x)
    {
        return index;
    }
    else
    {
        index -= 1; // if ends when [index] >, so search -1. If [index] == -1, then search to before
    }

    // binary search for element
    int low = index / 2; // since [prevIndex] < x and x < [index]
    int high = index;
    int mid;
    while (low <= high)
    {
        mid = low + (high - low) / 2;
        if (elementAt(list, mid) == -1 || elementAt(list, mid) > x)
        {
            high = mid - 1;
        }
        else if (elementAt(list, mid) < x)
        {
            low = mid + 1;
        }
        else
        {
            return mid;
        }
    }
    return -1;
}


/* Worst case: O(n) if list contains all empty strings or all but 1 */
int sparseSearch(const vector<string> &list, string s, int start, int end)
{
    if (list.empty() || s.empty() || start > end)
    {
        return -1;
    }

    int mid = start + (end - start) / 2;
    if (list[mid].empty())
    {
        // Find next actual string
        int left = mid - 1,
            right = mid + 1;
        for (; start <= left && right <= end; --left, ++right)
        {
            if (!list[left].empty())
            {
                mid = left;
                break;
            }
            else if (!list[right].empty())
            {
                mid = right;
                break;
            }
        }

        // no words found
        if (left < start)
        {
            return -1;
        }
    }

    int strCompare = list[mid].compare(s);
    if (strCompare < 0)
    {
        return sparseSearch(list, s, mid + 1, end);
    }
    else if (strCompare > 0)
    {
        return sparseSearch(list, s, start, mid - 1);
    }
    else if (0 == strCompare)
    {
        return mid;
    }

    return -1;
}


/* Find duplicates max 1-32000 values => 32000
 * 4K mem => 32000 bits */
vector<int> findDUplicates(const vector<int> &array)
{
    const int N = 32000;
    bitset<N> map;
    vector<int> duplicates;

    for (auto val : array)
    {
        if (map.test(val))
        {
            duplicates.push_back(val);
        }
        else
        {
            map.set(val);
        }
    }

    return duplicates;
}


bool sortedMatrixFind(const vector<vector<int>> &matrix, int x)
{
    int row = 0;
    int col = matrix[row].size() - 1;

    while (row < (int)matrix.size() && col >= 0)
    {
        if (x == matrix[row][col])
        {
            return true;
        }
        else if (x > matrix[row][col])
        {
            ++row;
        }
        else
        {
            --col;
        }
    }
    return false;
}

/* 1) track items in vector and sort on insertion
 * Mem: O(n)
 * Time: O(nlogn)
 * 2) do above in get rank with partial sort O(nlogx)
 * 3) track elements in a sorted linked list
 * track O(n)
 * rank O(1)
 * could make this faster by using map so don't have to do linear search for insertion
 * 4)  */
class RankFromStream
{
    deque<int> stream;

public:
    // O(n)
    void trackSort(int x)
    {
        uint insertPos = 0;
        for (auto element : stream)
        {
            if (element > x) break;
            ++insertPos;
        }
        auto itr = stream.begin();
        std::advance(itr, insertPos);
        stream.insert(itr, x);
    }

    // O(logn)
    int getRankOfNumberSort(int x)
    {
        int pos = binarySearch<deque<int>>(stream, x);
        for (;pos < (int)stream.size(); ++pos)
        {
            if (stream[pos] != x)
            {
                --pos;
                break;
            }
        }
        return pos;
    }
};

class RankFromStreamTree
{
    struct RankFromStreamTreeNode
    {
        int value;
        int leftSize;
        shared_ptr<RankFromStreamTreeNode> left;
        shared_ptr<RankFromStreamTreeNode> right;

        RankFromStreamTreeNode(int value) : value(value), leftSize(0) {}

        /* log(n) */
        void insert(int x)
        {
            if (x <= value)
            {
                if (left.get() == nullptr)
                {
                    left = make_shared<RankFromStreamTreeNode>(x);
                }
                else
                {
                    left->insert(x);
                }

                // Increasing count when going left since adding to the less than/left side
                ++leftSize;
            }
            else
            {
                if (right.get() == nullptr)
                {
                    right = make_shared<RankFromStreamTreeNode>(x);
                }
                else
                {
                    right->insert(x);
                }
            }
        }

        /* log(n) */
        int getRank(int x)
        {
            if (value == x)
            {
                return leftSize;
            }
            else if (x < value && left.get() != nullptr)
            {
                return left->getRank(x);
            }
            else if (right.get() != nullptr)
            {
                return leftSize + 1 + right->getRank(x);
            }

            return -1;
        }
    };

    shared_ptr<RankFromStreamTreeNode> root;

public:
    /* Log(n) average, O(n) worst case not balanced */
    void track(int x)
    {
        if (root.get() == nullptr)
        {
            root = make_shared<RankFromStreamTreeNode>(x);
        }
        else
        {
            root->insert(x);
        }
    }

    /* Log(n) average, O(n) worst case not balanced*/
    int getRankOfNumber(int x)
    {
        return root->getRank(x);
    }
};


/* 1) sort list and then either swap every second with previous or take from front and back - O(nlogn)
 * 2) min heap and max heap - O(nlogn) + difficult to remove same from both
 * 3) take max of neighbours O(n) */
void peaksAndVallies(vector<int> &list)
{
    vector<int> rangeCheck = {-1, 0, 1};
    for (uint i = 1; i < list.size(); i += 2)
    {
        // Find Min element from surrounding
        uint minIndex = i;
        for (auto offset : rangeCheck)
        {
            uint checkIndex = i + offset;
            if (checkIndex < list.size() && list[minIndex] > list[checkIndex])
            {
                minIndex = checkIndex;
            }
        }

        // Swap to get min element in center of high numbers
        if (i != minIndex)
        {
            swap(list[i], list[minIndex]);
        }
    }
}


template <class T>
void printVector(const vector<T> &list)
{
    for (auto value : list)
    {
        cout << value << " ";
    }
    cout << endl;
}

/* Bubble Sort: O(n^2), O(1) bubble up
 * Selection Sort: O(n^2), O(1) child select smallest
 * Merge Sort: O(nlogn) average and worst, Memory depends
 * Quick Sort: O(nlogn) average, O(n^2) worst case, O(logn) memory */
void Chap10SortingAndSearchingMain()
{
    {
        vector<int> values = {8, 6, 4, 1, 10, 9, 5, 3};
        mergeSort(values, 0, values.size() - 1);
        printVector<int>(values);
    }
    {
        vector<int> values = {10, 11, 4, 1, 7, 6, 5, 3};
        mergeSort(values, 0, values.size() - 1);
        printVector<int>(values);
    }
    {
        vector<int> values = {8, 6, 4, 1, 10, 9, 5, 3};
        quickSort(values, 0, values.size() - 1);
        printVector<int>(values);
    }
    {
        vector<int> values = {10, 11, 4, 1, 7, 6, 5, 3};
        quickSort(values, 0, values.size() - 1);
        printVector<int>(values);
    }

    {
        vector<string> words = {"abc", "def", "cba", "ghi", "ffe", "hig"};
        groupAnagramSort(words);
        printVector<string>(words);
    }
    {
        vector<string> words = {"abc", "def", "cba", "ghi", "ffe", "hig"};
        groupAnagramHash(words);
        printVector<string>(words);
    }
    {
        vector<string> words = {"abc", "def", "cba", "ghi", "ffe", "hig"};
        groupAnagramBucketSort(words);
        printVector<string>(words);
    }

    {
        vector<int> list = {15, 16, 19, 20, 25, 1, 3, 4, 5, 7, 10, 14};
        cout << "Index: " << searchRotatedArray(list, 5) << endl;
        cout << "Index: " << searchRotatedArrayBook(list, 5, 0, list.size() - 1) << endl;
    }

    {
        vector<int> list = {1,2,3,4,5,6,7,8,9};
        cout << "Index: " << findLimited(list, 6) << endl;
    }

    {
        vector<string> list = {"at", "", "", "", "ball", "", "", "car", "", "", "dad", "", ""};
        cout << "Index: " << sparseSearch(list, "ball", 0, list.size() - 1) << endl;
    }

    {
        vector<int> values = {1, 3, 5, 50, 33, 22, 11, 10, 5, 6, 4, 9, 10, 33};
        auto duplicates = findDUplicates(values);
        cout << "Duplicates: ";
        printVector<int>(duplicates);
    }

    {
        vector<vector<int>> matrix = {
                {15, 20, 40, 85},
                {20, 35, 80, 95},
                {30, 55, 95, 105},
                {40, 80, 100, 120}
        };
        assert(true == sortedMatrixFind(matrix, 55));
    }

    {
        RankFromStream stream;
        RankFromStreamTree streamTree;

        vector<int> values = {5,1,4,4,5,9,7,13,3};
        for (auto val : values)
        {
            stream.trackSort(val);
            streamTree.track(val);
        }

        cout << "Rank of 1: " << stream.getRankOfNumberSort(1) << endl;
        cout << "Rank of 1: " << streamTree.getRankOfNumber(1) << endl;
        cout << "Rank of 3: " << stream.getRankOfNumberSort(3) << endl;
        cout << "Rank of 3: " << streamTree.getRankOfNumber(3) << endl;
        cout << "Rank of 4: " << stream.getRankOfNumberSort(4) << endl;
        cout << "Rank of 4: " << streamTree.getRankOfNumber(4) << endl;
    }

    {
        vector<int> list = {5, 8, 6, 2, 3, 4, 6};
        peaksAndVallies(list);
        printVector<int>(list);
    }
}


