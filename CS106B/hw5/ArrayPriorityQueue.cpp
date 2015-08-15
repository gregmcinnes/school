/* Greg McInnes
 * CS106B
 * 7/31/2015
 *
 * ArrayPriorityQueue.cpp
 * This is the class file for ArrayPriorityQueue.  Public and private functions and
 * variables are defined in this file.  ArrayPriorityQueue stores queued elements
 * in an array.
 *
 * Citations:
 * The ArrayList class we built during lecture was used as a foundation for this code.
 */


#include "ArrayPriorityQueue.h"
#include "map.h"
#include "hashmap.h"

ArrayPriorityQueue::ArrayPriorityQueue() {
    myElements = new HashMap<string, int>[10]();
    mySize = 0;
    myCapacity = 10;
}

ArrayPriorityQueue::~ArrayPriorityQueue() {
    delete[] myElements;
}

void ArrayPriorityQueue::changePriority(string value, int newPriority) {
    for (int i = 0; i < mySize; i++) {
        for (string k: myElements[i]) {
            if (k == value) {
                if (myElements[i][k] <= newPriority) {
                    throw "New priority less urgent than current priority!";
                }
                myElements[i][k] = newPriority;
                return;
            }
        }
    }
    throw "Value not found in priority queue!";
}

void ArrayPriorityQueue::clear() {
    mySize = 0;
}

string ArrayPriorityQueue::dequeue() {
    if (isEmpty()) {
        throw "Empty priority queue!";
    }
    int lowest = std::numeric_limits<int>::max();
    string value;
    int index;
    for (int i = 0; i < mySize; i++) {
        for (string k: myElements[i]) {
            if (myElements[i][k] < lowest) {
                lowest = myElements[i][k];
                value = k;
                index = i;
            } else if (myElements[i][k] == lowest && k < value) {
                lowest = myElements[i][k];
                value = k;
                index = i;
            }
        }
    }
    for (int i = index; i < mySize; i++) {
        myElements[i] = myElements[i + 1];
    }
    mySize--;
    return value;
}

void ArrayPriorityQueue::enqueue(string value, int priority) {
    checkResize();
    HashMap<string, int> newEntry;
    newEntry.add(value, priority);
    myElements[mySize] = newEntry;
    mySize++;
}

bool ArrayPriorityQueue::isEmpty() const {
    return mySize == 0;
}

string ArrayPriorityQueue::peek() const {
    if (isEmpty()) {
        throw "Empty priority queue!";
    }
    int lowest = std::numeric_limits<int>::max();
    string value;
    for (int i = 0; i < mySize; i++) {
        for (string k: myElements[i]) {
            if (myElements[i][k] < lowest) {
                lowest = myElements[i][k];
                value = k;
            }
        }
    }
    return value;
}

int ArrayPriorityQueue::peekPriority() const {
    if (isEmpty()) {
        throw "Empty priority queue!";
    }
    int lowest = std::numeric_limits<int>::max();
    string value;
    for (int i = 0; i < mySize; i++) {
        for (string k: myElements[i]) {
            if (myElements[i][k] < lowest) {
                lowest = myElements[i][k];
                value = k;
            }
        }
    }
    return lowest;
}

int ArrayPriorityQueue::size() const {
    return mySize;
}

ostream& operator<<(ostream& out, const ArrayPriorityQueue& queue) {
    out << "{";
    if (!queue.isEmpty()) {
        for (int i = 0; i < queue.size(); i++) {
            if (i > 0) { out << ", ";}
            for (string k : queue.myElements[i]) {
                out << "\"" << k << "\":" << queue.myElements[i][k];
            }
        }
    }
    out << "}";
    return out;
}

void ArrayPriorityQueue::checkResize() {
    if (mySize == myCapacity) {
        // copy all elements into a bigger array
        HashMap<string, int>* bigger = new HashMap<string, int>[2 * myCapacity];
        for (int i = 0; i < mySize; i++) {
            bigger[i] = myElements[i];
        }

        // swap in the new bigger array for the old one
        delete[] myElements;
        myElements = bigger;
        myCapacity *= 2;
    }
}
