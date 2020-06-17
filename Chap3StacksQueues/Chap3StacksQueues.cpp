/*
 * Chap3StacksQueues.cpp
 *
 *  Created on: 2019-10-02
 *      Author: ckirsch
 */
#include "Chap3StacksQueues.h"
#include <iostream>
#include <cassert>
#include <stack>
#include <vector>
#include <queue>
#include <memory>
#include <unordered_map>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <forward_list>
#include <chrono>
#include <list>
#include <ctime>
#include <thread>
using namespace std;

//-------------------------------------------------------------------------------------
template <typename T>
class StackLinkedList
{
	class StackNode
	{
		T data;
		StackNode *next;

		StackNode(T data, StackNode *next = nullptr) : data(data), next(next) {}

		friend StackLinkedList;
	};

	StackNode *top;

public:
	StackLinkedList() : top(nullptr) {}

	~StackLinkedList()
	{
		while (!isEmpty())
		{
			auto tmp = top;
			top = top->next;
			tmp->next = nullptr;
			delete tmp;
		}
	}

	bool isEmpty() {return top == nullptr;}

	T pop()
	{
		if (isEmpty())
		{
			throw "Pop() called on empty stack";
		}

		// Get top value to return
		T topVal = top->data;

		// Get top node and separate from list
		auto topNode = top;
		top = top->next;
		topNode->next = nullptr;
		delete topNode;

		return topVal;
	}

	void push(T value)
	{
		top = new StackNode(value, top);
	}

	T peek()
	{
		if (isEmpty())
		{
			throw "Peek() called on empty stack";
		}

		return top->data;
	}
};


template <typename T>
class QueueLinkedList
{
	class QueueNode
	{
		T data;
		QueueNode *next;

		QueueNode(T data, QueueNode *next = nullptr) : data(data), next(next) {}
		friend QueueLinkedList;
	};

	QueueNode *head;
	QueueNode *tail; // Require tail for fast method of adding to end of list

public:
	QueueLinkedList() : head(nullptr), tail(nullptr) {}

	~QueueLinkedList()
	{
		while (!isEmpty())
		{
			auto tmp = head;
			head = head->next;
			tmp->next = nullptr;
			delete tmp;
		}
	}

	bool isEmpty() { return head == nullptr; }

	void push(T value)
	{
		auto newNode = new QueueNode(value);
		if (isEmpty())
		{
			head = newNode;
		}
		else
		{
			tail->next = newNode;
		}
		tail = newNode;
	}

	T pop()
	{
		if (isEmpty())
		{
			throw "Pop() called on empty queue";
		}

		// Get head
		auto node = head;
		T val = node->data;

		// update head
		head = head->next;

		// delete node
		node->next = nullptr;
		delete node;

		// update tail
		if (head == nullptr)
		{
			tail = head;
		}

		return val;
	}

	T peek()
	{
		if (isEmpty()) throw "Peek() call on empty queue";
		return head->data;
	}
};


/* 1) array store (stackId, value), then search backwards for top of stack entry(last) O(n) -> then can pop(stackId) or search forward for open space for next entry
 * - not efficient
 * 2) can do better by keeping map to top of stack, however after pop() to make use of empty place either search or have list of open places
 * 3) might be best if using linkedlist instead of array, then don't need freespots as delete will remove node */
template <typename T>
class MultiStack
{
	vector<T> data;
	vector<T> prev; // could also use vector<stack>
	unordered_map<int, int> topOfStack;
	queue<int> freeSpots;

	bool isStackExist(int stackId)
	{
		return topOfStack.find(stackId) != topOfStack.end();
	}

public:
	MultiStack()
	{
		for (uint i = 0; i < data.size(); ++i)
		{
			freeSpots.push(i);
		}
	}

	bool isEmpty(int stackId)
	{
		return !isStackExist(stackId) || topOfStack[stackId] == -1;
	}

	void push(int stackId, T value)
	{
		// Get free spot
		int freeSpot = -1;
		if (!freeSpots.empty())
		{
			freeSpot = freeSpots.front();
			freeSpots.pop();
		}

		if (-1 < freeSpot)
		{	// If free spot exists in array
			if (topOfStack.find(stackId) != topOfStack.end())
			{	// Check if stack exists
				prev[freeSpot] = topOfStack[stackId];
			}
			else
			{	// Else create new stack
				prev[freeSpot] = -1;
			}
			data[freeSpot] = value;
			topOfStack[stackId] = freeSpot;
		}
		else
		{ 	// If no more free spots exist in array, push back
			if (topOfStack.find(stackId) != topOfStack.end())
			{	// Check if stack exists
				prev.push_back(topOfStack[stackId]);
			}
			else
			{	// Else create new stack
				prev.push_back(-1);
			}
			data.push_back(value);
			topOfStack[stackId] = data.size() - 1;
		}
	}

	T pop(int stackId)
	{
		if (topOfStack.find(stackId) == topOfStack.end() ||
				-1 == topOfStack[stackId])
		{
			throw "Pop() called on empty stack";
		}

		// Get data
		T value = data[topOfStack[stackId]];

		// Add to freespots
		freeSpots.push(topOfStack[stackId]);

		// update top of stack
		topOfStack[stackId] = prev[topOfStack[stackId]];

		return value;
	}
};


/* Three stacks in one array
 * 1) split stack into three equal sizes.
 * This wastes space.
 * With more information about stack usage could use difference sizes for each
 * 2) Just use any array index for any stacks values
 * Array, Prev array, top of stack array => requires O(n) extra space */
class FixedMultiStack
{
	int numberOfStacks = 3;
	int stackCapacity;
	int *values;
	int *sizes;

	/* Get index of top of stack */
	int indexOfTop(int stackNum)
	{
		int offset = stackNum * stackCapacity;
		return offset + (sizes[stackNum] - 1);
	}

public:
	FixedMultiStack(int stackSize) : stackCapacity(stackSize)
	{
		values = new int[stackSize * numberOfStacks]();
		sizes = new int[numberOfStacks]();
	}

	~FixedMultiStack()
	{
		delete[] values;
		delete[] sizes;
	}

	/* stackNum [0-N] */
	bool isFull(int stackNum)
	{
		return sizes[stackNum] == stackCapacity;
	}

	/* stackNum [0-N] */
	bool isEmpty(int stackNum)
	{
		return sizes[stackNum] == 0;
	}

	int getStackSize(int stackNum)
	{
		return sizes[stackNum];
	}

	int getNumStacks()
	{
		return numberOfStacks;
	}

	int getStackCapity()
	{
		return stackCapacity;
	}

	/* stackNum [0-N] */
	void push(int stackNum, int value)
	{
		if (isFull(stackNum))
		{
			throw "Push onto full stack";
		}

		++sizes[stackNum];
		int topIndex = indexOfTop(stackNum);
		values[topIndex] = value;
	}

	/* stackNum [0-N] */
	int pop(int stackNum)
	{
		if (isEmpty(stackNum))
		{
			throw "Pop from empty stack";
		}

		int topIndex = indexOfTop(stackNum);
		--sizes[stackNum];
		return values[topIndex];
	}

	/* stackNum [0-N] */
	int peek(int stackNum)
	{
		if (isEmpty(stackNum))
		{
			throw "Peek on empty stack";
		}

		int topIndex = indexOfTop(stackNum);
		return values[topIndex];
	}
};


template <typename T>
class MinStack
{
    class MinStackNode
    {
    public:
        T value;
        MinStackNode* min;

        MinStackNode (T value, MinStackNode *min = nullptr) : value(value), min(min) {}
        friend MinStack;
    };

	stack<MinStackNode*> st;

public:

	~MinStack()
	{
	    while (!st.empty())
	    {
	        MinStackNode *top = st.top();
	        st.pop();
	        delete top;
	    }
	}

	void push (T value)
	{
	    MinStackNode *newNode = new MinStackNode(value);

	    // Check if min exists on stack
	    if (!st.empty())
	    {
	        if (st.top()->min == nullptr)
	        {
                if (st.top()->value < value)
                {
                    newNode->min = st.top();
                }
	        }
	        else
	        {
	            if (st.top()->min->value < value)
                {
	                newNode->min = st.top()->min;
                }
	        }
	    }

        st.push(newNode);
	}

	T pop()
	{
		if (st.empty())
		{
			throw "Pop() called on empty stack";
		}

		T value = st.top()->value;
		st.pop();

		return value;
	}

	T min()
	{
		if (st.empty())
		{
			throw "Min() called on empty stack";
		}

		if (st.top()->min == nullptr)
		{
		    return st.top()->value;
		}

		return st.top()->min->value;
	}

	T top()
	{
		if (st.empty())
		{
			throw "top() called on empty stack";
		}

		return st.top()->value;
	}
};

template <typename T>
class SetOfStacks
{
	forward_list<stack<T>*> stacks;
	int capacity;
	int numStacks;

	stack<T>* getStack(int stackIdx)
	{
		if (stacks.empty() || numStacks < stackIdx + 1)
		{
			throw "Pop() called on empty stack";
		}

		auto itr = stacks.begin();
		for (int i = 0; i < stackIdx; ++i, ++itr);
		return *itr;
	}


public:
	SetOfStacks(int capacity) : capacity(capacity), numStacks(0) {}

	~SetOfStacks()
	{
	    while (!stacks.empty())
	    {
	        auto stack = stacks.front();
	        stacks.pop_front();
	        delete stack;
	    }
	}

	void push(int value)
	{
		if (stacks.empty() || (int)(stacks.front()->size()) == capacity)
		{
			stacks.push_front(new stack<T>());
			++numStacks;
		}

		stacks.front()->push(value);
	}

	T pop()
	{
		if (stacks.empty())
		{
			throw "Pop() called on empty stack";
		}

		T topValue = stacks.front()->top();
		stacks.front()->pop();

		if (stacks.front()->empty())
		{
			stacks.pop_front();
			--numStacks;
		}

		return topValue;
	}

	T popAt(int stackIdx)
	{
		if (stacks.empty() || numStacks < stackIdx + 1)
		{
			throw "Pop() called on empty stack";
		}

		stack<T> *stackAtIdx = getStack(stackIdx);

		T value = stackAtIdx->top();
		stackAtIdx->pop();

		// Do below or use array/vector or deque instead of stack, or make my own or don't care if stack not at full capacity
		if (stackIdx + 1 < numStacks)
		{
			stack<T> shiftStack;
			while (!stackAtIdx->empty())
			{
				shiftStack.push(stackAtIdx->top());
				stackAtIdx->pop();
			}
			stackAtIdx->push(popAt(stackIdx + 1));
			while (!shiftStack.empty())
			{
				stackAtIdx->push(shiftStack.top());
				shiftStack.pop();
			}
		}

		return value;
	}
};

template<typename T>
class QueueViaStacks
{
	stack<T> inStack;
	stack<T> outStack;

public:
	void push(T value)
	{
		inStack.push(value);
	}

	T pop()
	{
		if (outStack.empty())
		{
			while (!inStack.empty())
			{
				outStack.push(inStack.top());
				inStack.pop();
			}
		}

		T value = outStack.top();
		outStack.pop();
		return value;
	}
};


/* 1) Stack1, OrderedStack
 * pop from st1 to orderedStack, when order broken move from ordered to st1 until appropriate spot found for st1.top(), then push back st1 to ordered. and repeat
 * 2) st1 is the ordered one and the st2 is used at tmp to make space for appropriate position to make order */
template <typename T>
void sortStack(stack<T> &st)
{
	stack<T> orderSt;
	while (!st.empty())
	{
		T top = st.top();
		st.pop();

		// Move over unordered elements to find where to insert next item
		while (!orderSt.empty() && orderSt.top() > top)
		{
			st.push(orderSt.top());
			orderSt.pop();
		}
		// Insert new item
		orderSt.push(top);
	}

	/*// Same as above however thought I needed to count and handle more per a iteration, instead of letting future iterations deal with it
	 * while (!st.empty())
	{
		// Push inorder into orderSt (biggest at top)
		while (orderSt.empty() || (!st.empty() && st.top() > orderSt.top()))
		{
			orderSt.push(st.top());
			st.pop();
		}

		// Now reorder if top of stack is out of order to ordered stack
		if (!st.empty())
		{
			T top = st.top();
			st.pop();

			int count = 0;
			while (!orderSt.empty() && orderSt.top() > top)
			{
				st.push(orderSt.top());
				orderSt.pop();
				++count;
			}
			orderSt.push(top);
			while (count > 0)
			{
				orderSt.push(st.top());
				st.pop();
				--count;
			}
		}
	}*/

	while (!orderSt.empty())
	{
		st.push(orderSt.top());
		orderSt.pop();
	}
}



class Animal
{
protected:
	string name;
	uint age;
	std::chrono::time_point<std::chrono::high_resolution_clock> arrivalTime;

public:
	Animal(string name, uint age) : name(name), age(age)
	{
		arrivalTime = std::chrono::high_resolution_clock::now();
	}

	std::chrono::time_point<std::chrono::high_resolution_clock> arrivalTimeGet()
	{
		return arrivalTime;
	}

	string nameGet()
	{
		return name;
	}

	uint ageGet()
	{
		return age;
	}

	virtual ~Animal() = 0;//default;
};
Animal::~Animal() {cout << "Delete Animal " << name << " " << age << endl;}


class Dog : public Animal
{
public:
	Dog(string name, uint age) : Animal(name, age) {}

	~Dog() {cout << "Delete Dog " << name << " " << age << endl;}
};

class Cat : public Animal
{
public:
	Cat(string name, uint age) : Animal(name, age) {}

	~Cat() {cout << "Delete Cat " << name << " " << age << endl;}
};


class AnimalShelter
{
	list<unique_ptr<Dog>> dogs;
	list<unique_ptr<Cat>> cats;

public:
	void enqueue(unique_ptr<Animal> newAnimal)
	{
		Dog * newDog = dynamic_cast<Dog*>(newAnimal.get());
		Cat * newCat = dynamic_cast<Cat*>(newAnimal.get());
		if (nullptr != newDog)
		{
			unique_ptr<Dog> dogPtr(dynamic_cast<Dog*>(newAnimal.release()));
			dogs.push_back(move(dogPtr));
		}
		else if (nullptr != newCat)
		{
			unique_ptr<Cat> catPtr(dynamic_cast<Cat*>(newAnimal.release()));
			cats.push_back(move(catPtr));
		}
	}

	Animal* dequeueAny()
	{
	    if (cats.empty() && dogs.empty())
	    {
	        throw "dequeueAny() called on empty queue";
	    }

		Animal *animalPtr = nullptr;

		if (cats.empty())
		{
			animalPtr = static_cast<Animal*>(dequeueDog());
		}
		else if (dogs.empty())
		{
			animalPtr = static_cast<Animal*>(DequeueCat());
		}
		else
		{
			if (cats.front().get()->arrivalTimeGet() < dogs.front().get()->arrivalTimeGet())
			{
				animalPtr = static_cast<Animal*>(DequeueCat());
			}
			else
			{
				animalPtr = static_cast<Animal*>(dequeueDog());
			}
		}

		return animalPtr;
	}

	Dog* dequeueDog()
	{
        if (dogs.empty())
        {
            throw "dequeueAny() called on empty queue";
        }

		Dog* dog = dogs.front().release();
		dogs.pop_front();
		return dog;
	}

	Cat* DequeueCat()
	{
        if (cats.empty())
        {
            throw "dequeueAny() called on empty queue";
        }

		Cat* cat = cats.front().release();
		cats.pop_front();
		return cat;
	}
};

//-------------------------------------------------------------------------------------

class MultiStack2
{
	/* Adjust index to be within range of 0 -> length-1 */
	int adjustIndex(int index)
	{
		int max = sizeof(values) / sizeof(values[0]);
		return index % max; //((index % max) + max) % max;
	}

	/* Get next index, adjust for wrap around */
	int nextIndex(int index)
	{
		return adjustIndex(index + 1);
	}

	/* Get previous index, adjust for wrap around */
	int prevIndex(int index)
	{
		return adjustIndex(index - 1);
	}

	/* StackInfo is a simple class that holds a set of data about each stack, but not it's data
	 * Could have used variables in class to do this but would be more messy */
	struct StackInfo
	{
		int start, size, capacity;
		MultiStack2 *parent;

		StackInfo(int start, int capacity, MultiStack2 *parent) :
			start(start), size(0), capacity(capacity), parent(parent)  {}

		bool isFull()
		{
			return size == capacity;
		}

		bool isEmpty()
		{
			return size == 0;
		}

		/* Check if an index on the full array is within the stack boundaries.
		 * Stack can wrap around to start of array. */
		bool isWithinStackCapacity(int index)
		{
			int valuesSize = sizeof(values) / sizeof(int);
			/* Check if outside of bounds of array */
			if (index < 0 || index >= valuesSize)
			{
				return false;
			}

			/* If index wraps around, adjust it */
			int contigousIndex = index < start ? index + valuesSize : index;
			int end = start + capacity;
			return start <= contigousIndex && contigousIndex < end;
		}

		int lastCapacityIndex()
		{
			return parent->adjustIndex(start + capacity - 1);
		}

		int lastElementIndex()
		{
			return parent->adjustIndex(start + size - 1);
		}
	};

	StackInfo **info;
	int *values;
	int numberOfStacks;
	int stackSize;

	/* Shift items in stack over by one element
	 * If we have available capacity, then will end up shrinking stack by one element
	 * If don't have available capacity will need to shift next stack over */
	void shift(int stackNum)
	{
		if ((uint)stackNum > (sizeof(info) / sizeof(StackInfo*)))
		{
			return;
		}

		StackInfo *stack = info[stackNum];

		/* If stack is at it's full capacity then need to move next stack over by one element
		 * for stack to claim freed index */
		if (stack->size >= stack->capacity)
		{
			int nextStack = (stackNum + 1) % ((sizeof(info) / sizeof(StackInfo*)));
			shift(nextStack);
			++stack->capacity; // Claim index that next stack lost
		}

		/* Shift all elements in stack over by one */
		int index = stack->lastCapacityIndex();
		while (stack->isWithinStackCapacity(index))
		{
			values[index] = values[prevIndex((index))];
			index = prevIndex(index);
		}

		/* Adjust stack data */
		values[stack->start] = 0; // clear item
		stack->start = nextIndex(stack->start); // move start
		--(stack->capacity); // Shrink capacity
	}

	/* Expand stack by shifting over other stacks */
	void expand(int stackNum)
	{
		shift((stackNum + 1) % (sizeof(info) / sizeof(StackInfo*)));
		++(info[stackNum]->capacity);
	}
public:
	MultiStack2 (int numberOfStacks, int defaultSize) : numberOfStacks(numberOfStacks), stackSize(defaultSize)
	{
		/* Create metadata for all stacks */
		info = new StackInfo*[numberOfStacks];
		for (int i = 0; i < numberOfStacks; ++i)
		{
			info[i] = new StackInfo(defaultSize * i, defaultSize, this);
		}
		values = new int[numberOfStacks * defaultSize];
	}

	~MultiStack2()
	{
		for (int i = 0; i < numberOfStacks; ++i)
		{
			delete info[i];
		}
		delete[] info;
		delete[] values;
	}

	int numberOfElements()
	{
		int size = 0;
		for (int i = 0; i < numberOfStacks; ++i)
		{
			size += info[i]->size;
		}
		return size;
	}

	bool allStacksAreFull()
	{
		for (int i = 0; i < numberOfStacks; ++i)
		{
			if (!info[i]->isFull()) return false;
		}
		return true;
		return numberOfElements() == (sizeof(values) / sizeof(values[0]));
	}

	/* Push value onto stack num, shifting/expanding stacks as necessary */
	void push (int stackNum, int value)
	{
		if (allStacksAreFull())
		{
			throw "Push onto full stack";
		}

		/* If stack full expand */
		StackInfo *stack = info[stackNum];
		if (stack->isFull())
		{
			expand(stackNum);
		}

		/* Find index of top element in array + 1 and increment stack pointer */
		++(stack->size);
		values[stack->lastElementIndex()] = value;
	}

	int pop(int stackNum)
	{
		StackInfo *stack = info[stackNum];

		if (stack->isEmpty())
		{
			throw "Pop on empty stack";
		}

		/* Remove last element */
		int value = values[stack->lastElementIndex()];
		values[stack->lastElementIndex()] = 0; // clear item
		--(stack->size); // shrink size
		return value;
	}

	int peek(int stackNum)
	{
		StackInfo *stack = info[stackNum];
		return values[stack->lastElementIndex()];
	}
};


//------------------------------Tests------------------------------------------------

void StackLinkedListTest()
{
	const int N = 10;
	StackLinkedList<int> st;

	cout << "StackLinkedListTest: ";

	for (int i = 0; i < N; ++i)
	{
		st.push(i);
	}

	for (int i = 0; i < N; ++i)
	{
		cout << st.pop() << " ";
	}
	cout << endl;
}

void QueueLinkedListTest()
{
	const int N = 10;
	QueueLinkedList<int> queue;

	cout << "QueueLinkedListTest: ";

	for (int i = 0; i < N; ++i)
	{
		queue.push(i);
	}

	for (int i = 0; i < N; ++i)
	{
		cout << queue.pop() << " ";
	}
	cout << endl;
}

void FixedMultiStackTest()
{
	bool pass = true;

	FixedMultiStack fixedMultiStack(5);
	for (int stackId = 0; stackId < fixedMultiStack.getNumStacks(); ++stackId)
	{
		for (int i = 0; i < fixedMultiStack.getStackCapity(); ++i)
		{
			fixedMultiStack.push(stackId, i);
		}
	}

	for (int stackId = 0; stackId < fixedMultiStack.getNumStacks(); ++stackId)
	{
		for (int i = fixedMultiStack.getStackCapity() - 1; i >= 0; --i)
		{
			assert(pass &= (i == fixedMultiStack.pop(stackId)));
		}
	}

	cout << "FixedMultiStackTest Pass: " << pass << endl;
}

void MultiStackTest()
{
	bool pass = true;
	const int N = 5;
	const int STACKS = 3;

	MultiStack<int> multiStack;
	for (int i = 0; i < 2; ++i)
	{
		for (int i = 0; i < N; ++i)
		{
			for (int stackId = 0; stackId < STACKS; ++stackId)
			{
				multiStack.push(stackId, i);
			}
		}

		for (int stackId = 0; stackId < STACKS; ++stackId)
		{
			for (int i = N - 1; i >= 0; --i)
			{
				assert(pass &= (i == multiStack.pop(stackId)));
			}
		}
	}

	cout << "MultiStackTest Pass: " << pass << endl;
}

void MultiStack2Test()
{
	bool pass = true;

	MultiStack2 multiStack2(3, 5);
	for (int stackId = 0; stackId < 3; ++stackId)
	{
		for (int i = 0; i < 5; ++i)
		{
			multiStack2.push(stackId, i);
		}
	}

	for (int stackId = 0; stackId < 3; ++stackId)
	{
		for (int i = 5 - 1; i >= 0; --i)
		{
			assert(pass &= (i == multiStack2.pop(stackId)));
		}
	}

	cout << "MultiStack2Test Pass: " << pass << endl;
}

void MinStackTest()
{
	MinStack<int> minStack;
	const int N = 10;
	int data[N];
	int min[N];

	srand (time(NULL));

	for (int i = 0; i < N; ++i)
	{
		minStack.push(rand() % 100);
		printf("MinStack push: %d, min:%d\n", minStack.top(), minStack.min());
		data[i] = minStack.top();
		min[i] = minStack.min();
	}

	for (int i = N - 1; i >= 0; --i)
	{
		printf("MinStack pop: %d, min:%d\n", minStack.top(), minStack.min());
		assert(data[i] == minStack.top());
		assert(min[i] == minStack.min());
		minStack.pop();
	}

	cout << "MinStack Pass: 1" << endl;
}

void SetOfStacksTest()
{
	SetOfStacks<int> stacks(3);
	const int N = 12;

	for (int i = 1; i <= N; ++i)
	{
		stacks.push(i);
	}

	assert(9 == stacks.popAt(1));

	for (int i = 1, val = N; i < N; ++i, --val)
	{
		if (val == 9) --val;
		assert(val == stacks.pop());
	}

	cout << "SetOfStacks Pass: 1" << endl;
}

void QueueViaStacksTest()
{
	QueueViaStacks<int> queue;
	const int N = 8;

	for (int i = 0; i < N; ++i)
	{
		queue.push(i);
	}

	for (int i = 0; i < N; ++i)
	{
		assert(i == queue.pop());
	}

	cout << "QueueViaStacks Pass: 1" << endl;
}

/* Time: O(n^2)
 * Space: O(n) */
void SortStackTest()
{
	stack<int> st;
	const int N = 12;
	srand (time(NULL));

	for (int i = 0; i < N; ++i)
	{
		st.push(rand() % 100);
	}

	sortStack(st);

	int prev = st.top();
	st.pop();
	cout << prev << " ";
	for (int i = 1; i < N; ++i)
	{
		cout << st.top() << " ";
		assert(prev <= st.top());
		prev = st.top();
		st.pop();
	}

	cout << "SortStack Pass: 1" << endl;
}

void AnimalShelterTest()
{
	AnimalShelter as;

	cout << "AnimalShelterTest will take a while, please wait ..." << endl;

    srand (time(NULL));

	for (int i = 0; i < 4; ++i)
	{
		// Loop over
		for (int j = 0; j < 2; ++j)
		{
		    std::unique_ptr<Animal> basePointer;
		    if (j == 0)
		    {
		        unique_ptr<Dog> newDog = make_unique<Dog>("Dog" + to_string(i), i);
		        basePointer = move(newDog); // make_unique<Animal>(static_cast<Animal*>(newDog.release()));
		    }
		    else
		    {
                unique_ptr<Cat> newCat = make_unique<Cat>("Cat" + to_string(i), i);
                basePointer = move(newCat); //make_unique<Animal>(static_cast<Animal*>(newCat.release()));
		    }
            as.enqueue(move(basePointer));

            // Add a bit of delay for timestamps
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000));
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		// Loop over
		for (int j = 0; j < 2; ++j)
		{
			Animal* am = as.dequeueAny();

			Dog* dog = dynamic_cast<Dog*>(am);
			Cat* cat = dynamic_cast<Cat*>(am);
			int type = (nullptr != dog) ? 1 : (nullptr != cat) ? 2 : 3;


		    std::time_t tp = std::chrono::system_clock::to_time_t(am->arrivalTimeGet());
		    std::string sTp= std::asctime(std::gmtime(&tp));

			cout << "Type: " << type <<
			        ", Name: " << am->nameGet() <<
			        ", Age: " << am->ageGet() <<
			        ", Arrival: " << sTp;

			delete am;
		}
	}
}

//-------------------------------------------------------------------------------------


void Chap3StacksQueuesMain()
{
	StackLinkedListTest();
	QueueLinkedListTest();
	FixedMultiStackTest();
	MultiStackTest();
	//MultiStack2Test(); //Copied code but doesn't really work

	MinStackTest();
	SetOfStacksTest();
	QueueViaStacksTest();
	SortStackTest();
	AnimalShelterTest();
}
