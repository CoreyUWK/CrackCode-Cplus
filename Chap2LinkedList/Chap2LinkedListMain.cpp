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


class LinkedListNode
{
public:
	int value;
	LinkedListNode *next;
	bool isDelete;

	LinkedListNode(int value, LinkedListNode *next = nullptr) : value(value), next(next), isDelete(false) {}

	~LinkedListNode()
	{
		if (next != nullptr && !next->isDelete)
		{
		    LinkedListNode *nextLocal = this->next;
			this->next = nullptr;
			this->isDelete = true;
			delete nextLocal;
		}

		/*if (head != nullptr && head->alive)
		{
			LinkedListNode *next = head->next;
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
		LinkedListNode *ptr = this;
		while (ptr != nullptr)
		{
			++count;
			ptr = ptr->next;
		}

		return count;
	}

	void append(int value)
	{
	    LinkedListNode *head = this;
		while (head->next != nullptr)
		{
			head = head->next;
		}
		head->next = new LinkedListNode(value);
	}

	void print()
	{
	    LinkedListNode *head = this;
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
void interLeave(LinkedListNode *head)
{
	if (head == nullptr || head->next == nullptr || head->next->next == nullptr)
	{
		return;
	}

	LinkedListNode *slow = head;
	LinkedListNode *fast = head->next;

	// At end slow = 1 before mid
	//        fast = last
	while (fast->next != nullptr)
	{
		slow = slow->next;
		fast = fast->next->next;
	}

	// Now interleave
	while (head > slow) // Memory going in opposite direction
	{
	    LinkedListNode *headNext = head->next;
		LinkedListNode *slowNext = slow->next->next;

		head->next = slow->next;
		slow->next->next = headNext;
		head = headNext;
		slow->next = slowNext;
	}
	// Handle end where all have been interleaved except last => An->Bn->0
	head->next = slow->next;
}


void interLeave2(LinkedListNode *head)
{
	if (head == nullptr || head->next == nullptr || head->next->next == nullptr)
	{
		return;
	}

	LinkedListNode *slow = head;
	LinkedListNode *fast = head->next;

	// At end slow = 1 before mid
	//        fast = last
	while (fast->next != nullptr)
	{
		slow = slow->next;
		fast = fast->next->next;
	}

	LinkedListNode *list1 = head;
	LinkedListNode *list2 = slow->next;
	slow->next = nullptr; // Split Lists

	// Now interleave
	while (list1 != nullptr)
	{
	    LinkedListNode *list1Next = list1->next;
	    LinkedListNode *list2Next = list2->next;

		list1->next = list2;
		list2->next = list1Next;

		list1 = list1Next;
		list2 = list2Next;
	}
}

//--------------------------------------------------------------
/* Write code to remove duplicates from an unsorted linked list.
FOLLOW UP
How would you solve this problem if a temporary buffer is not allowed? */
/* 1) With hash => Space O(n), Time O(n)
 * 2) brute force double loop cur, then check runner.data==cur.data => Space O(1), Time O(n^2) */
void removeDup(LinkedListNode *head)
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
		    LinkedListNode *tmp = head->next;
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
/* Implement an algorithm to find the kth to last element of a singly linked list */
LinkedListNode * kToLast_Recursive(LinkedListNode *head, int k, int &i)
{
	if (head == nullptr)
	{
		i = 0;
		return nullptr;
	}

	LinkedListNode *n = kToLast_Recursive(head->next, k, i);
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
LinkedListNode * kToLast_Recursive(LinkedListNode *head, int k)
{
	int i = 0;
	return kToLast_Recursive(head, k, i);
}

LinkedListNode * kToLast_Recursive2(LinkedListNode *head, int k)
{
	static int i = 0;

	if (head == nullptr)
	{
		i = 0;
		return nullptr;
	}

	LinkedListNode *n = kToLast_Recursive2(head, k);
	++i;
	if (i == k)
	{
		return head;
	}
	return n;
}


LinkedListNode * kToLast_Iterative(LinkedListNode *head, int k)
{
    LinkedListNode *follower = head;
    LinkedListNode *current = head;

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
/* Implement an algorithm to delete a node in the middle (i.e., any node but
the first and last node, not necessarily the exact middle) of a singly linked list, given only access to
that node.
EXAMPLE
Input: the node c from the linked list a - >b- >c - >d - >e- >f
Result: nothing is returned, but the new linked list looks like a - >b- >d - >e- >f */
bool deleteMidNode(LinkedListNode *n)
{
	// Check not empty or last
	if (n == nullptr || n->next == nullptr)
	{
		return false;
	}

	// Copy next nodes value to current and delete next
	LinkedListNode *d = n->next;
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
LinkedListNode * partition(LinkedListNode *n, int x)
{
	if (n == nullptr)
	{
		return nullptr;
	}

	LinkedListNode *lessListHead = nullptr, *lessListTail = nullptr;
	LinkedListNode *greaterListHead = nullptr, *greaterListTail = nullptr;

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

LinkedListNode* partition2(LinkedListNode *head, int key)
{
	if (head == nullptr)
	{
		return head;
	}

	LinkedListNode *cur = head;

	while (cur->next != nullptr)
	{
		if (cur->next->value < key)
		{
		    LinkedListNode *newHead = cur->next;
			// move cur to front of list
			cur->next = cur->next->next;  // remove prev link
			newHead->next = head;  // remove next link
			head = newHead;  // Update head
		}
		else
		{
			cur = cur->next;
		}
	}

	return head;
}


//--------------------------------------------------------------
/* You have two numbers represented by a linked list, where each node contains a single
digit. The digits are stored in reverse order, such that the 1 's digit is at the head of the list. Write a
function that adds the two numbers and returns the sum as a linked list.
EXAMPLE
Input: (7-) 1 -) 6) + (5 -) 9 -) 2) .That is,617 + 295.
Output: 2 -) 1 -) 9. That is, 912.
FOLLOW UP
Suppose the digits are stored in forward order. Repeat the above problem.
EXAMPLE
Input: (6 -) 1 -) 7) + (2 -) 9 -) 5).That is,617 + 295.
Output: 9 -) 1 -) 2. That is, 912. */

LinkedListNode * sumLists_Recursive(LinkedListNode *list1, LinkedListNode *list2, int carry)
{
	if (list1 == nullptr && list2 == nullptr && carry == 0)
	{
		return nullptr;
	}

	int value = carry;
	LinkedListNode *list1Next = nullptr;
	LinkedListNode *list2Next = nullptr;

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

	LinkedListNode *sum = new LinkedListNode(value % 10);
	sum->next = sumLists_Recursive(list1Next, list2Next, value / 10);
	return sum;
}


/* Input: (7->1->6) + (5->9->2) = 617 + 295
 * Output: 2->1->9 = 912
 *
 * If list was reversed Get length of lists, then add zeros to front of smaller
 * then recurse down and add */
LinkedListNode * sumLists_Recursive(LinkedListNode *list1, LinkedListNode *list2)
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


LinkedListNode * sumLists_Iterative(LinkedListNode *list1, LinkedListNode *list2)
{
	LinkedListNode *sumHead = nullptr;
	LinkedListNode *sum = nullptr;
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
			sum = new LinkedListNode(value % 10);
			sumHead = sum;
		}
		else
		{
			sum->next = new LinkedListNode(value % 10);
			sum = sum->next;
		}

		carry = value / 10;
	}

	return sumHead;
}


LinkedListNode * sumLists_Reverse(LinkedListNode *list1, LinkedListNode *list2, int &carry)
{
	if (list1 == nullptr || list2 == nullptr)
	{
		carry = 0;
		return nullptr;
	}

	LinkedListNode *sumNext = sumLists_Reverse(list1->next, list2->next, carry);

	int value = carry + list1->value + list2->value;
	carry = value / 10;

	LinkedListNode *newSum = new LinkedListNode(value % 10, sumNext);
	return newSum;
}


LinkedListNode * sumLists_Reverse_Pad(LinkedListNode *list, int x)
{
	LinkedListNode *head = list;
	for (int i = 0; i < x; ++i)
	{
		head = new LinkedListNode(0, head);
	}
	return head;
}


LinkedListNode * sumLists_Reverse(LinkedListNode *list1, LinkedListNode *list2)
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
	LinkedListNode *sum = sumLists_Reverse(list1, list2, carry);
	if (carry > 0)
	{
		sum = new LinkedListNode(carry, sum);
	}

	return sum;
}


//--------------------------------------------------------------
/* 1) can count with hash: time O(n), space O(n)
2) slow and fast and check: time O(n), space O(1)
3) slow and fast, push slow onto stack then compare with rest: Time O(n), space O(n/2 = n)
4) reverse list and compare: time O(n), space O(n) */
/* Implement a function to check if a linked list is a palindrome */
/* calls: 1 + 2 + 3 + 4 = 10
 * 1 + 2 + ... + (n-1) + n = n(n+1) / 2 = 10 => O(n^2) */
LinkedListNode * isPalidrome_ReverseCmp_Reverse_Recursive(LinkedListNode *list)
{
	if (list->next == nullptr)
	{
		return new LinkedListNode(list->value);
	}

	LinkedListNode *parent = isPalidrome_ReverseCmp_Reverse_Recursive(list->next);
	LinkedListNode *tmpP = parent;
	while (tmpP->next != nullptr)
	{
		tmpP = tmpP->next;
	}
	tmpP->next = new LinkedListNode(list->value);
	return parent;
}

LinkedListNode * isPalidrome_ReverseCmp_Reverse(LinkedListNode *list)
{
	LinkedListNode *head = nullptr;
	while (list != nullptr)
	{
		head = new LinkedListNode(list->value, head);
		list = list->next;
	}
	return head;
}


bool isPalidrome_ReverseCmp(LinkedListNode *list)
{
	LinkedListNode *reverse = isPalidrome_ReverseCmp_Reverse(list);

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


bool isPalidrome_Stack(LinkedListNode *list)
{
	stack<decltype(list->value)> st;
	LinkedListNode *slow = list;
	LinkedListNode *fast = list;

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


LinkedListNode * isPalidrome_Recursive(LinkedListNode *list, int length)
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

	LinkedListNode *outer = isPalidrome_Recursive(list->next, length - 2);
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

bool isPalidrome_Recursive(LinkedListNode *list)
{
	int length = list->getLength();

	if (length < 3)
	{
		return true;
	}

	return (nullptr == isPalidrome_Recursive(list, length)) ? false: true;
}

//--------------------------------------------------------------
/* Given two (singly) linked lists, determine if the two lists intersect. Return the intersecting
node. Note that the intersection is defined based on reference, not value. That is, if the kth
node of the first linked list is the exact same node (by reference) as the jth node of the second
linked list, then they are intersecting. */

/* 1) Get lengths of both lists, then match starting point for both lists and iterate till end
 * or both are same/intersect
 * Can check if lists intersect if tail node is the same for both
 * 2) Hash table check
 *
 *   a-b-c-d
 * h-e-f-c-d
Hence, end should be same at intersect
1) concatenate lists and see if loop exists
O(a+b), O(a+b)
2) add nodes to two stacks and then keep poping until not same
O(a+b), O(a+b)
3) hash table of pointer values
4) just chop off extra at front
-get lengths, take min and chop off front of other
-then find intersection
O(a+b) */
LinkedListNode * getIntersetion(LinkedListNode *list1, LinkedListNode *list2)
{
	if (list1 == nullptr || list2 == nullptr)
	{
		return nullptr;
	}

	// Get tails
	LinkedListNode *tail1 = list1;
	while (tail1->next != nullptr)
	{
		tail1 = tail1->next;
	}

	LinkedListNode *tail2 = list2;
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

/* Given a circular linked list, implement an algorithm that returns the node at the beginning of the loop.
 * DEFINITION
 * Circular linked list: A (corrupt) linked list in which a node's next pointer points to an earlier node, so
 * as to make a loop in the linked list.
 * EXAMPLE
 * Input: A -) B -) C -) 0 -) E - ) C[thesameCasearlierl
 * Output: C
 *
 * 1) Find if loop exists with slow/fast pointer, then get start of loop by incrementing from collision
 * and head and checking if head pointer ever in loop
 * That is looping through cycle part and checking if node is in it
 * O(cycle*noncycle)
 * 2) Find if loop exist same a 1) but to get start just increment from head and collision till equal
 * 3) hash table */
LinkedListNode * loopDetect(LinkedListNode *list)
{
	if (list == nullptr || list->next == nullptr)
	{
		return nullptr;
	}

	LinkedListNode *slow = list;
	LinkedListNode *fast = list;

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
	LinkedListNode *head = new LinkedListNode(1);
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

void interleave2Test()
{
	// Interleave
	LinkedListNode *head = new LinkedListNode(1);
	head->append(1);
	head->append(1);
	head->append(1);
	head->append(2);
	head->append(2);
	head->append(2);
	head->append(2);

	cout << "Interleave2" << endl;
	interLeave2(head);
	head->print();
	delete head;
}

void removeDuplicateTest()
{
	LinkedListNode *head = new LinkedListNode(1);
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
	LinkedListNode *head = new LinkedListNode(1);
	head->append(2);
	head->append(3);
	head->append(4);
	head->append(5);
	head->append(6);
	head->append(7);
	head->append(8);
	cout << "kToLast" << endl;
	LinkedListNode *kth = kToLast_Recursive(head, 2);
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
	LinkedListNode *head = new LinkedListNode(1);
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
	LinkedListNode *head = new LinkedListNode(1);
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

	head = new LinkedListNode(3);
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

void partition2Test()
{
	LinkedListNode *head = new LinkedListNode(1);
	head->append(2);
	head->append(3);
	head->append(4);
	head->append(5);
	head->append(6);
	head->append(7);
	head->append(8);

	// Partition
	cout << "Partition2" << endl;
	head = partition2(head, 4);
	head->print();
	delete head;
	head = nullptr;

	head = new LinkedListNode(3);
	head->append(5);
	head->append(8);
	head->append(5);
	head->append(10);
	head->append(2);
	head->append(1);
	head->append(6);
	head = partition2(head, 5);
	head->print();
	delete head;
	head = nullptr;
}

void sumListsTest()
{
	// Sum Lists
	cout << "Sum List" << endl;
	LinkedListNode *list1 = new LinkedListNode(7);
	list1->append(1);
	list1->append(6);
	list1->append(3);

	LinkedListNode *list2 = new LinkedListNode(5);
	list2->append(9);
	list2->append(2);

	LinkedListNode *sumList = sumLists_Recursive(list1, list2);
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
	LinkedListNode *list = new LinkedListNode(0);
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

	LinkedListNode *list1 = new LinkedListNode(7);
	list1->append(1);
	list1->append(6);
	list1->append(3);

	LinkedListNode *list2 = new LinkedListNode(5);
	list2->append(9);
	list2->append(2);

	// Intersect
	LinkedListNode *tail1 = list1;
	while(tail1->next != nullptr)
	{
		tail1 = tail1->next;
	}
	LinkedListNode *tail2 = list2;
	while(tail2->next != nullptr)
	{
		tail2 = tail2->next;
	}

	tail2->next = tail1;

	LinkedListNode *intersect = getIntersetion(list1, list2);
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

	LinkedListNode *head = new LinkedListNode(1);
	head->append(2);
	head->append(3);
	head->append(4);
	head->append(5);
	head->append(6);
	head->append(7);
	head->append(8);

	// Create loop
	LinkedListNode *tail = head;
	while (tail->next != nullptr)
	{
		tail = tail->next;
	}
	tail->next = head->next->next->next->next; // 8->5

	LinkedListNode *loopStart = loopDetect(head);
	assert(5 == loopStart->value);
	cout << loopStart->value << endl;
	delete head;
}

void Chap2LinkedListMain()
{
	interleaveTest();
	interleave2Test();
	removeDuplicateTest();
	kToLastTest();
	deleteMiddleTest();
	partitionTest();
	partition2Test();
	sumListsTest();
	palidromeTest();
	intersectTest();
	loopDetectTest();
	cout << "done" << endl;
}

