/* Greg McInnes
 * CS106B
 * 7/31/2015
 *
 * HeapPriorityQueue.cpp
 * This is the class file for HeapPriorityQueue.  Public and private functions and
 * variables are defined in this file.  HeapPriorityQueue stores queued elements
 * in a heap.
 *
 * Citations:
 * Parts of the ArrayList class we made during lecture were used in this code.
 */


#include "HeapPriorityQueue.h"
#include "hashmap.h"

HeapPriorityQueue::HeapPriorityQueue() {
    myElements = new HashMap<string, int>[10]();
    mySize = 0;
    myCapacity = 10;
}

HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] myElements;
}

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    emptyCheck();
    for (int i=1; i <= mySize; i++) {
        string currentValue;
        int currentPriority;
        getValues(myElements[i], currentValue, currentPriority);
        if (value == currentValue) {
            myElements[i][value] = newPriority;
            bubbleUp(i);
        }
    }
}

void HeapPriorityQueue::clear() {
    mySize = 0;
}

string HeapPriorityQueue::dequeue() {
    string value;
    int priority;
    getValues(myElements[1], value, priority);
    myElements[1] = myElements[mySize];
    mySize--;
    bubbleDown(1);
    return value;
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    checkResize();
    HashMap<string, int> newEntry;
    newEntry.add(value, priority);
    myElements[mySize+1] = newEntry;
    mySize++;
    bubbleUp(mySize);
}

bool HeapPriorityQueue::isEmpty() const {
    return mySize == 0;
}

string HeapPriorityQueue::peek() const {
    emptyCheck();
    string value;
    int priority;
    getValues(myElements[1], value, priority);
    return value;
}

int HeapPriorityQueue::peekPriority() const {
    emptyCheck();
    string value;
    int priority;
    getValues(myElements[1], value, priority);
    return priority;
}

int HeapPriorityQueue::size() const {
    return mySize;
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    if (!queue.isEmpty()) {
        for (int i = 1; i <= queue.size(); i++) {
            if (i > 1) { out << ", ";}
            for (string k : queue.myElements[i]) {
                out << "\"" << k << "\":" << queue.myElements[i][k];
            }
        }
    }
    out << "}";
    return out;
}

void HeapPriorityQueue::checkResize() {
    if (mySize == myCapacity-1) {
        // copy all elements into a bigger array
        HashMap<string, int>* bigger = new HashMap<string, int>[2 * myCapacity];
        for (int i = 1; i <= mySize; i++) {
            bigger[i] = myElements[i];
        }

        // swap in the new bigger array for the old one
        delete[] myElements;
        myElements = bigger;
        myCapacity *= 2;
    }
}

void HeapPriorityQueue::bubbleUp(int index) {
    if (index == 1) {
        return;
    } else {
        if (getPriority(index) < getPriority(index/2) ||
                getPriority(index) == getPriority(index/2) && getValue(index) < getValue(index/2)) {
            HashMap<string, int> temp = myElements[index];
            myElements[index] = myElements[index/2];
            myElements[index/2] = temp;
            bubbleUp(index/2);
        }
    }
}

void HeapPriorityQueue::bubbleDown(int index) {
    if (index*2 > mySize) {
        return;
    } else if (getPriority(index) > getPriority(index*2) || getPriority(index) > getPriority(index*2+1) ||
               getPriority(index) == getPriority(index*2) && getValue(index) > getValue(index) ||
               getPriority(index) == getPriority(index*2+1) && getValue(index) > getValue(index+1)) {
        int lower;
        if (getPriority(index*2) < getPriority(index*2+1)) {
            lower = index*2;
        } else if (getPriority(index*2) == getPriority(index*2+1) && getValue(index*2) < getValue(index*2+1)) {
            lower = index*2;
        } else {
            lower = index*2+1;
        }
        HashMap<string, int> temp = myElements[index];
        myElements[index] = myElements[lower];
        myElements[lower] = temp;
        bubbleDown(lower);
    } else {
        return;
    }
}

void HeapPriorityQueue::emptyCheck() const {
    if(isEmpty()) {
        throw "Empty queue!";
    }
}

void HeapPriorityQueue::getValues(HashMap<string, int> element, string &value, int &priority) const {
    for (string k : element) {
        value = k;
        priority = element[k];
        return;
    }
}

int HeapPriorityQueue::getPriority(int index) {
    for (string k : myElements[index]) {
        int priority = myElements[index][k];
        return priority;
    }
    return 0;
}

string HeapPriorityQueue::getValue(int index) {
    for (string k : myElements[index]) {
        return k;
    }
    return "";
}
