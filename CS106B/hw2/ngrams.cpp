// Greg McInnes
// CS 106B
// Homework 2
// July 7, 2015
//
// N-Grams
// ngrams.cpp
// This program produces random text based on provided literature.  The user
// submits a body of text and a number which is used to break up the text into
// phrases.  The program then outputs a randomly generated passage of a defined
// length.

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "queue.h"
#include "map.h"
#include "vector.h"
#include "random.h"

using namespace std;

// Function prototypes
void print_header();
string get_file();
int get_n();
int get_number_of_words(int n);
Queue<string> read_file(string text_file);
Map<Vector<string>, Vector<string>> create_map(Queue<string> & text_queue, int & n);
string get_max(Map<string, int> & count_map);
string choose_next(Vector<string> & values);
bool generate_output(Map<Vector<string>, Vector<string>> & text_map, int & n);

// Main
int main() {
    print_header();
    string text_file = get_file();
    Queue<string> text_queue = read_file(text_file);
    int n = get_n();
    Map<Vector<string>, Vector<string>> text_map = create_map(text_queue, n);
    bool play = true;
    while(play) {
        play = generate_output(text_map, n);
    }
    cout << "Exiting." << endl;
    return 0;
}

// Print welcome message
void print_header() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams').\n"
            "This program makes random text based on a document.\n"
            "Give me an input file and an 'N' value for groups\n"
            "of words, and I'll create random text for you.\n" << endl;
}

// Get text file from user
string get_file() {
    string file = promptUserForFile("Input file name? ");
    return file;
}

// Get N
int get_n() {
    int n = 0;
    while (n < 2) {
        n = getInteger("Value of N? ");
        if (n < 2) {
            cout << "N must be 2 or greater." << endl;
        }
    }
    return n;
}

// Get the number of words to generate
int get_number_of_words(int n) {
    int word_count = 0;
    while (word_count < n) {
        word_count = getInteger("\n# of random words to generate (0 to quit)? ");
        if (word_count == 0) {
            break;
        } else if (word_count < n) {
            cout << "Must be at least " << n << " words." << endl;
        }
    }
    return word_count;
}

// Read the file into a queue of words
// text_file: user provided text file
Queue<string> read_file(string text_file) {
    Queue<string> text_queue;
    ifstream input;
    input.open(text_file.c_str());
    string word;
    while (input >> word) {
        text_queue.enqueue(word);
    }
    return text_queue;
}

// Create a map of the words in the queue.  The key will be of length N,
// the value will be a vector of possible next words.
// text_queue: a queue of words from the text file
// n: the number of words to use for the key
Map<Vector<string>, Vector<string>> create_map(Queue<string> & text_queue, int & n) {
    Map<Vector<string>, Vector<string>> ngram_map;
    Vector<string> key_vector;
    for (int i=0; i < n; i++) {
        key_vector.add(text_queue.dequeue());
    }
    string current_value = text_queue.dequeue();
    // size + 2*n is used because we remove n words to start off in the preceeding lines.
    // Also we need to wrap around fromt the last word in the file to the first word.
    for (int i; i < text_queue.size() + 2*n; i++) {
        ngram_map[key_vector] += current_value;
        text_queue.enqueue(key_vector[0]);
        key_vector.remove(0);
        key_vector.add(current_value);
        current_value = text_queue.dequeue();
    }
    return ngram_map;
}

// Find the value that occurs the most
// count_map: a map of words and the number of times they appear
string get_max(Map<string, int> & count_map) {
    int max = 0;
    for (string word : count_map) {
        if (max < count_map[word]) {
            max = count_map[word];
        }
    }
    Vector<string> max_words;
    for (string word : count_map) {
        if (count_map[word] == max) {
            max_words.add(word);
        }
    }
    if (max_words.size() == 1) {
        return max_words[0];
    } else {
        int rand = randomInteger(0, max_words.size() - 1);
        return max_words[rand];
    }
}

// Select the next word to be output
// values: The possbile values to output.  The value that appears most will be output.  If there is a tie, one
// is selected at random.
string choose_next(Vector<string> & values) {
    if (values.size() == 1) {
        return values[0];
    }
    Map<string, int> value_counts;
    string next;
    for (int i=0; i < values.size(); i++) {
        if (value_counts.containsKey(values[i])) {
            value_counts[values[i]]++;
        } else {
            value_counts[values[i]] = 1;
        }
    }
    next = get_max(value_counts);
    return next;
}

// Print the random output
// text_map: a map of words and their possible successors
// n: the number of words to use as the root
bool generate_output(Map<Vector<string>, Vector<string>> & text_map, int & n) {
    int word_count = get_number_of_words(n);
    if (word_count == 0) {
        return false;
    }
    Vector<Vector<string>> keys = text_map.keys();
    int start = randomInteger(0, keys.size() - 1);
    Vector<string> current = keys[start];
    cout << "... ";
    for (string k: current) {
        cout << k << " ";
    }
    for (int i=0; i < word_count; i++) {
        string next = choose_next(text_map[current]);
        cout << next << " ";
        current.remove(0);
        current.add(next);
    }
    cout << "..." << endl;
    return true;
}


