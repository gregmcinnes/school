/* Greg McInnes
 * CS106B
 * 7/31/2015
 *
 * LinkedPriorityQueue.h
 * This is the header file for LinkedPriorityQueue.  Public and private functions and
 * variables are defined in this file.  LinkedPriorityQueue stores queued elements
 * in a linked list.
 */

#ifndef _linkedpriorityqueue_h
#define _linkedpriorityqueue_h

#include <cstddef>    // for NULL
#include <iostream>
#include <string>
#include "ListNode.h"
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see ArrayPriorityQueue.h.
 */
class LinkedPriorityQueue {
public:
    LinkedPriorityQueue();
    ~LinkedPriorityQueue();
    void changePriority(string value, int newPriority);// O(N)
    void clear();// O(1)
    string dequeue();// O(1)
    void enqueue(string value, int priority);// O(N)
    bool isEmpty() const;// O(1)
    string peek() const;// O(1)
    int peekPriority() const;// O(1)
    int size() const;// O(1)
    friend ostream& operator <<(ostream& out, const LinkedPriorityQueue& queue);// O(N)

private:
    // The first node of the linked list
    ListNode* front;
    // The number of elements in the queue
    int mySize;
    // A function to convert the value and priority in a string that can be stored in a ListNode
    string valueString(string value, int priority);
    // A function to return the value and priority from a value/priority string
    void stringToValues(string valueString, string & value, int & priority) const;
};

#endif
