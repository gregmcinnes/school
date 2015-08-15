/* Greg McInnes
 * CS106B
 * 7/31/2015
 *
 * LinkedPriorityQueue.cpp
 * This is the class file for LinkedPriorityQueue.  Public and private functions and
 * variables are defined in this file.  LinkedPriorityQueue stores queued elements
 * in a linked list.
 */


#include "LinkedPriorityQueue.h"
#include "linkedlist.h"
#include "strlib.h"
#include "vector.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
    mySize = 0;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    if (!isEmpty()) {
        ListNode* current = front;
        while (current != NULL) {
            ListNode* next = current->next;
            free(current);
            current = next;
        }
    }
}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    if (isEmpty()) {
        throw "Empty queue!";
    }
    bool found = false;
    ListNode* current = front;
    ListNode* last = current;
    for (int i = 0; i < mySize; i++) {
        int currentPriority;
        string currentValue;
        stringToValues(current->value, currentValue, currentPriority);
        if (currentValue == value) {
            if (currentPriority <= newPriority) {
                throw "New priority is not less than current priority!";
            }
            found = true;
            last->next = current->next;
            mySize--;
            break;
        }
        last = current;
        current = current->next;
    }
    if (found == false) {
        throw "Value not found in queue!";
    }
    enqueue(value, newPriority);
}

void LinkedPriorityQueue::clear() {
    if (!isEmpty()) {
        ListNode* current = front;
        while (current != NULL) {
            ListNode* next = current->next;
            free(current);
            current = next;
        }
        front = NULL;
    }
    mySize = 0;
}

string LinkedPriorityQueue::dequeue() {
    if (isEmpty()) {
        throw "Empty queue!";
    }
    string value = front->value;
    front = front->next;
    mySize--;
    string currentValue;
    int currentPriority;
    stringToValues(value, currentValue, currentPriority);
    return currentValue;
}

void LinkedPriorityQueue::enqueue(string value, int priority) {
    string vpString = valueString(value, priority);
    if (front == NULL) {
        front = new ListNode(vpString);
        mySize++;
    } else {
        ListNode* current = front;
        ListNode* last = current;
        for (int i = 0; i < mySize; i++) {
            int currentPriority;
            string currentValue;
            stringToValues(current->value, currentValue, currentPriority);
            if (priority < currentPriority || (priority == currentPriority && value < currentValue)) {
                if (i == 0) {
                    ListNode* oldFront = front;
                    front = new ListNode(vpString);
                    front->next = oldFront;
                } else {
                    ListNode* newNode = new ListNode(vpString);
                    newNode->next = last->next;
                    last->next = newNode;
                }
                mySize++;
                return;
            }

            if (current->next == NULL) {
                ListNode* newNode = new ListNode(vpString);
                current->next = newNode;
                mySize++;
                return;
            }
            last = current;
            current = current->next;
        }
    }
}

bool LinkedPriorityQueue::isEmpty() const {
    return mySize == 0;
}

string LinkedPriorityQueue::peek() const {
    if (isEmpty()) {
        throw "Empty queue!";
    }
    int priority;
    string value;
    stringToValues(front->value, value, priority);
    return value;
}

int LinkedPriorityQueue::peekPriority() const {
    if (isEmpty()) {
        throw "Empty queue!";
    }
    int priority;
    string value;
    stringToValues(front->value, value, priority);
    return priority;
}

int LinkedPriorityQueue::size() const {
    return mySize;
}

string LinkedPriorityQueue::valueString(string value, int priority) {
    return "\"" + value + "\":" + integerToString(priority);
}

void LinkedPriorityQueue::stringToValues(string valueString, string & value, int & priority) const {
    Vector<string> currentVector = stringSplit(valueString, ":");
    value = stringReplace(currentVector[0], "\"", "");
    priority = stringToInteger(currentVector[1]);
}

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";

    ListNode* current = queue.front;
    if (current != NULL) {
        out << current->value;
        while (current->next != NULL) {
            current = current->next;
            out << ", " << current->value;
        }
    }

    out << "}";
    return out;
}
