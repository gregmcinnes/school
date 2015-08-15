/* Greg McInnes
 * CS106B
 * 7/31/2015
 *
 * HeapPriorityQueue.h
 * This is the header file for HeapPriorityQueue.  Public and private functions and
 * variables are defined in this file.  HeapPriorityQueue stores queued elements
 * in a heap.
 */

#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
#include "hashmap.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class HeapPriorityQueue {
public:
    HeapPriorityQueue();
    ~HeapPriorityQueue();
    void changePriority(string value, int newPriority);// O(NlogN)
    void clear();// O(N)
    string dequeue();// O(logN)
    void enqueue(string value, int priority);// O(logN)
    bool isEmpty() const;// O(1)
    string peek() const;// O(1)
    int peekPriority() const;// O(1)
    int size() const;// O(1)
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue);// O(N)

private:
    // array storing element data
    HashMap<string, int>* myElements;
    // number of elements in the array
    int mySize;
    // The size of the array
    int myCapacity;
    // Check if a new array is necessary
    void checkResize();
    // Function to "bubble up" new or reprioritized values
    void bubbleUp(int index);
    // Function to "bubble down" values after dequeueing
    void bubbleDown(int index);
    // Check if there are no values and throw an error if none
    void emptyCheck() const;
    // Get the value and priority from the HashMap storage
    void getValues(HashMap<string, int> element, string & value, int & priority) const;
    // Get the priority from a specific index
    int getPriority(int index);
    // Get the value from a specific index
    string getValue(int index);
};

#endif
