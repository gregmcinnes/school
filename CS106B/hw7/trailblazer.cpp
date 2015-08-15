// Greg McInnes
// August 12, 2015
// CS106B HW7
// Trailblazer
// This file contains functions that can be used to find paths through mazes and maps.
// Four path finding different algorithms are implemented: depth first, breadth first,
// Dijstras, and A*.  Kruskals algorithm is also implemented to create mazes from
// a complete graph.

#include "trailblazer.h"
#include "basicgraph.h"
#include "queue.h"
#include "pqueue.h"
using namespace std;

Vector<Vertex*> depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*> path);
Vertex* breadthFirstSearchHelper(BasicGraph& graph, Vertex* end, Queue<Vertex*> queue);
Vector<Vertex*> getPath(Vertex* start, Vertex* end);
Vertex* aStarDijkstrasHelper(BasicGraph& graph, Vertex* start, Vertex* end, bool aStar);
bool graphCheck(BasicGraph& graph, Vertex* start, Vertex* end);

// Depth first search main function.  Finds a path between start and end
// using a depth first method
Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    if (!graphCheck(graph, start, end)) {
        return path;
    }
    start->setColor(4);
    path.add(start);
    path = depthFirstSearchHelper(graph, start, end, path);
    return path;
}

// Depth first search helper.  Recursively try all paths between start and end unil
// a path is found or all paths have been tried.
Vector<Vertex*> depthFirstSearchHelper(BasicGraph& graph, Vertex* start, Vertex* end, Vector<Vertex*> path) {
    if (start->name == end->name) {
        return path;
    }
    start->visited = true;
    for(Vertex* n : graph.getNeighbors(start)) {
        if (n->visited == true) {
            continue;
        }
        n->setColor(4);
        n->previous = start;
        Vector<Vertex*> tempPath = path;
        tempPath.add(n);
        Vector<Vertex*> result = depthFirstSearchHelper(graph, n, end, tempPath);
        if (result.size() > 0) {
            return result;
        }
    }
    start->setColor(2);
    path.clear();
    return path;
}

// Breadth first search main function.  Find a path between start and end using a
// breadth first implementation.
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    if (!graphCheck(graph, start, end)) {
        return path;
    }
    Queue<Vertex*> queue;
    queue.enqueue(start);
    Vertex* last = breadthFirstSearchHelper(graph, end, queue);
    if (last->visited == true
            ) {
        path = getPath(start, last);
    }
    return path;
}

// Breadth first helper.  Recursively search through graph until a path is found or
// all paths have been tried.
Vertex* breadthFirstSearchHelper(BasicGraph& graph, Vertex* end, Queue<Vertex*> queue) {
    if (queue.size() == 0) {
        Vertex* none;
        return none;
    }
    Vertex* current = queue.dequeue();
    current->setColor(4);
    if (current->name == end->name) {
        return end;
    }
    for (Vertex* c : graph.getNeighbors(current)) {
        if (c->visited == true) {
            continue;
        }
        c->visited = true;
        c->previous = current;
        c->setColor(3);
        queue.enqueue(c);
    }
    Vertex* result = breadthFirstSearchHelper(graph, end, queue);
    if (result == end) {
        return result;
    }
    Vertex* deadend;
    return deadend;
}

// Backtrack to find a path from start to end, then reverse it.
Vector<Vertex*> getPath(Vertex* start, Vertex* end) {
    Vector<Vertex*> backwards;
    Vector<Vertex*> path;
    Vertex* current = end;
    backwards.add(current);
    while (current != start) {
        try {
            current = current->previous;
        } catch (...) {
            return path;
        }
        backwards.add(current);
    }

    for (int i=backwards.size()-1; i>=0; i--) {
        path.add(backwards[i]);
    }
    return path;
}

// Dijkstras search implementation.
Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    if (!graphCheck(graph, start, end)) {
        return path;
    }
    for (Vertex* node: graph.getNodeSet()) {
        node->cost = POSITIVE_INFINITY;
    }
    start->cost = 0;
    Vertex* last = aStarDijkstrasHelper(graph, start, end, false);
    if (last->visited == true) {
        path = getPath(start, last);
    }
    return path;
}

// A* search implementation
Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    if (!graphCheck(graph, start, end)) {
        return path;
    }
    for (Vertex* node: graph.getNodeSet()) {
        node->cost = POSITIVE_INFINITY;
    }
    start->cost = 0;
    Vertex* last = aStarDijkstrasHelper(graph, start, end, true);
    if (last->visited == true) {
         path = getPath(start, last);
    }
    return path;
}

// A helper function for both A* and dijstras.  If A* is selected the heuristic cost will
// be added to the priority queue value.
Vertex* aStarDijkstrasHelper(BasicGraph& graph, Vertex* start, Vertex* end, bool aStar) {
    PriorityQueue<Vertex*> pqueue;
    double cost = 0;
    if (aStar) {
        cost = heuristicFunction(start, end);
    }
    pqueue.enqueue(start, cost);
    while (pqueue.size() > 0) {
        Vertex* current = pqueue.dequeue();
        current->setColor(4);
        current->visited = true;
        if (current->name == end->name) {
            return current;
        }
        for (Vertex* n: graph.getNeighbors(current)) {
            if (n->visited == true) {
                continue;
            }
            double newCost = current->cost + graph.getEdge(current, n)->cost;
            if (newCost < n->cost) {
                n->cost = newCost;
                n->previous = current;
                double priority = newCost;
                if (aStar) {
                    priority += heuristicFunction(n, end);
                }
                try {
                    pqueue.changePriority(n, priority);
                } catch (...) {
                    pqueue.enqueue(n, priority);
                }
            }
            n->setColor(3);
        }
    }
    Vertex* deadend;
    return deadend;
}

// Check if start and end vertices are in a graph
bool graphCheck(BasicGraph& graph, Vertex* start, Vertex* end) {
    if (!graph.containsVertex(start) || !graph.containsVertex(end)) {
        return false;
    }
    return true;
}

// Kruskals implementation.  Returns a graph with the least number of edges possible while still
// maintaining a complete tree.
Set<Edge*> kruskal(BasicGraph& graph) {
    PriorityQueue<Edge*> pqueue;
    Vector< Set<string> > vertices;
    Set<Edge*> edges = graph.getEdgeSet();
    while(!edges.isEmpty()) {
        Edge* e = edges.first();
        edges.remove(e);
        pqueue.enqueue(e, e->cost);
        bool exists = false;
        for (int i=0; i<vertices.size(); i++) {
            if (vertices[i].contains(e->start->name)) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            Set<string> node;
            node.add(e->start->name);
            vertices.add(node);
        }
    }
    Set<Edge*> mst;
    while (!pqueue.isEmpty()) {
        Edge* e = pqueue.dequeue();
        int startSet = -1;
        int endSet = -1;
        for (int i=0; i<vertices.size(); i++) {
            if (vertices[i].contains(e->start->name)) {
                startSet = i;
            }
            if (vertices[i].contains(e->end->name)) {
                endSet = i;
            }
        }
        if (startSet != endSet) {
            vertices[startSet].addAll(vertices[endSet]);
            vertices.remove(endSet);
            mst.add(e);
        }
    }
    return mst;
}


