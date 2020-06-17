/*
 * Chap8Recursion.cpp
 *
 *  Created on: May 27, 2020
 *      Author: ckirsch
 */
#include "Chap8Recursion.h"
#include <stdio.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <functional>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <array>
#include <stack>
#include <cmath>
#include <list>
using namespace std;

int fibMemHelper(int n, int mem[])
{
    if (n == 0 || n == 1) return n;

    if (mem[n] == 0)
        mem[n] = fibMemHelper(n - 1, mem) + fibMemHelper(n - 2, mem);
    return mem[n];
}

int fibMem(int n)
{
    const int memSize = n + 1;

    int mem[memSize] = {};
    return fibMemHelper(n, mem);
}

int fib(int n)
{
    if (n <= 1)
    {
        return n;
    }

    int minus1 = 0;
    int minus2 = 1;

    for (int i = 0; i < n; ++i)
    {
        int val = minus2 + minus1;
        minus2 = minus1;
        minus1 = val;
    }

    return minus1;
}

/* Time if mem removes: O(3^n) as each node in recursive stack creates three branches
 * Time: O(numStairs)*/
template <int N>
int numPossibleSteps(int numStairs, int step[N], int mem[])
{
    if (numStairs == 0)
    {
        return 1;
    }
    else if (mem[numStairs] != -1)
    {
        return mem[numStairs];
    }

    int possibleSteps = 0;
    for (int i = 0; i < N; ++i)
    {
        if (step[i] <= numStairs)
        {
            possibleSteps += numPossibleSteps<N>(numStairs - step[i], step, mem);
        }
    }
    mem[numStairs] = possibleSteps;
    return mem[numStairs];
}
template <int S>
int numPossibleStepsMem(int numStairs, int step[S])
{
    if (numStairs <= 0)
    {
        return 0;
    }

    int mem[numStairs + 1] = {0};
    mem[0] = 1;
    for (int i = 1; i < numStairs + 1; ++i)
    {
        for (int s = 0; s < S; ++s)
        {
            if (step[s] <= i)
            {
                mem[i] += mem[i - step[s]];
            }
        }
    }

    return mem[numStairs];
}

/* Time: O(2^(rows + cols)) branching twice for depth of rows + cols
 * or since each path has rows + cols steps, and there are 2 choices at each step */
template <int rows, int cols>
bool findRobotPath(bool grid[rows][cols], pair<int,int> robot, vector<pair<int,int>> &path)
{
    if (robot.first == (rows - 1) && robot.second == (cols - 1))
    {
        path.push_back(robot);
        return true;
    }

    pair<int,int> moves[] = {
            {0, 1}, // move right
            {1, 0}  // move down
    };

    bool foundPath = false;
    for (int i = 0; i < 2 && !foundPath; ++i)
    {
        pair<int,int> nextIndex = make_pair(robot.first + moves[i].first,
                robot.second + moves[i].second);

        if (nextIndex.first < rows && nextIndex.second < cols &&
                grid[nextIndex.first][nextIndex.second])
        {
            foundPath = findRobotPath<rows, cols>(grid, nextIndex, path);
        }
    }

    if (foundPath)
    {
        path.push_back(robot);
    }

    return foundPath;
}
template <int rows, int cols>
bool findRobotPathRecurise(bool grid[rows][cols], int row, int col, vector<pair<int,int>> &path)
{
    /* If out of bounds or not available, return. */
    if (col < 0 || row < 0 || !grid[row][col])
    {
        return false;
    }

    bool isAtOrigin = (row == 0) && (col == 0);
    /* If there's a path from the start to here, add my location. */
    if (isAtOrigin || findRobotPathRecurise<rows, cols>(grid, row, col - 1, path) ||
            findRobotPathRecurise<rows, cols>(grid, row - 1, col, path))
    {
        path.push_back(make_pair(row, col));
        return true;
     }

     return false;
}

struct PairHash
{
    // Hash
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const
    {
        std::size_t h1 = std::hash<T1>()(pair.first);
        std::size_t h2 = std::hash<T2>()(pair.second);

        return h1 ^ h2;
    }
};
struct PairCompare
{
    // Compare
    template <class T1, class T2>
    bool operator() (const std::pair<T1, T2> &pair1, const std::pair<T1, T2> &pair2) const
    {
        if (pair1.first == pair2.first && pair1.second == pair2.second)
        {
            return true;
        }
        return false;
    }
};
template <int rows, int cols>
bool findRobotPathRecuriseMem(bool grid[rows][cols], int row, int col,
        vector<pair<int,int>> &path, unordered_set<pair<int,int>, PairHash, PairCompare> &failedPoints)
{
    /* If out of bounds or not available, return. */
    if (col < 0 || row < 0 || !grid[row][col])
    {
        return false;
    }
    else if (failedPoints.find(make_pair(row, col)) != failedPoints.end())
    {
        return false;
    }

    bool isAtOrigin = (row == 0) && (col == 0);
    /* If there's a path from the start to here, add my location. */
    if (isAtOrigin || findRobotPathRecuriseMem<rows, cols>(grid, row, col - 1, path, failedPoints) ||
            findRobotPathRecuriseMem<rows, cols>(grid, row - 1, col, path, failedPoints))
    {
        path.push_back(make_pair(row, col));
        return true;
    }

    failedPoints.insert(make_pair(row, col));

    return false;
}

/* 1) could do brute force O(n)
 * 2) binary search O(logn) -> requires no duplicates */
template <int N>
int magicIndex(int arr[N], int start, int end)
{
    if (start > end)
    {
        return -1;
    }

    int mid = start + (end - start) / 2;
    if (arr[mid] < mid)
    {
        return magicIndex<N>(arr, mid + 1, end);
    }
    else if (arr[mid] > mid)
    {
        return magicIndex<N>(arr, start, mid - 1);
    }
    else
    {
        return mid;
    }
}
template <int N>
int magicIndexDuplicates(int arr[N], int start, int end)
{
    if (start > end)
    {
        return -1;
    }

    int mid = start + (end - start) / 2;
    if (arr[mid] == mid)
    {
        return mid;
    }

    /* Search left */
    int leftIdx = min(mid - 1, arr[mid]);
    int left = magicIndex<N>(arr, start, leftIdx);
    if (-1 != left)
    {
        return left;
    }

    /* Search right */
    int rightIdx = max(mid + 1, arr[mid]);
    int right = magicIndex<N>(arr, rightIdx, end);
    return right;
}

/* Get all subsets of set
 * For ABC:
 * {} = 1 = 2^0
 * {A} = {}, {A} = 2 = 2^1
 * {AB} = {},{A},{B},{AB} = 4 = 2^2
 * {ABC} = {},{A},{B},{C},{AB},{AC},{BC},{ABC} = 8 = 2^3
 * {ABCD} = {},{A},{B},{C},{D},
 *         {AB},{AC},{AD},{BC},{BD},{CD},
 *         {ABC},{ABD},{ACD},{BCD}
 *         {ABCD}
 *        = 16 = 2^4
 *
 * Therefore num subsets = 2^(subset size) = O(2^n)
 * 1) powerSet(n) = powerset(n-1) + n/2, n/2=difference in subset size from p(n) and p(n-1)
 * so if we iteratively add new element to that of previously generated subsets
 * we get next levels
 * Time = 1 + 1 + 2 + 4 + 8 = 16 O(2^n)
 * Space = 2^n subsets * n size = O(n2^n) or 2^n/2 elements = 2^n-1 as if you represent choices in binary half are zero and half have element
 * There for there will be 2^(n-1) subsets containing all elements of size n
 * O(n*2^(n-2)) */
vector<vector<int>> powerSetIterative(vector<int> items)
{
    vector<vector<int>> sets;
    sets.push_back(vector<int>());

    for (int item : items)
    {
        int setSize = sets.size();
        for (int i = 0; i < setSize; ++i)
        {
            vector<int> newSet = sets[i];
            newSet.push_back(item);
            sets.push_back(newSet);
        }
    }

    return sets;
}
/* Time and space O(n2^n) */
vector<vector<int>> powerSetRecrsive(const vector<int> &set, int index)
{
    vector<vector<int>> allSubsets;
    if ((int)set.size() == index)
    {
        allSubsets.push_back(vector<int>());
    }
    else
    {
        allSubsets = powerSetRecrsive(set, index + 1);
        int item = set[index];

        vector<vector<int>> moreSubsets;
        for (auto subset : allSubsets)
        {
            vector<int> newSubset(subset);
            newSubset.push_back(item);
            moreSubsets.push_back(newSubset);
        }
        allSubsets.insert(allSubsets.begin(), moreSubsets.begin(), moreSubsets.end());
    }

    return allSubsets;
}
vector<vector<int>> powerSetCombinatoric(const vector<int> &set)
{
    auto powerToSubSet = [](const vector<int> &set, int x)
    {
        vector<int> subSet;
        int index = 0;
        while (x > 0)
        {
            // Check first bit needs to be added to subset
            if (0x1 == (x & 0x1))
            {
                subSet.push_back(set[index]);
            }

            x >>= 1;
            ++index;
        }

        return subSet;
    };

    vector<vector<int>> sets;
    int possibleSolutions = 1 << set.size(); // 2^n
    for (int i = 0; i < possibleSolutions; ++i)
    {
        auto subSet = powerToSubSet(set, i);
        sets.push_back(subSet);
    }

    return sets;
}

/* Time: O(log smaller) since everytime dividing by half */
int recursiveMultiply(int smaller, int larger)
{
    if (smaller == 0)
    {
        return 0;
    }
    else if (smaller == 1)
    {
        return larger;
    }

    int result = recursiveMultiply(smaller >> 1, larger) << 1;
    if (smaller % 2)
    {
        result += larger;
    }
    return result;
}

void towerOfHanoiMine(array<stack<int>, 3> &towers, int numDisk, int origin, int buffer, int destination)
{
    if (numDisk <= 0)
    {
        return;
    }

    towerOfHanoiMine(towers, numDisk - 1, origin, destination, buffer);
    int top = towers[origin].top();
    towers[origin].pop();
    towers[destination].push(top);
    towerOfHanoiMine(towers, numDisk - 1, buffer, origin, destination);
}

class Tower
{
    int index;
    stack<int> disks;

public:
    Tower(int i) : index(i) {}

    int getIndex() {return index;}
    void add(int d) {disks.push(d);}
    void moveTop(Tower *t)
    {
        int top = disks.top();
        disks.pop();
        t->add(top);
    }

    void moveDisks(int n, Tower *dest, Tower *buf)
    {
        if (n > 0)
        {
            moveDisks(n - 1, buf, dest);
            moveTop(dest);
            buf->moveDisks(n - 1, dest, this);
        }
    }
};


/* there are n! permutations
 * it takes O(n) to go through all characters to build a permutation
 * adding strings takes O(n)
 * Time=O(n*n*n!) = O(n^2*n!) */
void permNoDups(string prefix, string postfix, vector<string> &permList)
{
    if (postfix.empty())
    {
        permList.push_back(prefix);
    }
    else
    {
        for (uint i = 0; i < postfix.size(); ++i)
        {
            permNoDups(prefix + postfix[i], postfix.substr(0, i) + postfix.substr(i + 1, postfix.size() - i), permList);
        }
    }
}
vector<string> permNoDups(string s)
{
    vector<string> permList;
    permNoDups("", s, permList);
    return permList;
}

vector<string> permNoDupsIterative(string s)
{
    vector<string> permList = {""};
    if (s.empty())
    {
        return permList;
    }

    // Go through each character adding it's permutations
    for (uint i = 0; i < s.size(); ++i)
    {
        string newChar = s.substr(i, 1);

        // Go through permList to add next character to previous permutations
        int premListsize = permList.size();
        for (int permIdx = 0; permIdx < premListsize; ++permIdx)
        {
            if (!permList[permIdx].empty())
            {
                // Go through permutations indexs to add new character
                for (uint pos = 0; pos < permList[permIdx].size(); ++pos)
                {
                    string perm = permList[permIdx];
                    permList.push_back(perm.insert(pos, newChar));
                }
            }
            permList[permIdx] += newChar;
        }
    }

    return permList;
}
vector<string> getPermsRecursive(string str)
{
    vector<string> permutations;

    // Base Case
    if (str.empty())
    {
        permutations.push_back("");
        return permutations;
    }

    char first = str.at(0); // get first char
    string remainder = str.substr(1, str.size() - 1); // remove first char
    vector<string> words = getPermsRecursive(remainder);
    for (string word : words)
    {
        for (uint j = 0; j <= word.size(); ++j)
        {
            string s = word.substr(0, j) + first + word.substr(j, word.size() - j);
            permutations.push_back(s);
        }
    }
    return permutations;
}
vector<string> getPermsRecursive2(string remainder)
{
    int len = remainder.size();
    vector<string> result;

    // base case
    if (len == 0)
    {
        result.push_back(""); // be sure to return empty string
        return result;
    }

    for (int i = 0; i < len; ++i)
    {
        // remove char i and find permutations of remaining chars
        string before = remainder.substr(0, i);
        string after = remainder.substr(i + 1, len - (i + 1));
        vector<string> partials = getPermsRecursive(before + after);

        // prepend char i to each permutation
        for (string s : partials)
        {
            result.push_back(remainder.at(i) + s);
        }
    }

    return result;
}

/* This solution is faster for strings with duplicates like: aaaabbbbbc as it doesn't process duplicate permutations like the others using a hash for caching
 * instead using hash counts for characters
 * Had to use remaining cause modifying hash by erasing items seems to affect algo so just left items with count of zero and updated remaining
 * This is a top down approch adding permutation when one is reached at leaf of descent */
void permWithDups(string prefix, unordered_map<char, int> &hash, int remaining, vector<string> &results)
{
    if (remaining == 0)
    {
        results.push_back(prefix);
        return;
    }

    for (auto it : hash)
    {
        if (it.second == 0)
            continue;

        // Take chosen out of hash
        --hash[it.first];

        permWithDups(prefix + it.first, hash, remaining - 1, results);

        // Put back chosen into hash for next permutation
        ++hash[it.first];
    }
}
vector<string> permWithDups(string str)
{
    vector<string> results;
    if (str.empty())
    {
        results.push_back("");
        return results;
    }

    // Get duplicate char counts
    unordered_map<char, int> hash;
    for (char c : str)
    {
        ++hash[c];
    }

    permWithDups("", hash, str.size(), results);
    return results;
}

/* Note: could return a vector<string> and used an unorder_set<string> as hash, but found could just
 * return the set as it already does a check for unique */
unordered_set<string> parenthises(int n)
{
    unordered_set<string> parenComb;
    if (n == 0)
    {
        parenComb.insert("");
        return parenComb;
    }

    auto parenCombPrev = parenthises(n - 1);
    for (auto comb : parenCombPrev)
    {
        for (uint i = 0; i <= comb.size(); ++i)
        {
            if (comb[i] == '(')
            {
                string newCombo = comb.substr(0, i + 1) + "()" + comb.substr(i + 1, comb.size() - (i + 1));
                parenComb.insert(newCombo);
            }
        }
        string newCombo = comb + "()";
        parenComb.insert(newCombo);
    }

    return parenComb;
}

/* This solution garentees no duplicates unlike above */
void parenthisesBuild(string &str, int index, int open, int close, vector<string> &result)
{
    if (open < 0 || close < open) return; // invalid state
    if (open == 0 && close == 0)
    {
        result.push_back(str);
        return;
    }

    str[index] = '(';
    parenthisesBuild(str, index + 1, open - 1, close, result);

    str[index] = ')';
    parenthisesBuild(str, index + 1, open, close - 1, result);
}
vector<string> parenthisesBuild(int n)
{
    vector<string> result;
    string str(n * 2, ' ');
    parenthisesBuild(str, 0, n, n, result);
    return result;
}

template<int rows, int cols>
void paintColor(int grid[rows][cols], int row, int col, int oColor, int nColor)
{
    if (row < 0 || row >= rows || col < 0 || col >= cols)
    {
        return;
    }
    else if (grid[row][col] != oColor)
    {
        return;
    }

    grid[row][col] = nColor;
    paintColor<rows, cols>(grid, row + 1, col, oColor, nColor);
    paintColor<rows, cols>(grid, row - 1, col, oColor, nColor);
    paintColor<rows, cols>(grid, row, col + 1, oColor, nColor);
    paintColor<rows, cols>(grid, row, col - 1, oColor, nColor);
}

int coinsPerm(const vector<int> &coins, int coinIdx, int cents, unordered_map<int, unordered_map<int, int>> &hash)
{
    if (cents == 0)
    {
        return 1;
    }
    else if (cents < 0)
    {
        return 0;
    }
    else if (coinIdx >= (int)coins.size())
    {
        return 0;
    }

    if (hash.find(coinIdx) == hash.end() || hash[coinIdx].find(cents) == hash[coinIdx].end())
    {
        int coin = coins[coinIdx];
        int numPerm = coinsPerm(coins, coinIdx, cents - coin, hash);
        numPerm += coinsPerm(coins, coinIdx + 1, cents, hash);
        hash[coinIdx][cents] = numPerm;
    }

    return hash[coinIdx][cents];
}

int makeChange(int amount, const vector<int> &coins, uint index, int **map)
{
    // retrieve value
    if (map[amount][index] > 0)
    {
        return map[amount][index];
    }

    if (index >= (coins.size() - 1)) // one coin remaining
    {
        return 1;
    }

    int coinAmount = coins[index];
    int ways = 0;
    for (int i = 0; i * coinAmount <= amount; ++i)
    {
        // go to next coin assuming i coins of coinAmount
        int amountRemaining = amount - (i * coinAmount);
        ways += makeChange(amountRemaining, coins, index + 1, map);
    }

    map[amount][index] = ways;
    return ways;
}
int makeChange(int n)
{
    vector<int> coins = {25, 10, 5, 1};
    int **map = new int*[n + 1]; // precomputed values
    for (int i = 0; i < n + 1; ++i)
    {
        map[i] = new int[coins.size()];
    }
    int count = makeChange(n, coins, 0, map);

    for (int i = 0; i < n + 1; ++i)
    {
        delete map[i];
    }
    delete map;
    return count;
}

bool canPlaceQueen(bool (&grid)[8][8], int rowIdx, int colIdx)
{
    // Check column
    for (int row = 0; row < 8; ++row)
    {
        if (grid[row][colIdx])
        {
            return false;
        }
    }

    // Check column
    for (int col = 0; col < 8; ++col)
    {
        if (grid[rowIdx][col])
        {
            return false;
        }
    }

    // Check top left diagonal
    for (int row = rowIdx - 1, col = colIdx - 1; 0 <= row && 0 <= col; row -= 1, col -= 1)
    {
        if (grid[row][col])
        {
            return false;
        }
    }

    // Check top right diagonal
    for (int row = rowIdx - 1, col = colIdx + 1; 0 <= row && col < 8; row -= 1, col += 1)
    {
        if (grid[row][col])
        {
            return false;
        }
    }

    // Check bottom left diagonal
    for (int row = rowIdx + 1, col = colIdx - 1; row < 8 && 0 <= col; row += 1, col -= 1)
    {
        if (grid[row][col])
        {
            return false;
        }
    }

    // Check bottom right diagonal
    for (int row = rowIdx + 1, col = colIdx + 1; row < 8 && col < 8; row += 1, col += 1)
    {
        if (grid[row][col])
        {
            return false;
        }
    }

    return true;
}

class Solution {
    vector<string> boardToString(bool **board, int N)
    {
        vector<string> convertedBoard;
        for (int row = 0; row < N; ++row)
        {
            string strRow = "";
            for (int col = 0; col < N; ++col)
            {
                strRow += (board[row][col]) ? "Q" : ".";
            }
            convertedBoard.push_back(strRow);
        }
        return convertedBoard;
    }

    void solveNQueens(bool **board,
                      bool *rowHasQueen,
                      int **slash,
                      bool *slashHasQueen,
                      int **backSlash,
                      bool *backSlashHasQueen,
                      int N,
                      int col,
                      int queens,
                      vector<vector<string>> &solutions)
    {
        if (queens == 0)
        {
            auto solution = boardToString(board, N);
            solutions.push_back(solution);
            return;
        }

        for (int row = 0; row < N; ++row)
        {
            if (row < 0 || row >= N || col < 0 || col >= N ||
                rowHasQueen[row] || slashHasQueen[slash[row][col]] || backSlashHasQueen[backSlash[row][col]])
            {
                continue;
            }

            board[row][col] = true;
            rowHasQueen[row] = true;
            slashHasQueen[slash[row][col]] = true;
            backSlashHasQueen[backSlash[row][col]] = true;

            solveNQueens(board, rowHasQueen, slash, slashHasQueen, backSlash, backSlashHasQueen,
                            N, col + 1, queens - 1, solutions);

            board[row][col] = false;
            rowHasQueen[row] = false;
            slashHasQueen[slash[row][col]] = false;
            backSlashHasQueen[backSlash[row][col]] = false;
        }
    }

public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> solutions;

        bool **board = new bool*[n];
        for (int row = 0; row < n; ++row)
        {
            board[row] = new bool[n];
            for (int col = 0; col < n; ++col)
            {
                board[row][col] = false;
            }
        }

        bool *rowHasQueen = new bool[n];
        std::fill_n(&rowHasQueen[0], n, false);

        bool *slashHasQueen = new bool[2*n];
        std::fill_n(&slashHasQueen[0], 2*n, false);

        bool *backSlashHasQueen = new bool[2*n];
        std::fill_n(&backSlashHasQueen[0], 2*n, false);

        int **backSlash = new int*[n];
        for (int row = 0; row < n; ++row)
        {
            backSlash[row] = new int[n];
            for (int col = 0; col < n; ++col)
            {
                backSlash[row][col] = row - col + n;
            }
        }

        int **slash = new int*[n];
        for (int row = 0; row < n; ++row)
        {
            slash[row] = new int[n];
            for (int col = 0; col < n; ++col)
            {
                slash[row][col] = row + col + 1;
            }
        }

        solveNQueens(board,
                        rowHasQueen, slash, slashHasQueen, backSlash, backSlashHasQueen,
                        n, 0, n, solutions);

        for (int i = 0; i < n; ++ i)
        {
            delete [] board[i];
        }
        delete [] board;
        delete [] rowHasQueen;
        delete [] slashHasQueen;
        delete [] backSlashHasQueen;

        for (int i = 0; i < n; ++i)
        {
            delete [] backSlash[i];
        }
        delete [] backSlash;

        for (int i = 0; i < n; ++i)
        {
            delete [] slash[i];
        }
        delete [] slash;

        return solutions;
    }
};


bool eightQueenCheckSpot(const array<int, 8> &colToRowQueen, int row, int col)
{
    for (int prevCol = 0; prevCol < col; ++prevCol)
    {
        int prevRow = colToRowQueen[prevCol];
        if (prevRow == row)
        {
            return false; // same row as previous queen seleted
        }

        // check slope for diagonal
        if ((col - prevCol) == abs(row - prevRow))
        {
            return false; // same diagonal
        }
    }
    return true;
}
void eightQueen(array<int, 8> &colToRowQueen, int col, vector<array<int, 8>> &results)
{
    if (col >= 8)
    {
        results.push_back(colToRowQueen);
    }
    else
    {
        for (int row = 0; row < 8; ++row)
        {
            if (eightQueenCheckSpot(colToRowQueen, row, col))
            {
                colToRowQueen[col] = row;
                eightQueen(colToRowQueen, col + 1, results);
            }
        }
    }
}
void eightQueen()
{
    array<int, 8> colToRowQueen = {-1,-1,-1,-1,-1,-1,-1,-1};
    vector<array<int,8>> results;
    eightQueen(colToRowQueen, 0, results);

    cout << "eightQueen: " << results.size() << endl;
}


struct Box
{
    int width;
    int height;
    int depth;

    Box(int width, int height, int depth) : width(width), height(height), depth(depth) {}

    bool operator < (const Box &b) const
    {
        return height < b.height;
    }
};
int stackOfBoxes(const vector<Box> &boxes, unordered_set<int> &processedIndex, vector<int> &hash,
        const Box * const top)
{
    if (processedIndex.size() == boxes.size())
    {
        return 0;
    }

    int maxHeight = 0;
    for (uint i = 0; i < boxes.size(); ++i)
    {
        if (processedIndex.find(i) != processedIndex.end())
        {
            continue;
        }

        const Box * const newBox = &(boxes[i]);
        if (top == nullptr ||
                (newBox->width <= top->width &&
                newBox->height <= top->height &&
                newBox->depth <= top->depth))
        {
            processedIndex.insert(i);
            if (hash[i] == 0)
            {
                int height = newBox->height + stackOfBoxes(boxes, processedIndex, hash, newBox);
                if (hash[i] < height)
                {
                    hash[i] = height;
                }
                if (maxHeight < height)
                {
                    maxHeight = height;
                }
            }
            else
            {
                maxHeight = hash[i];
            }

            processedIndex.erase(i);
        }
    }

    return maxHeight;
}

int stackOfBoxesIterative(const vector<Box> &boxes, int bottomBoxIdx, unordered_map<int,int> &hash)
{
    // check if already found max for prev
    if (hash.find(bottomBoxIdx) != hash.end())
    {
        return hash[bottomBoxIdx];
    }

    int maxHeight = 0;
    for (int boxId = bottomBoxIdx - 1; boxId >= 0; --boxId)
    {
        // Check if can add box to stack
        if (boxes[bottomBoxIdx].depth >= boxes[boxId].depth &&
                boxes[bottomBoxIdx].height >= boxes[boxId].height &&
                boxes[bottomBoxIdx].width >= boxes[boxId].width)
        {
            int height = stackOfBoxesIterative(boxes, boxId, hash);
            if (height > maxHeight)
            {
                maxHeight = height;
            }
        }
    }
    maxHeight += boxes[bottomBoxIdx].height;
    hash[bottomBoxIdx] = maxHeight;
    return maxHeight;
}

int createStack(vector<Box> boxes, int bottomIndex, vector<int> &stackMap)
{
    if (bottomIndex >= 0 && stackMap[bottomIndex] != -1)
    {
        return stackMap[bottomIndex];
    }

    Box bottom = boxes[bottomIndex];
    int maxHeight = 0;
    for (int i = bottomIndex - 1; i >= 0; --i)
    {
        if (boxes[i].height <= bottom.height &&
                boxes[i].depth <= bottom.depth &&
                boxes[i].width <= bottom.width)
        {
            int height = createStack(boxes, i, stackMap);
            maxHeight = (maxHeight < height) ? height : maxHeight;
        }
    }
    maxHeight += bottom.height;
    stackMap[bottomIndex] = maxHeight;
    return maxHeight;
}
int createStack(const vector<Box> &boxes)
{
    int maxHeight = 0;
    vector<int> stackMap;
    for (uint i = 0; i < boxes.size(); ++i)
    {
        stackMap.push_back(-1);
    }

    for (int i = boxes.size() - 1; i >= 0; --i)
    {
        int height = createStack(boxes, i, stackMap);
        maxHeight = (maxHeight < height) ? height : maxHeight;
    }

    return maxHeight;
}

void stackOfBoxes()
{
    vector<Box> boxes;
    for (int width = 1; width <= 5; ++width)
    {
        for (int height = 1; height <= 5; ++height)
        {
            for (int depth = 1; depth <= 5; ++depth)
            {
                boxes.push_back(Box{width, height, depth});
            }
        }
    }
    /*boxes.push_back(Box{1,1,1});
    boxes.push_back(Box{1,2,1});
    boxes.push_back(Box{2,1,1});
    boxes.push_back(Box{1,1,2});
    boxes.push_back(Box{1,7,1});
    boxes.push_back(Box{2,2,2});*/

    sort(boxes.begin(), boxes.end());
    unordered_set<int> processedIndexs;
    vector<int> hash;
    for (uint i = 0; i < boxes.size(); ++i)
    {
        hash.push_back(0);
    }
    cout << "stackOfBoxes: " << stackOfBoxes(boxes, processedIndexs, hash, nullptr) << endl;

    unordered_map<int,int> hashBottomToHeight;
    int maxHeight = 0;
    for (int i = boxes.size() - 1; i >= 0; --i)
    {
        int height = stackOfBoxesIterative(boxes, i, hashBottomToHeight);
        if (height > maxHeight)
        {
            maxHeight = height;
        }
    }
    cout << "stackOfBoxesIterative: " << maxHeight << endl;

    cout << "createStack: " << createStack(boxes) << endl;
}


int boolEval(string expression, bool result, unordered_map<string, int> &hash)
{
    if (expression.empty())
    {
        return 0;
    }
    else if (expression.size() == 1)
    {
        return ((int)(expression[0] - '0') == result) ?  1 : 0;
    }
    else if (hash.find(expression + to_string(result)) != hash.end())
    {
        return hash[expression + to_string(result)];
    }

    // Loop over
    int count = 0;
    for (uint i = 1; i < expression.size(); i += 2)
    {
        string left = expression.substr(0, i);
        string right = expression.substr(i + 1, expression.size());
        char operand = expression[i];

        int leftTrue = boolEval(left, true, hash);
        int leftFalse = boolEval(left, false, hash);
        int rightTrue = boolEval(right, true, hash);
        int rightFalse = boolEval(right, false, hash);

        switch(operand)
        {
        case '&':
            if (result)
            {
                count += leftTrue * rightTrue;
            }
            else
            {
                count += leftTrue * rightFalse;
                count += leftFalse * rightTrue;
                count += leftFalse * rightFalse;
            }
            break;
        case '|':
            if (result)
            {
                count += leftTrue * rightTrue;
                count += leftFalse * rightTrue;
                count += leftTrue * rightFalse;
            }
            else
            {
                count += leftFalse * rightFalse;
            }
            break;
        case '^':
            if (result)
            {
                count += leftFalse * rightTrue;
                count += leftTrue * rightFalse;
            }
            else
            {
                count += leftTrue * rightTrue;
                count += leftFalse * rightFalse;
            }
            break;
        default:
            throw "Unknown expression";
            break;
        }
    }

    hash[expression + to_string(result)] = count;
    return count;
}

void chap8RecusionMain()
{
    cout << "Fib: " << fibMem(10) << " " << fib(10) << endl;

    // Triple Step
    {
        int steps[] = {1, 2, 3};
        const int numSteps = sizeof(steps) / sizeof(int);
        int numStairs = 4;
        int mem[numStairs + 1]; fill_n(mem, numStairs + 1, -1);
        int possibles = numPossibleSteps<numSteps>(numStairs, steps, mem);
        cout << "tripleStep: " << possibles << endl;
        assert(7 == possibles);

        possibles = numPossibleStepsMem<3>(4, steps);
        cout << "tripleStepMem: " << possibles << endl;
        assert(7 == possibles);
    }

    // Robot
    {
        const int rows = 4;
        const int cols = 3;
        bool grid[rows][cols];
        fill_n(&grid[0][0], rows * cols, true);

        srand (time(NULL));
        int numWalls = 2;

        for (int i = 0; i < numWalls; ++i)
        {
            grid[0][i] = false;
        }
        random_shuffle(&grid[0][0], &grid[rows - 1][cols - 1]);

        vector<pair<int,int>> path;
        cout << "FindRobotPath: " << findRobotPath<rows, cols>(grid, make_pair(0,0), path) << endl;
        reverse(path.begin(), path.end());
        for (auto pair : path)
        {
            cout << "(" << pair.first << ", " << pair.second << "), ";
        }
        cout << endl;
        path.clear();

        cout << "FindRobotPathRecurse: " << findRobotPathRecurise<rows, cols>(grid, rows - 1, cols - 1, path) << endl;
        for (auto pair : path)
        {
            cout << "(" << pair.first << ", " << pair.second << "), ";
        }
        cout << endl;
        path.clear();

        unordered_set<pair<int,int>, PairHash, PairCompare> failedPoints;
        cout << "findRobotPathRecuriseMem: " <<
                findRobotPathRecuriseMem<rows, cols>(grid, rows - 1, cols - 1, path, failedPoints) << endl;
        for (auto pair : path)
        {
            cout << "(" << pair.first << ", " << pair.second << "), ";
        }
        cout << endl;
        path.clear();
    }

    {
        vector<int> items = {1, 2, 3};
        auto sets = powerSetIterative(items);
        for (auto set : sets)
        {
            cout << "{";
            for (auto item : set)
            {
                cout << item << ",";
            }
            cout << "},";
        }
        cout << endl;
    }
    {
        vector<int> items = {1, 2, 3};
        auto sets = powerSetRecrsive(items, 0);
        for (auto set : sets)
        {
            cout << "{";
            for (auto item : set)
            {
                cout << item << ",";
            }
            cout << "},";
        }
        cout << endl;
    }
    {
        vector<int> items = {1, 2, 3};
        auto sets = powerSetCombinatoric(items);
        for (auto set : sets)
        {
            cout << "{";
            for (auto item : set)
            {
                cout << item << ",";
            }
            cout << "},";
        }
        cout << endl;
    }
    {
        assert((23*27) == recursiveMultiply(23, 27));
    }

    {
        Tower *towers[3];
        for (int i = 0; i < 3; ++i)
        {
            towers[i] = new Tower(i);
        }

        for (int i = 5; i >= 0; --i)
        {
            towers[0]->add(i);
        }
        towers[0]->moveDisks(5, towers[2], towers[1]);

        // delete
        for (int i = 0; i < 3; ++i)
        {
            delete towers[i];
        }
    }
    {
        array<stack<int>, 3> towers;
        for (int i = 5; i >= 1; --i)
        {
            towers[0].push(i);
        }
        towerOfHanoiMine(towers, 5, 0, 1, 2);

        while (!towers[2].empty())
        {
            cout << towers[2].top() << " ";
            towers[2].pop();
        }
        cout << endl;
    }

    {
        vector<string> permList = permNoDups("abcd");
        for (auto perm : permList)
        {
            cout << perm << " ";
        }
        cout << endl;
        assert(permList.size() == 24);
    }
    {
        vector<string> permList = permNoDupsIterative("abcd");
        for (auto perm : permList)
        {
            cout << perm << " ";
        }
        cout << endl;
        assert(permList.size() == 24);
    }
    {
        vector<string> permList = getPermsRecursive("abcd");
        for (auto perm : permList)
        {
            cout << perm << " ";
        }
        cout << endl;
        assert(permList.size() == 24);
    }
    {
        vector<string> permList = permWithDups("aab");
        for (auto perm : permList)
        {
            cout << perm << " ";
        }
        cout << endl;
        assert(permList.size() == 3);
    }

    {
        auto comboList = parenthises(3);
        int i = 1;
        for (auto combo : comboList)
        {
            cout << i << ": " << combo << ", ";
            ++i;
        }
        cout << endl;
    }
    {
        auto comboList = parenthisesBuild(3);
        int i = 1;
        for (auto combo : comboList)
        {
            cout << i << ": " << combo << ", ";
            ++i;
        }
        cout << endl;
    }

    {
        const int rows = 3;
        const int cols = 3;
        int grid[rows][cols] = {
                {1, 2, 2},
                {3, 2, 2},
                {2, 2, 2}
        };
        paintColor<rows, cols>(grid, 1, 1, grid[1][1], 4);
        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                cout << grid[row][col] << " ";
            }
            cout << endl;
        }
    }

    {
        vector<int> coins = {25, 10, 5, 1};
        unordered_map<int,unordered_map<int,int>> hash;
        cout << "Coins: " << coinsPerm(coins, 0, 37, hash) << endl;
        cout << "Coins: " << makeChange(37) << endl;
    }

    {
        Solution s;
        vector<vector<string>> result = s.solveNQueens(8);
        cout << "solveNQueens: " << result.size() << endl;
        /*for (auto board : result)
        {
            cout << "Board: " << endl;
            for (auto row : board)
            {
                cout << row << endl;
            }
        }*/

        eightQueen();
    }

    {
        stackOfBoxes();
    }

    {
        unordered_map<string, int> hash;
        cout << "boolEval: " << boolEval("1^0|0|1", false, hash) << endl;
        cout << "boolEval: " << boolEval("0&0&0&1^1|0", true, hash) << endl;
    }
}
