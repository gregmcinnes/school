/* Greg McInnes
 * CS106B
 * 7/31/2015
 *
 * ArrayPriorityQueue.h
 * This is the header file for ArrayPriorityQueue.  Public and private functions and
 * variables are defined in this file.  ArrayPriorityQueue stores queued elements
 * in an array.
 */

#ifndef _arraypriorityqueue_h
#define _arraypriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
#include "map.h"
#include "hashmap.h"
using namespace std;

class ArrayPriorityQueue {
public:
    // Constructor
    ArrayPriorityQueue();
    // Destructor
    ~ArrayPriorityQueue();
    // Change the priority of an existing value - O(N)
    void changePriority(string value, int newPriority);
    // Clear all values - O(1)
    void clear();
    // Dequeue the lowest priority value and return it  - O(N)
    string dequeue();
    // Enqueue a value with a given priority - O(1)
    void enqueue(string value, int priority);
    // Check if the queue is empty.  Return true if empty - O(1)
    bool isEmpty() const;
    // Peek a the lowest priority value without removing it - O(N)
    string peek() const;
    // Peek at the lowest priority without removing the value - O(N)
    int peekPriority() const;
    // Check the number of elements stored - O(1)
    int size() const;
    // Print the queue - O(N)
    friend ostream& operator <<(ostream& out, const ArrayPriorityQueue& queue);

private:
    // Array to store the queue elements
    HashMap<string, int>* myElements;
    // Number of elements in the array
    int mySize;
    // Capacity of the current array
    int myCapacity;
    // A function to check if an array resize is necessary
    void checkResize();
};

#endif
