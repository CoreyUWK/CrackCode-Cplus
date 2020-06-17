/*
 * Chap4TreesGraphs.cpp
 *
 *  Created on: May 4, 2020
 *      Author: ckirsch
 */
#include "Chap4TreesGraphs.h"
#include <iostream>
#include <cassert>
#include <stack>
#include <vector>
#include <queue>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <stdlib.h>     /* srand, rand */
#include <forward_list>
#include <list>
#include <algorithm>    // std::reverse
#include <functional>
#include <cmath>
#include <limits>
using namespace std;

class Graph
{
public:

    class GraphNode
    {
    public:
        string name;
        list<GraphNode*> friends;
        int refCount; // for delete

        GraphNode(string name) : name(name), refCount(1) {}
        friend Graph;
    };

    GraphNode *center;
    static const int numFriends = 3;
    static const int layers = 3;

    static void CreateGraph(GraphNode *root, int layer = 1)
    {
        if (layer == layers)
        {
            return;
        }

        for (int j = 0; j < numFriends; ++j)
        {
            GraphNode *newNode = new GraphNode(to_string(layer) + "." + to_string(j));
            root->friends.push_back(newNode);
            newNode->friends.push_back(root);
            CreateGraph(root->friends.back(), layer + 1);
        }
    }

    Graph()
    {
        center = new GraphNode("0");
        Graph::CreateGraph(center);
    }

    ~Graph()
    {
        unordered_set<GraphNode*> visited;
        stack<GraphNode*> st;

        st.push(center);
        visited.insert(center);

        while (!st.empty())
        {
            auto top = st.top();

            while (!top->friends.empty())
            {
                bool hasVisit = false;
                for (auto it = top->friends.begin(); it != top->friends.end(); ++it)
                {
                    GraphNode *f = *it;
                    if (visited.find(f) == visited.end())
                    {
                        st.push(f);
                        visited.insert(f);
                        hasVisit = true;
                        break;
                    }
                }
                top = st.top();

                if (!hasVisit) break;
            }

            auto removeNode = st.top();
            st.pop();
            //cout << "deleted: " << removeNode << " " << removeNode->name << endl;
            --(removeNode->refCount);
            if (removeNode->refCount <= 0)
                delete removeNode;
        }
    }

    void updateRefCount(const int amount, GraphNode *node, unordered_set<GraphNode*> &visited)
    {
        if (nullptr == node)
        {
            return;
        }

        visited.insert(node);
        for (auto it = node->friends.begin(); it != node->friends.end(); ++it)
        {
            if (visited.find(*it) == visited.end())
            {
                updateRefCount(amount, *it, visited);
            }
        }

        node->refCount += amount;
    }

    void join(Graph &g)
    {
        GraphNode *g1 = center;
        GraphNode *g2 = g.center;

        GraphNode *joinNode = new GraphNode(to_string(layers));

        unordered_set<GraphNode *> hash;
        hash.insert(g1);
        while (!g1->friends.empty())
        {
            bool foundFriend = false;
            for (auto f : g1->friends)
            {
                if (hash.find(f) == hash.end())
                {
                    g1 = f;
                    hash.insert(g1);
                    foundFriend = true;
                    break;
                }
            }

            if (!foundFriend) break;
        }
        g1->friends.push_back(joinNode);
        joinNode->friends.push_back(g1);

        hash.insert(joinNode);
        hash.insert(g2);
        while (!g2->friends.empty())
        {
            bool foundFriend = false;
            for (auto f : g2->friends)
            {
                if (hash.find(f) == hash.end())
                {
                    g2 = f;
                    hash.insert(g2);
                    foundFriend = true;
                    break;
                }
            }
            if (!foundFriend) break;
        }

        joinNode->friends.push_back(g2);
        g2->friends.push_back(joinNode);


        // increase refCount
        hash.clear();
        updateRefCount(1, g2, hash);
    }

    static void PrintGraphBFS(GraphNode *root)
    {
        if (root == nullptr)
        {
            return;
        }

        queue<GraphNode*> q;
        unordered_set<GraphNode*> hash;

        q.push(root);
        hash.insert(root);
        q.push(nullptr); // for print newline
        while (!q.empty())
        {
            GraphNode *top = q.front();
            q.pop();

            if (top == nullptr)
            {
                cout << endl;
                continue;
            }

            cout << top << "-" << top->name << " ";
            hash.insert(top);

            bool addNodes = false;
            for (GraphNode *n : top->friends)
            {
                if (hash.find(n) == hash.end())
                {
                    q.push(n);
                    addNodes = true;
                }
            }

            if (addNodes)
            {
                q.push(nullptr);
            }
        }
    }

    static void findDFS(queue<GraphNode*> &q, unordered_set<GraphNode*> &visited,
            unordered_map<GraphNode*, GraphNode*> &parent)
    {
        auto top = q.front();
        q.pop();

        for (auto it = top->friends.begin(); it != top->friends.end(); ++it)
        {
            GraphNode *f = *it;
            if (visited.find(f) == visited.end())
            {
                q.push(f);
                visited.insert(f);
                parent[f] = top;
            }
        }
    }

    static vector<GraphNode*> findBidirectional(GraphNode *n1, GraphNode *n2)
    {
        queue<GraphNode*> q1;
        queue<GraphNode*> q2;

        unordered_set<GraphNode*> visited1;
        unordered_set<GraphNode*> visited2;

        unordered_map<GraphNode*, GraphNode*> parent1;
        unordered_map<GraphNode*, GraphNode*> parent2;

        q1.push(n1);
        visited1.insert(n1);

        q2.push(n2);
        visited2.insert(n2);

        GraphNode *overlap = nullptr;
        while (!q1.empty() && !q2.empty())
        {
            findDFS(q1, visited1, parent1);
            findDFS(q2, visited2, parent2);

            // Improve this performance O(n)
            // Find if any overlap
            for (auto it = visited1.begin(); it != visited1.end(); ++it)
            {
                if (visited2.find(*it) != visited2.end())
                {
                    overlap = *it;
                    goto path;
                }
            }
        }
        path:
        vector<GraphNode*> path;
        if (overlap != nullptr)
        {
            GraphNode *parent = overlap;
            while (parent != nullptr && parent != n1)
            {
                path.push_back(parent);
                parent = (parent1.find(parent) == parent1.end()) ? nullptr : parent1[parent];
            }
            if (parent == n1) path.push_back(parent);
            std::reverse(path.begin(), path.end());

            parent = overlap;
            parent = (parent2.find(parent) == parent2.end()) ? nullptr : parent2[parent];
            while (parent != nullptr && parent != n2)
            {
                path.push_back(parent);
                parent = (parent2.find(parent) == parent2.end()) ? nullptr : parent2[parent];
            }
            if (parent == n2) path.push_back(parent);
        }
        return path;
    }
};

class Graph2
{
    unordered_map<string, list<string>> adjList;

public:
    void addEdge(string nodeU, string nodeV)
    {
        adjList[nodeU].push_back(nodeV);
    }

    bool routeExists(string nodeU, string nodeV)
    {
        unordered_set<string> visited;
        queue<string> q;

        q.push(nodeU);
        visited.insert(nodeU);

        bool found = false;
        while (!q.empty())
        {
            string top = q.front();
            q.pop();

            // Found
            if (top == nodeV)
            {
                found = true;
                goto found;
            }

            if (adjList.find(top) != adjList.end())
            {
                for (auto it = adjList[top].begin(); it != adjList[top].end(); ++it)
                {
                    if (top == *it)
                    {
                        found = true;
                        goto found;
                    }

                    q.push(*it);
                    visited.insert(*it);
                }
            }
        }

found:
        return found;
    }
};


class TreeNode
{
    static void printTree (TreeNode *root, TreeNode *n)
    {
        if (n == nullptr) return;
        cout << n->value << " ";
        printTree(root, n->left);
        printTree(root, n->right);

        if (root == n)
        {
            cout << endl;
        }
    }

    /* Optimized isBalanced
    * Time: O(n), Space: O(logn)*/
    static int isBalancedOptimized (TreeNode *n)
    {
        if (nullptr == n)
        {
            return -1;
        }

        int leftHeight = isBalancedOptimized(n->left);
        int rightHeight = isBalancedOptimized(n->right);
        if (leftHeight == numeric_limits<int>::min() ||
              rightHeight == numeric_limits<int>::min() ||
              1 < abs(leftHeight - rightHeight))
        {
            return numeric_limits<int>::min();
        }
        return max(leftHeight, rightHeight) + 1;
    }

    // Find next inorder by moving up the tree (find parent with node being left branch)
    static TreeNode *getNextInOrderParentRecurse(TreeNode *node)
    {
        if (nullptr == node || nullptr == node->parent)
        {
            return nullptr;
        }

        if (node == node->parent->left)
        {
            return node->parent;
        }
        return getNextInOrderParentRecurse(node->parent);
    }
    static TreeNode *getNextInOrderParent(TreeNode *node)
    {
        if (nullptr == node || nullptr == node->parent)
        {
            return nullptr;
        }

        TreeNode *p = node->parent;
        while (p != nullptr && p->left != node)
        {
            node = p;
            p = p->parent;
        }
        return p;
    }

    static TreeNode *getNextInOrderLeftRecurse(TreeNode *node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        else if (nullptr == node->left)
        {
            return node;
        }
        return getNextInOrderLeftRecurse(node->left);
    }
    static TreeNode *getNextInOrderLeft(TreeNode *node)
    {
        if (node == nullptr) return nullptr;
        while (nullptr != node->left)
        {
            node = node->left;
        }
        return node;
    }

    TreeNode *getRandNodeHelper(int idx)
    {
        if (this->left == nullptr)
        {
            if (1 == idx)
            {
                return this;
            }
            else
            {
                return this->right->getRandNodeHelper(idx - 1);
            }
        }

        int curIdx = this->left->size + 1;
        if (curIdx == idx)
        {
            return this;
        }
        else if (idx < curIdx)
        {
            return this->left->getRandNodeHelper(idx);
        }
        else
        {
            return this->right->getRandNodeHelper(idx - curIdx);
        }
    }

public:
    int value;
    TreeNode *parent;
    TreeNode *left;
    TreeNode *right;
    int size; // number of nodes in tree

    TreeNode(int value, TreeNode *left = nullptr, TreeNode *right = nullptr) :
        value(value), parent(nullptr), left(left), right(right), size(1) {}

    ~TreeNode()
    {
        if (left != nullptr) delete left;
        if (right != nullptr) delete right;
    }

    static TreeNode * arrayToTree(int arr[], int start, int end)
    {
        if (start > end)
        {
            return nullptr;
        }

        int mid = start + (end - start) / 2;
        TreeNode *root = new TreeNode(arr[mid]);
        root->left = arrayToTree(arr, start, mid - 1);
        if (nullptr != root->left)
        {
            root->left->parent = root;
            root->size += root->left->size;
        }

        root->right = arrayToTree(arr, mid + 1, end);
        if (nullptr != root->right)
        {
            root->right->parent = root;
            root->size += root->right->size;
        }

        return root;
    }

    static void printTree (TreeNode *root)
    {
        printTree(root, root);
    }

    static bool isBalanced(TreeNode *root)
    {
        return numeric_limits<int>::min() != isBalancedOptimized(root);
    }

    static void inOrder(TreeNode *root, vector<int> &inOrderList)
    {
        if (root == nullptr)
        {
            return;
        }

        inOrder(root->left, inOrderList);
        inOrderList.push_back(root->value);
        inOrder(root->right, inOrderList);
    }

    // Requires tree have no duplicates
    // Time: O(n), Space: O(n)
    static bool isBstNoDuplicate(TreeNode *root)
    {
        vector<int> inOrderList;
        inOrder(root, inOrderList);

        for (uint i = 1; i < inOrderList.size(); ++i)
        {
            if (inOrderList[i - 1] >= inOrderList[i])
            {
                return false;
            }
        }

        return true;
    }

    // This is same as isBstNoDuplicate above however not tracking all in vector as only previous is needed
    // Time: O(n) Space: O(1)
    static bool checkInOrder(TreeNode *root, int **prevVal)
    {
        if (prevVal == nullptr)
        {
            throw "Invalid prevVal arguments";
        }
        else if (root == nullptr)
        {
            return true;
        }

        if (!checkInOrder(root->left, prevVal)) return false;

        if (nullptr != *prevVal && **prevVal >= root->value)
        {
            return false;
        }
        *prevVal = &(root->value);

        if (!checkInOrder(root->right, prevVal)) return false;

        return true;
    }

    static bool isBst(TreeNode *root,
            int min = numeric_limits<int>::min(), int max = numeric_limits<int>::max())
    {
        if (root == nullptr)
        {
            return true;
        }

        if (root->value < min || max < root->value)
        {
            return false;
        }

        return isBst(root->left, min, root->value) && isBst(root->right, root->value + 1, max);
    }

    static TreeNode * getNextInOrder(TreeNode *node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (nullptr == node->left && nullptr == node->right)
        {   // Child node
            return getNextInOrderParent(node);
        }
        else if (nullptr != node->right)
        {   // Next is right of parent
            return getNextInOrderLeft(node->right);
        }
        // No right exists so get next parent
        return getNextInOrderParent(node);
    }

    TreeNode * find(int val)
    {
        if (this->value == val)
        {
            return this;
        }

        TreeNode* foundLeft = nullptr;
        if (this->left != nullptr && nullptr != (foundLeft = this->left->find(val)))
        {
            return foundLeft;
        }

        TreeNode* foundRight = nullptr;
        if (this->right != nullptr && nullptr != (foundRight = this->right->find(val)))
        {
            return foundRight;
        }

        return nullptr;
    }

    /* in Balanced tree is O(logn) time */
    TreeNode * getRandNode()
    {
        int idx = (rand() % size) + 1;
        return getRandNodeHelper(idx);
    }
};

//-----------------------------------------Tests--------------------------------------
void ConnectFriendsTest()
{
    Graph g1;
    Graph g2;

    g1.join(g2);

    Graph::PrintGraphBFS(g1.center);
    auto path = Graph::findBidirectional(g1.center, g2.center);

    cout << "ConnectFriendsTest: ";
    for (auto node : path)
    {
        cout << node->name << " ";
    }
    cout << endl;
}

void RouteBetweeenNodesTest()
{
    Graph2 g;

    g.addEdge("A", "B");
    g.addEdge("B", "H"); g.addEdge("B", "C");
    g.addEdge("C", "D");
    g.addEdge("H", "G");
    g.addEdge("G", "L"); g.addEdge("G", "C"); g.addEdge("G", "F");
    g.addEdge("F", "E");
    g.addEdge("E", "C");

    assert(true == g.routeExists("H", "D"));
    cout << "RouteBetweeenNodesTest: Passed" << endl;
}


void MinimalTreeTest()
{
    int arr[] = {1,2,3,4,5,6,7,8};
    const int size = sizeof(arr) / sizeof(int);

    // Function to turn array into minimal binary search tree
    std::function<TreeNode*(int,int)> minimalTree;
    minimalTree = [&minimalTree, arr](int start, int end) -> TreeNode*
    {
       if (start > end)
       {
           return nullptr;
       }

       int mid = start + (end - start) / 2;
       TreeNode *root = new TreeNode(arr[mid]);
       root->left = minimalTree(start, mid - 1);
       root->right = minimalTree(mid + 1, end);
       return root;
    };

    // function to print tree
    function<void(TreeNode*)> printTree = [&printTree](TreeNode *root)
    {
       if (root == nullptr) return;
       cout << root->value << " ";
       printTree(root->left);
       printTree(root->right);
    };

    TreeNode *tree = minimalTree(0, size - 1);
    // Note: could validate is binary search tree
    printTree(tree); cout << endl;

    // Tree to Linked List using depth first search by level
    vector<list<TreeNode*>> listsDfs;
    function<void(TreeNode*,int)> makeListDFS = [&makeListDFS, &listsDfs](TreeNode *root, uint level)
    {
        if (root == nullptr) return;

        list<TreeNode*> list;
        if (level >= listsDfs.size())
        {
            listsDfs.push_back(list);
        }
        listsDfs[level].push_back(root);

        makeListDFS(root->left, level + 1);
        makeListDFS(root->right, level + 1);
    };

    // Tree to linked list using breadth first search per level
    vector<list<TreeNode*>> listsBfs;
    auto makeListBFS = [&listsBfs](TreeNode *root)
    {
        if (root == nullptr) return;

        list<TreeNode*> current;
        current.push_back(root);

        while (!current.empty())
        {
            listsBfs.push_back(current);
            list<TreeNode*> parent = current;
            current.clear();
            for (auto it = parent.begin(); it != parent.end(); ++it) // parent/cur list acting as queue -> FIFO
            {
                TreeNode *n = *it;
                if (nullptr != n->left) current.push_back(n->left);
                if (nullptr != n->right) current.push_back(n->right);
            }
        }
    };

    auto printLinkedList = [](const vector<list<TreeNode*>> &lists)
    {
        // print linked list
        //cout << "Tree to list" << endl;
        for (uint i = 0; i < lists.size(); ++i)
        {
            for (auto it = lists[i].begin(); it != lists[i].end(); ++it)
            {
                cout << (*it)->value << " ";
            }
            cout << endl;
        }
    };

    makeListDFS(tree, 0);
    cout << "Tree->LinkedList DFS:" << endl;
    printLinkedList(listsDfs);
    makeListBFS(tree);
    cout << "Tree->LinkedList BFS:" << endl;
    printLinkedList(listsBfs);


    /* according this is O(nlogn) maybe cause we run isBalanced on all nodes which then call getheight in logn
     * so O(n*logn)
     * but I believe its O(n^2) */
    // Check balanced
    function<int(TreeNode*)> getHeight = [&getHeight](TreeNode *root) -> int
    {
        if (root == nullptr)
        {
            return -1;
        }

        return std::max(getHeight(root->left), getHeight(root->right)) + 1;
    };

    function<bool(TreeNode*)> isBalanced = [&isBalanced, &getHeight](TreeNode *root) -> bool
    {
        if (root == nullptr)
        {
            return true;
        }

        if (1 < abs(getHeight(root->left) - getHeight(root->right)))
        {
            return false;
        }
        return isBalanced(root->left) && isBalanced(root->right);
    };

    /* Optimized isBalanced
     * Time: O(n), Space: O(logn)*/
    function<int(TreeNode*)> isBalancedOptimized = [&isBalancedOptimized] (TreeNode *n) -> int
    {
        if (nullptr == n)
        {
            return -1;
        }

        int leftHeight = isBalancedOptimized(n->left);
        int rightHeight = isBalancedOptimized(n->right);
        if (leftHeight == numeric_limits<int>::min() ||
                rightHeight == numeric_limits<int>::min() ||
                1 < abs(leftHeight - rightHeight))
        {
            return numeric_limits<int>::min();
        }
        return max(leftHeight, rightHeight) + 1;
    };

    assert(true == isBalanced(tree));
    assert(-1 <= isBalancedOptimized(tree));

    // Add extra nodes to unbalance tree
    TreeNode *n = tree;
    while (n->left != nullptr)
    {
        n = n->left;
    }
    n->left = new TreeNode(0);
    n->left->left = new TreeNode(0);
    assert(false == isBalanced(tree));
    assert(numeric_limits<int>::min() == isBalancedOptimized(tree));
    cout << "Tree Balanced: Pass" << endl;

    // Delete Tree
    delete tree;
}

void isBstTest()
{
    const int N = 10;
    vector<int> arr;

    // Setup array -> tree
    srand (time(NULL));
    unordered_set<int> hash;
    for (int i = 0; i < N; ++i)
    {
        int value = rand() % 100;
        if (hash.find(value) == hash.end())
        {
            arr.push_back(value);
            hash.insert(value);
        }
    }
    sort(arr.begin(), arr.end());
    TreeNode *root = TreeNode::arrayToTree(arr.data(), 0, arr.size() - 1);

    // Check is BST
    assert(TreeNode::isBstNoDuplicate(root));
    int *prevPtr = nullptr;
    assert(TreeNode::checkInOrder(root, &prevPtr));
    assert(TreeNode::isBst(root));
    delete root;

    // Test negative
    arr.push_back(0);
    root = TreeNode::arrayToTree(arr.data(), 0, arr.size() - 1);
    assert(!TreeNode::isBstNoDuplicate(root));
    prevPtr = nullptr;
    assert(!TreeNode::checkInOrder(root, &prevPtr));
    assert(!TreeNode::isBst(root));
    delete root;

    cout << "IsBst: Pass" << endl;
}


void getNextInorderTestTree(TreeNode *root, int *hash, int hashSize)
{
    if (root == nullptr)
    {
        return;
    }

    getNextInorderTestTree(root->left, hash, hashSize);
    if (root->value < hashSize)
    {
        assert(hash[root->value] == TreeNode::getNextInOrder(root)->value);
    }
    else
    {
        assert(nullptr == TreeNode::getNextInOrder(root));
    }
    getNextInorderTestTree(root->right, hash, hashSize);
}

void getNextInorderTest()
{
    int arr[] = {0, 1, 2, 3, 4, 5, 6};
    TreeNode *root = TreeNode::arrayToTree(arr, 0, 6);

    int hash[] = {1, 2, 3, 4, 5, 6};

    getNextInorderTestTree(root, hash, 6);

    delete root;
    cout << "getNextInorderTest: Pass" << endl;
}


vector<string> buildOrderWithCount(const vector<string> &projects, const vector<pair<string,string>> &dependices)
{
    // Create dependency list
    unordered_map<string, int> dependCount;
    for (auto project : projects)
    {
        dependCount[project] = 0;
    }
    for (auto pair : dependices)
    {
        ++dependCount[pair.second];
    }

    // Get build order
    vector<string> order;
    while (order.size() < projects.size())
    {
        bool found = false;
        for (auto it = dependCount.begin(); it != dependCount.end();)
        {
            auto countPair = *it;
            ++it;

            if (0 == countPair.second)
            {
                found = true;

                order.push_back(countPair.first);
                for (auto dependPair : dependices)
                {
                    if (dependPair.first == countPair.first)
                    {
                        --dependCount[dependPair.second];
                    }
                }
                dependCount.erase(countPair.first);
            }
        }

        if (!found) throw "No valid build order";
    }
    return order;
}

vector<string> buildOrderBfs(const vector<string> &projects, const vector<pair<string,string>> &dependices)
{
    unordered_map<string, vector<string>> reverseDependices;
    for (auto project : projects)
    {
        reverseDependices[project] = vector<string>();
    }
    for (auto d : dependices)
    {
        reverseDependices[d.second].push_back(d.first);
    }

    vector<string> order;
    unordered_set<string> built;
    forward_list<string> toBeProcessed;

    // find starting point
    for (auto project : projects)
    {
        if (reverseDependices[project].size() == 0)
        {
            toBeProcessed.push_front(project);
        }
    }

    // Start processing
    while (!toBeProcessed.empty())
    {
        bool haveBuiltAProject = false;

        // Process parent projects
        for (auto it = toBeProcessed.begin(); it != toBeProcessed.end();)
        {
            bool readyToBuild = true;

            // Check to see if parent project ready to be built
            string project = *it;
            for (auto child : reverseDependices[project])
            {
                if (built.find(child) == built.end())
                {
                    readyToBuild = false;
                    break;
                }
            }

            if (readyToBuild)
            {
                order.push_back(project);
                built.insert(project);
                haveBuiltAProject = true;

                // Remove built project
                auto deleteIt = it;
                ++it;
                toBeProcessed.remove(*deleteIt);

                // Add next possible projects to build
                for (auto d : dependices)
                {
                    if (d.first == project)
                    {
                        bool add = true;
                        for (auto p : toBeProcessed)
                        {
                            if (p == d.second)
                            {
                                add = false;
                                break;
                            }
                        }
                        if (add)
                        {
                            toBeProcessed.push_front(d.second);
                        }
                    }
                }
            }
            else
            {
                ++it;
            }
        }

        if (!haveBuiltAProject)
        {
            throw "No valid build order";
        }
    }

    return order;
}

//------------------------------------------------------
class DependencyGraph
{
    class DependencyProject
    {
    public:
        enum State {COMPLETE, PARTIAL, BLANK};

    private:
        string name;
        int dependices;
        State state;
        vector<shared_ptr<DependencyProject>> dependents;

    public:
        DependencyProject(string name) : name(name), dependices(0), state(State::BLANK) {}

        void addDependent(shared_ptr<DependencyProject> node)
        {
            dependents.push_back(node);
            node->increamentDependices();
        }

        string getName() {return name;}
        vector<shared_ptr<DependencyProject>> getDependents() {return dependents;}
        void decrementDependices() {--dependices;}
        void increamentDependices() {++dependices;}
        int getNumDependices() {return dependices;}

        State getState() {return state;}
        void setState(State st) {state = st;}
    };


    vector<shared_ptr<DependencyProject>> nodes;
    unordered_map<string, shared_ptr<DependencyProject>> map;

    /* A helper function for version 1 to insert projects with zero dependencies into there order list */
    bool addNonDependent(vector<string> &order)
    {
        bool added = false;
        for (auto project : nodes)
        {
            if (project->getNumDependices() == 0)
            {
                added = true;

                order.push_back(project->getName());
                for (auto dependent : project->getDependents())
                {
                    dependent->decrementDependices();
                }

                // Double decreament to -1 to prevent double add
                project->decrementDependices();
            }
        }
        return added;
    }

    /* Helper function for version 2 */
    bool doDfs(shared_ptr<DependencyProject> project, stack<string> &st)
    {
        if (project->getState() == DependencyProject::State::PARTIAL)
        {
            return false; // cycle
        }

        if (project->getState() == DependencyProject::State::BLANK)
        {
            project->setState(DependencyProject::State::PARTIAL);
            for (auto dependent : project->getDependents())
            {
                if (!doDfs(dependent, st))
                {
                    return false;
                }
            }
            project->setState(DependencyProject::State::COMPLETE);
            st.push(project->getName());
        }

        return true;
    }


public:
    DependencyGraph(vector<string> projects,
        vector<pair<string,string>> dependencies)
    {
        // Create nodes
        for (auto project : projects)
        {
            nodes.push_back(make_shared<DependencyProject>(project));
            map[project] = nodes.back();
        }

        // Create dependencies
        for (auto dep : dependencies)
        {
            addEdge(dep.first, dep.second);
        }
    }

    /* startName -> endName, that is endName project is dependent on startName */
    void addEdge(string startName, string endName)
    {
        auto start = map[startName];
        auto end = map[endName];

        start->addDependent(end);
    }

    vector<string> buildOrder()
    {
        vector<string> order;

        bool haveAdded = true;
        while (order.size() < nodes.size() && haveAdded)
        {
            haveAdded = addNonDependent(order);
        }

        return order;
    }

    vector<string> buildOrder2()
    {
        vector<string> order;
        stack<string> st;
        for (auto& project : nodes)
        {
            if (!doDfs(project, st))
            {
                return order;
            }
        }

        while (!st.empty())
        {
            order.push_back(st.top());
            st.pop();
        }
        return order;
    }
};


/* The dependency graph performs a topological sort: linear ordering of vertices in a graph, that is a search on the graph taking
 * O(V + E)==O(P + D), P=# projects D=# dependency pairs */
vector<string> findBuildOrder(vector<string> projects,
        vector<pair<string,string>> dependencies)
{
    auto graph = DependencyGraph(projects, dependencies);
    return graph.buildOrder();
}
vector<string> findBuildOrder2(vector<string> projects,
        vector<pair<string,string>> dependencies)
{
    auto graph = DependencyGraph(projects, dependencies);
    return graph.buildOrder2();
}

//--------------------------------------------

void buildOrderTest()
{
    int count = 1;
    auto printOrder = [](int test, vector<string> order) -> void
    {
        cout << "Build Order " << test << ": ";
        for (auto s : order)
        {
            cout << s << " ";
        }
        cout << endl;
    };
    {
        vector<string> projects{"a", "b", "c", "d", "e", "f"};
        vector<pair<string,string>> dependices{
            {"a", "d"},
            {"f", "b"},
            {"b", "d"},
            {"f", "a"},
            {"d", "c"}
        };

        /*
         *   -> a -
         *   |     |
         * ->f     d -> c
         *   |     |
         *   -> b -
         *
         *  f,[a,b],d,c
         */

        vector<string> order1 = buildOrderWithCount(projects, dependices);
        printOrder(count++, order1);

        vector<string> order2 = buildOrderBfs(projects, dependices);
        printOrder(count++, order2);

        vector<string> order3 = findBuildOrder(projects, dependices);
        printOrder(count++, order3);

        vector<string> order4 = findBuildOrder2(projects, dependices);
        printOrder(count++, order4);
    }

    {
        vector<string> projects{"a", "b", "c", "d", "e", "f", "g"};
        vector<pair<string,string>> dependices{
            {"f", "a"},
            {"f", "b"},
            {"f", "c"},
            {"c", "a"},
            {"b", "a"},
            {"b", "e"},
            {"a", "e"},
            {"d", "g"},
        };

        /*     ->  c
         *    |    |
         *    |    V
         * -> f -> a -->
         *    |    ^   |
         *    |    |   V
         *     -> b -> e
         *
         *  -> d-> g
         *  [f,d][c,b,g]a,e
         */

        vector<string> order1 = buildOrderWithCount(projects, dependices);
        printOrder(count++, order1);

        vector<string> order2 = buildOrderBfs(projects, dependices);
        printOrder(count++, order2);

        vector<string> order3 = findBuildOrder(projects, dependices);
        printOrder(count++, order3);

        vector<string> order4 = findBuildOrder2(projects, dependices);
        printOrder(count++, order4);
    }
}

// O(logn)
TreeNode* commonAncestorBst(TreeNode *root, TreeNode *a, TreeNode *b)
{
    if (root == nullptr || a == nullptr || b == nullptr)
    {
        return nullptr;
    }
    else if (a == b)
    {
        return a;
    }

    TreeNode *min = (a->value < b->value) ? a : b;
    while (root != nullptr && min->value > root->value)
    {
        if (min->value > root->value)
        {
            root = root->right;
        }
        else
        {
            root = root->left;
        }
    }

    return root;
}

// O(logn)
TreeNode *commonAncestorParent(TreeNode *root, TreeNode *a, TreeNode *b)
{
    auto getDepth = [&root](TreeNode *node)
    {
        int count = 0;
        while (node != root)
        {
            node = node->parent;
            ++count;
        }
        return count;
    };

    int depthA = getDepth(a);
    int depthB = getDepth(b);
    int diff = abs(depthA - depthB);

    TreeNode *minDepthNode = (depthA > depthB) ? a : b;
    TreeNode *maxDepthNode = (depthA > depthB) ? b : a;
    while (0 < diff)
    {
        maxDepthNode = maxDepthNode->parent;
        --diff;
    }

    // Find common ancestor
    while (minDepthNode != maxDepthNode)
    {
        minDepthNode = minDepthNode->parent;
        maxDepthNode = maxDepthNode->parent;
    }

    return minDepthNode;
}

// Check if tree has nodes
bool covers(TreeNode *root, TreeNode *node)
{
   if (root == nullptr) return false;
   else if (node == nullptr) return false;
   else if (root->value == node->value) return true;

   return covers(root->left, node) || covers(root->right, node);
}

// O(n) as checking all nodes
TreeNode *commonAncestorParent2(TreeNode *root, TreeNode *a, TreeNode *b)
{
    function<TreeNode*(TreeNode*)> sibling = [](TreeNode *node) -> TreeNode*
    {
        if (node == nullptr) return nullptr;
        TreeNode * parent = node->parent;
        TreeNode *sibling = (parent->left == node) ? parent->right : parent->left;

        return sibling;
    };

    /* check nodes are in tree */
    if (!covers(root, a) || !covers(root, b))
    {
        return nullptr;
    }
    // check if a and b are subtrees of each other
    else if (covers(a, b))
    {
        return a;
    }
    else if (covers(b, a))
    {
        return b;
    }

    // Hence, nodes are not in same subtrees
    TreeNode* nodeA = a;
    while (!covers(sibling(nodeA), b))
    {
        nodeA = nodeA->parent;
    }
    return (nodeA == nullptr) ? nullptr : nodeA->parent;
}

stack<TreeNode *> commonAncestorPathHelper(TreeNode *root, TreeNode *node)
{
    stack<TreeNode *> st;
    if (root == nullptr || node == nullptr)
    {
        return st;
    }
    else if (root->value == node->value)
    {
        st.push(node);
        return st;
    }

    stack<TreeNode *> stLeft = commonAncestorPathHelper(root->left, node);
    if (!stLeft.empty())
    {
        stLeft.push(root);
        return stLeft;
    }

    stack<TreeNode *> stRight = commonAncestorPathHelper(root->right, node);
    if (!stRight.empty())
    {
        stRight.push(root);
        return stRight;
    }

    return st;
}
// O(n) for search to get paths and O(logn) space
TreeNode *commonAncestorPath(TreeNode *root, TreeNode *a, TreeNode *b)
{
    if (root == nullptr || a == nullptr || b == nullptr)
    {
        return nullptr;
    }
    else if (a == b)
    {
        return a;
    }

    auto stackA = commonAncestorPathHelper(root, a); // O(n)
    auto stackB = commonAncestorPathHelper(root, b); // O(n)
    TreeNode *parent = nullptr;

    // O(logn)
    while (!stackA.empty() && !stackB.empty())
    {
        auto topA = stackA.top();
        stackA.pop();
        auto topB = stackB.top();
        stackB.pop();

        if (topA == topB)
        {
            parent = topA;
        }
        else
        {
            break;
        }
    }

    return parent;
}

TreeNode *commonAncestorSearchHelper(TreeNode *root, TreeNode *a, TreeNode *b)
{
    if (root == nullptr || root == a || root == b)
    {
        return root;
    }

    bool leftA = covers(root->left, a);
    bool leftB = covers(root->left, b);

    // One on each side of root
    if (leftA != leftB)
    {
        return root;
    }

    TreeNode *child = (leftA) ? root->left : root->right;
    return commonAncestorSearchHelper(child, a, b);
}
TreeNode *commonAncestorSearch(TreeNode *root, TreeNode *a, TreeNode *b)
{
    if (!covers(root, a) || !covers(root, b))
    {
        return nullptr;
    }

    return commonAncestorSearchHelper(root, a, b);
}


void commonAncestorTest()
{
    int arr[] = {9, 10, 14, 15, 18, 20, 25};
    int size = sizeof(arr) / sizeof(int);
    TreeNode *root = TreeNode::arrayToTree(arr, 0, size - 1);

    TreeNode *a = root->left->right;
    TreeNode *b = root->right->left;

    /* if BST search for ancestor in O(logn)*/
    TreeNode* ancestor = commonAncestorBst(root, a, b);
    cout << "commonAncestorBst: " << ((ancestor == nullptr) ? "Null" : to_string(ancestor->value)) << endl;

    /* if have parent pointer in O(logn) */
    ancestor = commonAncestorParent(root, a, b);
    cout << "commonAncestorParent: " << ((ancestor == nullptr) ? "Null" : to_string(ancestor->value)) << endl;
    ancestor = commonAncestorParent2(root, a, b);
    cout << "commonAncestorParent2: " << ((ancestor == nullptr) ? "Null" : to_string(ancestor->value)) << endl;

    /* if not parent pointer and not BST then two main ways - both O(n)
     * 1) get path to each node and then compare to get point nodes different
     * 2) find node a and then recurse back up check all other sibling subtrees for node b */
    ancestor = commonAncestorPath(root, a, b);
    cout << "commonAncestorPath: " << ((ancestor == nullptr) ? "Null" : to_string(ancestor->value)) << endl;

    /* n + 2n/2 + 2n/4 + ... = n + n + n/2 + n/4 + ... = n * (1/(1-1/2)) = 2n = O(n) */
    ancestor = commonAncestorSearch(root, a, b);
    cout << "commonAncestorSearch: " << ((ancestor == nullptr) ? "Null" : to_string(ancestor->value)) << endl;


    delete root;
}


/* this is a depth first search
 * Time = O(n)
 * Space = O(logn) stack + O(n) levels = O(n)
 * can't really do better since will need to hit every node and store levels */
void BstSequenceMineGetLevels(TreeNode *root, int level, vector<deque<int>> &levels)
{
    if (root == nullptr)
    {
        return;
    }

    // Check if need to add new level
    if (level >= (int)levels.size())
    {
        levels.push_back(deque<int>());
    }

    levels[level].push_back(root->value);
    BstSequenceMineGetLevels(root->left, level + 1, levels);
    BstSequenceMineGetLevels(root->right, level + 1, levels);
}
/* Time: O(b*b!) since getting all permutations of b and then coping over to results */
void BstSequenceMineOrder(vector<vector<int>> &result, vector<int> tmp,
        deque<int> &b)
{
    if (b.empty())
    {
        result.push_back(tmp); // O(b) as coping size of b into result
        return;
    }

    for (uint bIdx = 0; bIdx < b.size(); ++bIdx)
    {
        int bVal = b[bIdx];
        tmp.push_back(bVal);
        b.erase(b.begin() + bIdx); // O(1) as linked list underhood

        BstSequenceMineOrder(result, tmp, b);

        tmp.pop_back();
        b.insert(b.begin() + bIdx, bVal); // O(1) as linked list underhood
    }
}
/* O(n) + O(levels * (levelSize * levelSize!) + (levelSize! * prevLevelSize) + (levelSize!(=#ofperm) * levelSize(=Sizeof1)))
 * n=2^(L+1) - 1
 * levels = L
 * levelSize = 2^L
 * L * (2^L*2^L! + 2^L!*2^L-1! + 2^L!*2^L)
 * = L (2*2^L*2^L! + 2^L!*2^L-1!)
 * = 2L*2^L!(2^L-1 + 2^L-1!)
 * = O(L * 2^L! * 2^L-1!) pretty bad
 * */
vector<vector<int>> BstSequenceMine(TreeNode *root)
{
    // Get levels
    vector<deque<int>> levels;
    BstSequenceMineGetLevels(root, 0, levels); // O(n)

    // merge permutations of current level with previous
    vector<vector<int>> sequences;
    for (uint i = 0; i < levels.size(); ++i)
    {
        vector<vector<int>> sequencesTmp;
        BstSequenceMineOrder(sequencesTmp, vector<int>(), levels[i]); // Get permutations for each level -> O(levelSize * levelSize!)

        // O(levelSize! * prevLevelSize)
        if (!sequences.empty())
        {
            // merge current permutations of level with previous levels
            for (uint j = 0; j < sequencesTmp.size(); ++j)
            {
                sequencesTmp[j].insert(sequencesTmp[j].begin(),
                        sequences[j % sequences.size()].begin(), sequences[j % sequences.size()].end());
            }
        }

        // copy O(levelSize!(=#ofperm) * levelSize(=Sizeof1))
        sequences = sequencesTmp;
    }

    return sequences;
}


/* Weave lists in together in all possible ways
 * This algo works by removing the head from one list, recursing, and then doing the same for the other
 * list */
void weaveLists(list<int> &first, list<int> &second,
        vector<list<int>> &results, list<int> &prefix)
{
    /* One list is empty
     * Add remainder to [a cloned] prefix and store result */
    if (first.empty() || second.empty())
    {
        auto result = prefix;
        result.insert(result.end(), first.begin(), first.end());
        result.insert(result.end(), second.begin(), second.end());
        results.push_back(result);
        return;
    }

    /* binary select either lhs or rhs */

    /* Recurse with head of first added to prefix. Removing the head will damage first, so
     * we'll need to put it back afterwards */
    // Get next from lhs list
    int headFirst = first.front();
    first.pop_front();
    prefix.push_back(headFirst);
    weaveLists(first, second, results, prefix);  // Go down this path
    // revert
    prefix.pop_back();
    first.push_front(headFirst);

    /* Do the same thing with second, damaging and then restoring list */
    // Get next from rhs list
    int headSecond = second.front();
    second.pop_front();
    prefix.push_back(headSecond);
    weaveLists(first, second, results, prefix);  // Go down this path
    // revert
    prefix.pop_back();
    second.push_front(headSecond);
}
vector<list<int>> allSequences(TreeNode *node)
{
    auto result = vector<list<int>>();

    if (nullptr == node)
    {
        result.push_back(list<int>());
        return result;
    }

    list<int> prefix;
    prefix.push_back(node->value);

    /* Recurse on left and right subtrees */
    auto leftSeq = allSequences(node->left);
    auto rightSeq = allSequences(node->right);

    /* Weave together each list from left and right sides */
    for (auto itLeft = leftSeq.begin(); itLeft != leftSeq.end(); ++itLeft)
    {
        for (auto itRight = rightSeq.begin(); itRight != rightSeq.end(); ++itRight)
        {
            weaveLists(*itLeft, *itRight, result, prefix);
        }
    }

    return result;
}


void BstSequenceTest()
{
    // Note Mine is wrong misses ordering of all left before right
    {
    int arr[] = {1,2,3};
    TreeNode *root = TreeNode::arrayToTree(arr, 0, 2);

    // Get possible array's to make binary search tree
    auto arraysMine = BstSequenceMine(root);
    cout << "arraysMine: " << arraysMine.size() << endl;
    /*for (uint i = 0; i < arraysMine.size(); ++i)
    {
        cout << "arraysMine[" << i << "]: ";
        for (auto val : arraysMine[i])
        {
            cout << val << " ";
        }
        cout << endl;
    }*/

    auto arrays = allSequences(root);
    cout << "allSequences: " << arrays.size() << endl;
    /*for (uint i = 0; i < arrays.size(); ++i)
    {
       cout << "allSequences[" << i << "]: ";
       for (auto val : arrays[i])
       {
           cout << val << " ";
       }
       cout << endl;
    }*/

    delete root;
    }

    {
    TreeNode *root = new TreeNode(50);
    root->left = new TreeNode(20);
    root->left->left = new TreeNode(10);
    root->left->right = new TreeNode(25);
    root->left->left->left = new TreeNode(5);
    root->left->left->right = new TreeNode(15);

    root->right = new TreeNode(60);
    root->right->right = new TreeNode(70);
    root->right->right->left = new TreeNode(65);
    root->right->right->right = new TreeNode(80);

    /*     50
     *    /  \
     *   20  60
     *   / \   \
     *  10 25   70
     *  / \    /  \
     * 5  15  65  80 */

    // Get possible array's to make binary search tree
    auto arraysMine = BstSequenceMine(root);
    cout << "arraysMine: " << arraysMine.size() << endl;
    /*for (uint i = 0; i < arraysMine.size(); ++i)
    {
        cout << "arraysMine[" << i << "]: ";
        for (auto val : arraysMine[i])
        {
            cout << val << " ";
        }
        cout << endl;
    }*/

    auto arrays = allSequences(root);
    cout << "allSequences: " << arrays.size() << endl;
    /*for (uint i = 0; i < arrays.size(); ++i)
    {
       cout << "allSequences[" << i << "]: ";
       for (auto val : arrays[i])
       {
           cout << val << " ";
       }
       cout << endl;
    }*/

    delete root;
    }
}

/* This requires tree node data is simple (ie an int or char) */
void checkSubtreePreOrderStringbuild(TreeNode *root, string &output)
{
    if (root == nullptr)
    {
        output += "X,";
        return;
    }

    output += to_string(root->value);
    output += ",";

    checkSubtreePreOrderStringbuild(root->left, output);
    checkSubtreePreOrderStringbuild(root->right, output);
}
/* Time: O(t1 + t2 + t1) = O(t1 + t2)
 * Space: O(t1 + t2) */
bool checkSubtreePreOrder(TreeNode *t1, TreeNode *t2)
{
    string s1;
    string s2;

    checkSubtreePreOrderStringbuild(t1, s1);
    checkSubtreePreOrderStringbuild(t2, s2);

    return s1.find(s2) != string::npos; // linear
}

/* Time: O(t2)
 * Space: O(logT2) */
bool checkSubtreeSearchCompare(TreeNode *t1, TreeNode *t2)
{
    if (t1 == nullptr && t2 == nullptr)
    {
        return true; // nothing left in subtree
    }
    else if (t1 == nullptr || t2 == nullptr)
    {
        return false; // only one of the trees has nothing left, and so they don't match
    }
    else if (t1->value != t2->value)
    {
        return false; // the values don't match
    }

    // AND is same as adding bool checks for if any are false, but takes less code
    return checkSubtreeSearchCompare(t1->left, t2->left) &&
            checkSubtreeSearchCompare(t1->right, t2->right);
}
/* Time: O(T1) * O(T2)
 * Space: O(logT1 - logT2) + O(logT2) */
bool checkSubtreeSearchIs(TreeNode *t1, TreeNode *t2)
{
    if (t1 == nullptr)
    {
        return false; // if subtree in t1 not found hit end return false and keep searching other paths
    }

    if (t1->value == t2->value && checkSubtreeSearchCompare(t1, t2))
    {
        return true;
    }

    // OR is same as add checks for true between calls, but takes less code
    return checkSubtreeSearchIs(t1->left, t2) || checkSubtreeSearchIs(t1->right, t2);
}

/* Time: O(T1) * O(T2)
 * Space: O(logT1 - logT2) + O(logT2) or book says O(logT1 + logT2) */
bool checkSubtreeSearch(TreeNode *t1, TreeNode *t2)
{
    // Empty tree is always a subtree
    if (t2 == nullptr)
    {
        return true;
    }

    return checkSubtreeSearchIs(t1, t2);
}

void checkSubtreeTest()
{
    {
        TreeNode *t1 = new TreeNode(10);
        t1->left = new TreeNode(5);
        t1->left->left = new TreeNode(2);
        t1->left->right = new TreeNode(7);

        t1->right = new TreeNode(8);

        /*     10
         *    /  \
         *   5    8
         *  / \
         *  2 7
         */

        TreeNode *t2 = new TreeNode(5);
        t2->left = new TreeNode(2);
        t2->right = new TreeNode(7);

         /*  5
         *  / \
         *  2 7
         */

        /* Hence,
         * First one has faster (logT1, logT2) runtime but more memory (T1 + T2)
         * Second one has slower (T1*T2) runtime but less memory (logT1 + logT2), though book suggests O(T1 + #occurrence * T2)
         * */
        cout << "checkSubtreePreOrder: " << checkSubtreePreOrder(t1, t2) << endl;
        cout << "checkSubtreeSearch: " << checkSubtreeSearch(t1, t2) << endl;

        delete t1;
        delete t2;
    }
}

void randNodeTest()
{
    int arr[] = {1, 2, 3, 4, 5, 6, 7};
    int size = sizeof(arr) / sizeof(int);
    TreeNode *root = TreeNode::arrayToTree(arr, 0, size - 1);
    cout << "Size: " << root->size << endl;
    TreeNode * found = root->find(6);
    cout << "Found: " << found->value << ", " << found->size << endl;

    for (int i = 0; i < 10; ++i)
    {
        TreeNode *randNode = root->getRandNode();
        cout << "Rand[" << i << "]: " << randNode->value << ", " << randNode->size << endl;
    }

    delete root;
}

/* Time:
 * first passing down total sum from root node = O(N)
 * then for every node below performing a passing down of total sum,
 * hence for every node calculating path sum a path is logN, and there or N nodes => O(NlogN)
 *
 * Could also look at it as: N + N-1 + N-3 + N-7 + ... = sum(N - 2^i) upto logN =
 * N * number of elements - sum(2^i) upto logN = N*logN - (1 + 2 + 4) since 2^i is number of nodes per level for logN levels
 * O(N*logN - N) = O(NlogN)
 * Note: Sum of powers of 2 = 001 + 010 + 100 = 111 = N
 * If not balanced tree = N-1 + N-2 + N-3 + ... + 1 = sum(1 to N) = O(N^2) */
int pathSumBruteForceMine(TreeNode *root, const int sum, int total = 0)
{
    if (root == nullptr)
    {
        return 0;
    }

    /* Add current node and check */
    total += root->value;
    int count = (total == sum) ? 1 : 0;

    /* Continue left and right based on current node count */
    count += pathSumBruteForceMine(root->left, sum, total);
    count += pathSumBruteForceMine(root->right, sum, total);

    /* Restart a check on child nodes */
    count += pathSumBruteForceMine(root->left, sum);
    count += pathSumBruteForceMine(root->right, sum);

    return count;
}

/* Returns the number of paths with this sum starting from this node */
int bruteForceBookCountPathsWithSumFromNode(TreeNode *root, const int targetSum, int curSum)
{
    if (root == nullptr) return 0;

    curSum += root->value;
    int totalPaths = (curSum == targetSum) ? 1 : 0;

    totalPaths += bruteForceBookCountPathsWithSumFromNode(root->left, targetSum, curSum);
    totalPaths += bruteForceBookCountPathsWithSumFromNode(root->right, targetSum, curSum);

    return totalPaths;
}
int bruteForceBookCountPathsWithSum(TreeNode *root, const int targetSum)
{
    if (root == nullptr) return 0;

    /* Count paths with sum starting from root */
    int pathsFromRoot = bruteForceBookCountPathsWithSumFromNode(root, targetSum, 0);

    /* Try the node on the left and right */
    int pathsFromLeft = bruteForceBookCountPathsWithSum(root->left, targetSum);
    int pathsFromRight = bruteForceBookCountPathsWithSum(root->right, targetSum);
    return pathsFromRoot + pathsFromLeft + pathsFromRight;
}

int pathSumHashHelper(TreeNode *root, const int targetSum, int runningSum,
        unordered_map<int, int> &pathCount)
{
    if (root == nullptr)
    {
        return 0;
    }

    // calculate rolling sum and get sum => sumY - T = sumX
    runningSum += root->value;
    int sum = runningSum - targetSum;

    // Count paths with sum ending at current node
    int countSums = 0;
    if (pathCount.find(sum) != pathCount.end())
    {
        countSums += pathCount[sum];
    }

    // If runningSum equals targetSum, then one additional path starts are root
    if (runningSum == targetSum)
    {
        ++countSums;
    }

    // Add current node to hash
    ++pathCount[runningSum];

    // continue and get counts for left and right subtrees
    countSums += pathSumHashHelper(root->left, targetSum, runningSum, pathCount);
    countSums += pathSumHashHelper(root->right, targetSum, runningSum, pathCount);

    // Revert for other branches
    --pathCount[runningSum];
    if (pathCount[runningSum] <= 0)
    {
        pathCount.erase(runningSum);
    }

    return countSums;
}
/* Time: O(n) since processing each node once
 * Space: O(logn) for hashtable */
int pathSumHash(TreeNode *root, const int targetSum)
{
    if (root == nullptr)
    {
        return 0;
    }

    unordered_map<int, int> hash;
    return pathSumHashHelper(root, targetSum, 0, hash);
}

void pathSumTest()
{
    int arr[] = {1, 1, 1, 1, 1, 1, 1};
    int size = sizeof(arr) / sizeof(int);
    TreeNode *root = TreeNode::arrayToTree(arr, 0, size - 1);
    cout << "1) Sum Count: " << pathSumBruteForceMine(root, 2) << endl;
    cout << "2) Sum Count: " << bruteForceBookCountPathsWithSum(root, 2) << endl;
    cout << "3) Sum Count: " << pathSumHash(root, 2) << endl;

    delete root;
}

void Chap4TreesGraphsMain()
{
    ConnectFriendsTest();
    RouteBetweeenNodesTest();
    MinimalTreeTest(); //+create linked list, isBalanced
    isBstTest();
    getNextInorderTest();
    buildOrderTest();
    commonAncestorTest();
    BstSequenceTest();
    checkSubtreeTest();
    randNodeTest();
    pathSumTest();
}
