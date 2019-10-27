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
using namespace std;

//-------------------------------------------------------------------------------------

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


//-------------------------------------------------------------------------------------

class MultiStack
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
		MultiStack *parent;

		StackInfo(int start, int capacity, MultiStack *parent) :
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
	MultiStack (int numberOfStacks, int defaultSize)
	{
		/* Create metadata for all stacks */
		info = new StackInfo*[numberOfStacks];
		for (int i = 0; i < numberOfStacks; ++i)
		{
			info[i] = new StackInfo(defaultSize * i, defaultSize, this);
		}
		values = new int[numberOfStacks * defaultSize];
	}

	~MultiStack()
	{
		for (uint i = 0; i < sizeof(info)/sizeof(StackInfo*); ++i)
		{
			delete info[i];
		}
		delete[] info;
		delete[] values;
	}

	int numberOfElements()
	{
		int size = 0;
		for (uint i = 0; i < sizeof(info) / sizeof(StackInfo*); ++i)
		{
			size += info[i]->size;
		}
		return size;
	}

	bool allStacksAreFull()
	{
		return numberOfElements() == (sizeof(values) / sizeof(int));
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

void MultiStackTest()
{
	bool pass = true;

	MultiStack multiStack(3, 5);
	for (int stackId = 0; stackId < 3; ++stackId)
	{
		for (int i = 0; i < 5; ++i)
		{
			multiStack.push(stackId, i);
		}
	}

	for (int stackId = 0; stackId < 3; ++stackId)
	{
		for (int i = 5 - 1; i >= 0; --i)
		{
			assert(pass &= (i == multiStack.pop(stackId)));
		}
	}

	cout << "MultiStackTest Pass: " << pass << endl;
}

//-------------------------------------------------------------------------------------

class MultiArrayStack
{
	vector<int> stackData;
	vector<int> prevToOfStack;
	vector<int> topOfStack;

public:
	void push(int stackId, int value)
	{
		int index = stackData.size();
		bool firstEntry = (index == 0);

		stackData.push_back(value);
		if (firstEntry)
		{
			prevToOfStack.push_back(-1);

			if (topOfStack.size() < stackId + 1)
			{
				while (topOfStack.size() < stackId)
				{
					topOfStack.push_back(-1);
				}
				topOfStack.push_back(index);
			}
			else
			{
				topOfStack[stackId] = index;
			}
		}
		else
		{
			prevToOfStack[index] = topOfStack[stackId];
			topOfStack[stackId] = stackData.size() - 1;
		}
	}
};

void Chap3StacksQueuesMain()
{
	FixedMultiStackTest();
	// MultiStackTest(); Copied code but doesn't really work
}
