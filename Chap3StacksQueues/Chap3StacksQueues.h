/*
 * Chap3StacksQueues.h
 *
 *  Created on: 2019-10-02
 *      Author: ckirsch
 */

#ifndef CHAP3STACKSQUEUES_CHAP3STACKSQUEUES_H_
#define CHAP3STACKSQUEUES_CHAP3STACKSQUEUES_H_


template <typename T>
class MyStack
{
	struct StackNode
	{
		T data;
		StackNode *next;

		StackNode(T data) : data (data), next(nullptr) {}
	};

	StackNode *top;

public:
	T pop()
	{
		if (top == nullptr)
		{
			throw "Pop() called on empty stack";
		}

		T item = top->data;
		auto topLocal = top;
		top = top->next;
		delete topLocal;
		return item;
	}

	void push(T item)
	{
		auto newTop = new StackNode(item);
		newTop->next = top;
		top = newTop;
	}

	T peek()
	{
		if (top == nullptr)
		{
			throw "Peek on empty stack";
		}
		return top->data;
	}

	bool isEmpty()
	{
		return (top == nullptr);
	}
};


template <typename T>
class MyQueue
{
	struct QueueNode
	{
		T data;
		QueueNode *next;

		QueueNode(T data) : data(data), next(nullptr) {}
	};

	QueueNode *first;
	QueueNode *last;

	void add(T item)
	{
		auto newNode = new QueueNode(item);
		if (last != nullptr)
		{
			last->next = newNode;
		}
		last = newNode;
		if (first == nullptr)
		{
			first = last;
		}
	}

	T remove()
	{
		if (first == nullptr)
		{
			throw "remove called on empty queue";
		}

		auto top = first;
		T data = first->data;
		first = first->next;
		delete top;
		if (first == nullptr)
		{
			last = nullptr;
		}
		return data;
	}

	T peek()
	{
		if (first == nullptr)
		{
			throw "peek called on empty queue";
		}
		return first->data;
	}

	bool isEmpty()
	{
		return (first == nullptr);
	}
};


void Chap3StacksQueuesMain();


#endif /* CHAP3STACKSQUEUES_CHAP3STACKSQUEUES_H_ */
