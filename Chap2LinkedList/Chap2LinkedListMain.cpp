/*
 * Chap2LinkedListMain.cpp
 *
 *  Created on: 2019-09-24
 *      Author: ckirsch
 */
#include "Chap2LinkedListMain.h"
#include <iostream>
#include <cassert>
#include <unordered_set>
#include <stack>
using namespace std;


class Node
{
public:
	int value;
	Node *next;

	Node(int value, Node *next = nullptr) : value(value), next(next) {}

	~Node()
	{
		if (next != nullptr)
		{
			Node *nextLocal = this->next;
			this->next = nullptr;
			delete nextLocal;
		}

		/*if (head != nullptr && head->alive)
		{
			Node *next = head->next;
			head->next = nullptr;
			head->alive = false;
			delete head;

			if (next != nullptr)
			{
				delete next;
			}
		}*/
	}

	int getLength()
	{
		int count = 0;
		Node *ptr = this;
		while (ptr != nullptr)
		{
			++count;
			ptr = ptr->next;
		}

		return count;
	}

	void append(int value)
	{
		Node *head = this;
		while (head->next != nullptr)
		{
			head = head->next;
		}
		head->next = new Node(value);
	}

	void print()
	{
		Node *head = this;
		while (head != nullptr)
		{
			cout << head->value << " ";
			head = head->next;
		}
		cout << endl;
	}
};


//--------------------------------------------------------------

/* Given linked list a1->a2->a3->b1->b2->b3 => a1->b1->a2->b2->a3->b3 */
void interLeave(Node *head)
{
	if (head == nullptr || head->next == nullptr || head->next->next == nullptr)
	{
		return;
	}

	Node *slow = head;
	Node *fast = head->next;

	// At end slow = 1 before mid
	//        fast = last
	while (fast->next != nullptr)
	{
		slow = slow->next;
		fast = fast->next->next;
	}

	// Now interleave
	while (head < slow)
	{
		Node *headNext = head->next;
		Node *slowNext = slow->next->next;

		head->next = slow->next;
		slow->next->next = headNext;
		head = headNext;
		slow->next = slowNext;
	}
	// Handle end where all have been interleaved except last => An->Bn->0
	head->next = slow->next;
}

//--------------------------------------------------------------

/* 1) With hash => Space O(n), Time O(n)
 * 2) brute force double loop cur, then check runner.data==cur.data => Space O(1), Time O(n^2) */
void removeDup(Node *head)
{
	if (nullptr == head)
	{
		return;
	}

	unordered_set<decltype(head->value)> hash;
	hash.insert(head->value);
	while (head->next != nullptr)
	{
		if (hash.find(head->next->value) != hash.end())
		{
			Node *tmp = head->next;
			head->next = head->next->next;

			tmp->next = nullptr;
			delete tmp;
		}
		else
		{
			hash.insert(head->next->value);
			head = head->next;
		}
	}
}

//--------------------------------------------------------------
Node * kToLast_Recursive(Node *head, int k, int &i)
{
	if (head == nullptr)
	{
		i = 0;
		return nullptr;
	}

	Node *n = kToLast_Recursive(head->next, k, i);
	++i;
	if (i == k)
	{
		return head;
	}
	return n;
}


/* 1) get size of linked list O(n)
 * then using K=1 is the nth item, N-K=# of jumps from head
 * O(n-K) to perform jumps
 * 2) Move pointer to head + K, then iterate [head, head + K] pointers till point to end(null),
 * then head will be pointing to Kth item O(n)
 * 3) Recursively iterate till hit end(null), then count back up. When hit K pass node back.
 * O(n) time and space */
Node * kToLast_Recursive(Node *head, int k)
{
	int i = 0;
	return kToLast_Recursive(head, k, i);
}

Node * kToLast_Iterative(Node *head, int k)
{
	Node *follower = head;
	Node *current = head;

	if (head == nullptr || k == 0)
	{
		return nullptr;
	}

	// Move current by k
	for (int i = 0; i < k; ++i)
	{
		// Check not out-of-bounds
		if (current == nullptr)
		{
			return nullptr;
		}

		current = current->next;
	}

	// Move both at same pace till follower = k and current = null
	while (current != nullptr)
	{
		current = current->next;
		follower = follower->next;
	}

	return follower;
}


//--------------------------------------------------------------

bool deleteMidNode(Node *n)
{
	// Check not empty or last
	if (n == nullptr || n->next == nullptr)
	{
		return false;
	}

	// Copy next nodes value to current and delete next
	Node *d = n->next;
	n->value = d->value;
	n->next = d->next;
	d->next = nullptr;
	delete d;

	return true;
}

//--------------------------------------------------------------

/* Partition all nodes < x come before those greater and equal, order does not matter.
 * x does not need to be in middle
 *
 * ex. 3 5 8 5 10 2 1 [x=5]
 * out: 3 1 2 10 5 5 8
 *
 * 1) Make two lists of less and greater/equal, then merege the two O(n)
 * 2) If you hit a less then push to head, if you hit greater/equal push tail O(n) */
Node * partition(Node *n, int x)
{
	if (n == nullptr)
	{
		return nullptr;
	}

	Node *lessListHead = nullptr, *lessListTail = nullptr;
	Node *greaterListHead = nullptr, *greaterListTail = nullptr;

	while (n != nullptr)
	{
		if (n->value < x)
		{
			if (lessListHead == nullptr)
			{
				lessListHead = n;
				n = n->next;
				lessListHead->next = nullptr;
				lessListTail = lessListHead;
			}
			else
			{
				lessListTail->next = n;
				n = n->next;
				lessListTail = lessListTail->next;
				lessListTail->next = nullptr;
			}
		}
		else
		{
			if (greaterListHead == nullptr)
			{
				greaterListHead = n;
				n = n->next;
				greaterListHead->next = nullptr;
				greaterListTail = greaterListHead;
			}
			else
			{
				greaterListTail->next = n;
				n = n->next;
				greaterListTail = greaterListTail->next;
				greaterListTail->next = nullptr;
			}
		}
	}

	// Merge the two lists
	lessListTail->next = greaterListHead;
	return lessListHead;
}

//--------------------------------------------------------------
Node * sumLists_Recursive(Node *list1, Node *list2, int carry)
{
	if (list1 == nullptr && list2 == nullptr && carry == 0)
	{
		return nullptr;
	}

	int value = carry;

	Node *list1Next = nullptr;
	Node *list2Next = nullptr;

	if (list1 != nullptr)
	{
		value += list1->value;
		list1Next = list1->next;
	}

	if (list2 != nullptr)
	{
		value += list2->value;
		list2Next = list2->next;
	}

	if (value > 9)
	{
		carry = value / 10;
		value %= 10;
	}
	else
	{
		carry = 0;
	}

	Node *sum = new Node(value);
	sum->next = sumLists_Recursive(list1Next, list2Next, carry);
	return sum;
}


/* Input: (7->1->6) + (5->9->2) = 617 + 295
 * Output: 2->1->9 = 912
 *
 * If list was reversed Get length of lists, then add zeros to front of smaller
 * then recurse down and add */
Node * sumLists_Recursive(Node *list1, Node *list2)
{
	if (list1 == nullptr && list2 == nullptr)
	{
		return nullptr;
	}
	else if (list1 == nullptr)
	{
		return list2;
	}
	else if (list2 == nullptr)
	{
		return list1;
	}

	return sumLists_Recursive(list1, list2, 0);
}

Node * sumLists_Iterative(Node *list1, Node *list2)
{
	Node *sumHead = nullptr;
	Node *sum = nullptr;
	int carry = 0;

	while (!(list1 == nullptr && list2 == nullptr && carry == 0))
	{
		int value = carry;
		if (list1 != nullptr)
		{
			value += list1->value;
			list1 = list1->next;
		}
		if (list2 != nullptr)
		{
			value += list2->value;
			list2 = list2->next;
		}

		if (sumHead == nullptr)
		{
			sum = new Node(value % 10);
			sumHead = sum;
		}
		else
		{
			sum->next = new Node(value % 10);
			sum = sum->next;
		}

		carry = value / 10;
	}

	return sumHead;
}


Node * sumLists_Reverse(Node *list1, Node *list2, int &carry)
{
	if (list1 == nullptr || list2 == nullptr)
	{
		carry = 0;
		return nullptr;
	}

	Node *sum = sumLists_Reverse(list1->next, list2->next, carry);

	int value = carry + list1->value + list2->value;
	carry = value / 10;
	value = value % 10;

	Node *newSum = new Node(value);
	newSum->next = sum;

	return newSum;
}


Node * sumLists_Reverse_Pad(Node *list, int x)
{
	Node *head = list;
	for (int i = 0; i < x; ++i)
	{
		head = new Node(0, head);
	}
	return head;
}


Node * sumLists_Reverse(Node *list1, Node *list2)
{
	if (list1 == nullptr && list2 == nullptr)
	{
		return nullptr;
	}
	else if (list1 == nullptr)
	{
		return list2;
	}
	else if (list2 == nullptr)
	{
		return list1;
	}

	// Pad with zeros to match lengths
	int list1Length = list1->getLength();
	int list2Length = list2->getLength();

	if (list1Length > list2Length)
	{
		list2 = sumLists_Reverse_Pad(list2, list1Length - list2Length);
	}
	else if (list2Length > list1Length)
	{
		list1 = sumLists_Reverse_Pad(list1, list2Length - list1Length);
	}

	int carry = 0;
	Node *sum = sumLists_Reverse(list1, list2, carry);
	if (carry > 0)
	{
		Node *newSum = new Node(carry, sum);
		sum = newSum;
	}

	return sum;
}


//--------------------------------------------------------------

Node * isPalidrome_ReverseCmp_Reverse(Node *list)
{
	Node *head = nullptr;
	while (list != nullptr)
	{
		Node *clone = new Node(list->value);
		clone->next = head;
		head = clone;
		list = list->next;
	}
	return head;
}

bool isPalidrome_ReverseCmp(Node *list)
{
	Node *reverse = isPalidrome_ReverseCmp_Reverse(list);

	// Compare original and reverse
	while (list != nullptr)
	{
		if (list->value != reverse->value)
		{
			return false;
		}
		list = list->next;
		reverse = reverse->next;
	}

	return true;
}


bool isPalidrome_Stack(Node *list)
{
	stack<decltype(list->value)> st;
	Node *slow = list;
	Node *fast = list;

	while (fast != nullptr && fast->next != nullptr)
	{
		fast = fast->next->next;
		st.push(slow->value);
		slow = slow->next;
	}

	if (fast != nullptr)
	{
		slow = slow->next;
	}

	while (slow != nullptr)
	{
		auto value = st.top();
		st.pop();

		if (slow->value != value)
		{
			return false;
		}
		slow = slow->next;
	}

	return true;
}


Node * isPalidrome_Recursive(Node *list, int length)
{
	if (length < 2)
	{
		// if odd
		if (length == 1)
		{
			// skip middle
			return list->next;
		}
		// else just go next
		return list;
	}

	Node *outer = isPalidrome_Recursive(list->next, length - 2);
	if (outer == nullptr ||
			outer->value != list->value)
	{
		return nullptr;
	}

	// Return outer when at end of list to make sure pass check for not null occurs
	if (outer->next == nullptr)
	{
		return outer;
	}
	return outer->next;
}

bool isPalidrome_Recursive(Node *list)
{
	int length = list->getLength();

	if (length < 3)
	{
		return true;
	}

	return (nullptr == isPalidrome_Recursive(list, length)) ? false: true;
}

//--------------------------------------------------------------

/* 1) *Get lengths of both lists, then match starting point for both lists and iterate till end
 * or both are same/intersect
 * Can check if lists intersect if tail node is the same for both
 * 2) Hash table check
 * 3) Get lengths of both lists and then match starting point, then recurse to end and work up to get
 * intersecting node */
Node * getIntersetion(Node *list1, Node *list2)
{
	if (list1 == nullptr || list2 == nullptr)
	{
		return nullptr;
	}

	// Get tails
	Node *tail1 = list1;
	while (tail1->next != nullptr)
	{
		tail1 = tail1->next;
	}

	Node *tail2 = list2;
	while (tail2->next != nullptr)
	{
		tail2 = tail2->next;
	}

	// Check if intersection occurred
	if (tail1 != tail2)
	{
		return nullptr;
	}

	int length1 = list1->getLength();
	int length2 = list2->getLength();

	// Move head pointer to match same lengths
	if (length1 > length2)
	{
		int diff = length1 - length2;
		for (int i = 0; i < diff; ++i)
		{
			list1 = list1->next;
		}
	}
	else if (length2 > length1)
	{
		int diff = length2 - length1;
		for (int i = 0; i < diff; ++i)
		{
			list2 = list2->next;
		}
	}

	// Move both pointers till collide
	while (list1 != list2)
	{
		list1 = list1->next;
		list2 = list2->next;
	}

	return list1; // can return either
}

//--------------------------------------------------------------

/* 1) Find if loop exists with slow/fast pointer, then get start of loop by incrementing from collision
 * and head and checking if head pointer ever in loop
 * That is looping through cycle part and checking if node is in it
 * O(cycle*noncycle)
 * 2) Find if loop exist same a 1) but to get start just increament from head and collision till equal  */
Node * loopDetect(Node *list)
{
	if (list == nullptr || list->next == nullptr)
	{
		return nullptr;
	}

	Node *slow = list;
	Node *fast = list;

	// Check if loop exists
	while (fast != nullptr && fast->next != nullptr)
	{
		fast = fast->next->next;
		slow = slow->next;

		// Check for collision
		if (fast == slow)
		{
			break;
		}
	}

	// No loop exists
	if (fast == nullptr || fast->next == nullptr)
	{
		return nullptr;
	}

	/* Move slow to head and keep fast at collision point.
	 * Each are k steps from the loop start.  If they move at same pace they must meet a loop start */
	slow = list;
	while (slow != fast)
	{
		slow = slow->next;
		fast = fast->next;
	}

	return slow;
}

//--------------------------------------------------------------

void interleaveTest()
{
	// Interleave
	Node *head = new Node(1);
	head->append(1);
	head->append(1);
	head->append(1);
	head->append(2);
	head->append(2);
	head->append(2);
	head->append(2);

	cout << "Interleave" << endl;
	interLeave(head);
	head->print();
	delete head;
}

void removeDuplicateTest()
{
	Node *head = new Node(1);
	head->append(1);
	head->append(1);
	head->append(1);
	head->append(2);
	head->append(2);
	head->append(2);
	head->append(2);

	// Remove Duplicate
	cout << "Remove Duplicate" << endl;
	removeDup(head);
	head->print();
	delete head;
	head = nullptr;
}

void kToLastTest()
{
	// kToLast
	Node *head = new Node(1);
	head->append(2);
	head->append(3);
	head->append(4);
	head->append(5);
	head->append(6);
	head->append(7);
	head->append(8);
	cout << "kToLast" << endl;
	Node *kth = kToLast_Recursive(head, 2);
	if (kth != nullptr)
	{
		cout << kth->value << endl;
	}
	else
	{
		cout << "NULL" << endl;
	}

	kth = kToLast_Iterative(head, 2);
	if (kth != nullptr)
	{
		cout << kth->value << endl;
	}
	else
	{
		cout << "NULL" << endl;
	}

	delete head;
}

void deleteMiddleTest()
{
	Node *head = new Node(1);
	head->append(2);
	head->append(3);
	head->append(4);
	head->append(5);
	head->append(6);
	head->append(7);
	head->append(8);

	// Delete Middle
	cout << "Delete Middle" << endl;
	assert(deleteMidNode(head->next->next->next));
	head->print();
	delete head;
}

void partitionTest()
{
	Node *head = new Node(1);
	head->append(2);
	head->append(3);
	head->append(4);
	head->append(5);
	head->append(6);
	head->append(7);
	head->append(8);

	// Partition
	cout << "Partition" << endl;
	head = partition(head, 4);
	head->print();
	delete head;
	head = nullptr;

	head = new Node(3);
	head->append(5);
	head->append(8);
	head->append(5);
	head->append(10);
	head->append(2);
	head->append(1);
	head->append(6);
	head = partition(head, 5);
	head->print();
	delete head;
	head = nullptr;
}

void sumListsTest()
{
	// Sum Lists
	cout << "Sum List" << endl;
	Node *list1 = new Node(7);
	list1->append(1);
	list1->append(6);
	list1->append(3);

	Node *list2 = new Node(5);
	list2->append(9);
	list2->append(2);

	Node *sumList = sumLists_Recursive(list1, list2);
	sumList->print();
	delete sumList;

	sumList = sumLists_Iterative(list1, list2);
	sumList->print();
	delete sumList;

	sumList = sumLists_Reverse(list1, list2);
	sumList->print();
	delete sumList;

	delete list1;
	delete list2;
}

void palidromeTest()
{
	// Palindrome
	Node *list = new Node(0);
	list->append(1);
	list->append(2);
	list->append(1);
	list->append(0);

	cout << "Palidrome" << endl;
	assert(true == isPalidrome_ReverseCmp(list));
	assert(true == isPalidrome_Stack(list));
	assert(true == isPalidrome_Recursive(list));
	list->append(5);
	assert(false == isPalidrome_ReverseCmp(list));
	assert(false == isPalidrome_Stack(list));
	assert(false == isPalidrome_Recursive(list));
}

void intersectTest()
{
	cout << "Intersect Test" << endl;

	Node *list1 = new Node(7);
	list1->append(1);
	list1->append(6);
	list1->append(3);

	Node *list2 = new Node(5);
	list2->append(9);
	list2->append(2);

	// Intersect
	Node *tail1 = list1;
	while(tail1->next != nullptr)
	{
		tail1 = tail1->next;
	}
	Node *tail2 = list2;
	while(tail2->next != nullptr)
	{
		tail2 = tail2->next;
	}

	tail2->next = tail1;

	Node *intersect = getIntersetion(list1, list2);
	if (intersect != nullptr)
	{
		cout << intersect->value << endl;
	}
	else
	{
		cout << "null" << endl;
	}

	delete list1;
	tail2->next = nullptr;
	delete list2;
}

void loopDetectTest()
{
	cout << "loopDetect" << endl;

	Node *head = new Node(1);
	head->append(2);
	head->append(3);
	head->append(4);
	head->append(5);
	head->append(6);
	head->append(7);
	head->append(8);

	// Create loop
	Node *tail = head;
	while (tail->next != nullptr)
	{
		tail = tail->next;
	}
	tail->next = head->next->next->next->next; // 8->5

	Node *loopStart = loopDetect(head);
	assert(5 == loopStart->value);
	cout << loopStart->value << endl;
	delete head;
}

void Chap2LinkedListMain()
{
	interleaveTest();
	removeDuplicateTest();
	kToLastTest();
	deleteMiddleTest();
	partitionTest();
	sumListsTest();
	palidromeTest();
	intersectTest();
	loopDetectTest();
	cout << "done" << endl;
}

