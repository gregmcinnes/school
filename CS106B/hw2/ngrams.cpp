// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

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

void print_header() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams').\n"
            "This program makes random text based on a document.\n"
            "Give me an input file and an 'N' value for groups\n"
            "of words, and I'll create random text for you.\n" << endl;
}

string get_file() {
    string file = promptUserForFile("Input file name? ");
    return file;
}

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

Map<Vector<string>, Vector<string>> create_map(Queue<string> & text_queue, int & n) {
    // setup initial
    Map<Vector<string>, Vector<string>> ngram_map;

    Vector<string> key_vector;
    for (int i=0; i < n; i++) {
        key_vector.add(text_queue.dequeue());
    }
    string current_value = text_queue.dequeue();

    for (int i; i < text_queue.size() + 2*n; i++) {
        ngram_map[key_vector] += current_value;
        text_queue.enqueue(key_vector[0]);
        key_vector.remove(0);
        key_vector.add(current_value);
        current_value = text_queue.dequeue();
    }
    return ngram_map;
}

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

bool generate_output(Map<Vector<string>, Vector<string>> & text_map, int & n) {
    int word_count = get_number_of_words(n);
    if (word_count == 0) {
        return false;
    }
    int start = randomInteger(0, text_map.size() - 1);
    Vector<Vector<string>> keys = text_map.keys();
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
