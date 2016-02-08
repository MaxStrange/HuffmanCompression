#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_


#include "stdafx.h"
#include <vector>

#include "Heap.h"

using namespace std;

template<class T>
class PriorityQueue
{
public:
	PriorityQueue();
	PriorityQueue(bool minQueue);
	~PriorityQueue();

	bool isEmpty() const;
	int getLength() const;

	void Add(const T * const item);
	void AddEach(const vector<T> &items);
	T Remove();

private:
	bool minQueue = true;
	Heap<T> *heap = nullptr;
};




template<class T>
PriorityQueue<T>::PriorityQueue()
{
	this->heap = new Heap<T>();
}

template<class T>
PriorityQueue<T>::PriorityQueue(bool minQueue)
{
	this->minQueue = minQueue;
	this->heap = new Heap<T>(minQueue);
}

template<class T>
PriorityQueue<T>::~PriorityQueue()
{
	delete this->heap;
}

template<class T>
inline bool PriorityQueue<T>::isEmpty() const
{
	return this->heap->isEmpty();
}

template<class T>
inline int PriorityQueue<T>::getLength() const
{
	return this->heap->getSize();
}

template<class T>
void PriorityQueue<T>::Add(const T * const item)
{
	this->heap->Add(item);
}

template<class T>
void PriorityQueue<T>::AddEach(const vector<T> &items)
{
	for (unsigned int i = 0; i < items.size(); i++)
	{
		this->Add(&items.at(i));
	}
}

template<class T>
T PriorityQueue<T>::Remove()
{
	T retrieved;
	this->heap->Retrieve(retrieved);
	return retrieved;
}

#endif //_PRIORITY_QUEUE_H_
