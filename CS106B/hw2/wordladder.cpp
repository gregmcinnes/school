// Greg McInnes
// CS 106B
// Homework 2
// July 7, 2015
//
// Word Ladders
// wordladder.cpp
// This program takes two words and finds the shortest path
// from one two the other by changing one letter at a time.
// All intermediate words must be found in a user provided
// dictionary.


#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "lexicon.h"
#include "stack.h"
#include "queue.h"
#include "strlib.h"
#include "simpio.h"

using namespace std;

// Function prototypes
void print_header();
string get_dictionary();
string get_words(int word_number);
Lexicon fill_lexicon(string dictionary_file);
void check_new_word(string & new_word, string & word2, Lexicon & dictionary, Lexicon & used, Stack<string> & new_stack,
                    Queue<Stack<string>> & word_queue, Stack<string> & finished_stack);
void check_neighbors(string word1, string word2, Lexicon & dictionary, Stack<string> current_stack,
                     Queue<Stack<string>> & word_queue, Lexicon & used, Stack<string> & finished_stack);
void word_ladder(string & word1, string & word2, Lexicon & dictionary, Stack<string> & best_path);
void print_stack(string word1, string word2, Stack<string> stack);
bool check_words(string & word1, string & word2, Lexicon & dictionary);
bool play_word_ladder(Lexicon & dictionary);

// Main function
int main() {
    print_header();
    string dictionary_file = get_dictionary();
    Lexicon dictionary = fill_lexicon(dictionary_file);
    bool play = true;
    while (play) {
        play = play_word_ladder(dictionary);
    }
    cout << "Have a nice day." << endl;
    return 0;
}

// Print header
void print_header() {
    cout << "Welcome to CS 106B Word Ladder.\n"
            "Please give me two English words, and I will change the\n"
            "first into the second by changing one letter at a time.\n" << endl;
}

// Have user define dictionary
string get_dictionary() {
    ifstream input;
    string file = promptUserForFile(input, "Dictionary file name?", "Unable to open that file.  Try again.");
    //string file = "smalldict1.txt";  // TESTING
    cout << endl;
    return file;
}

// Have user enter words
// word_number: 1 or 2, which word is being entered
string get_words(int word_number) {
    string word = getLine("Word #" + integerToString(word_number) + " (or Enter to quit): ");
    return word;
}

// Create dictionary lexicon
// dictionary_file: user defined dictionary file
Lexicon fill_lexicon(string dictionary_file) {
    Lexicon dictionary;
    dictionary.addWordsFromFile(dictionary_file);
    return dictionary;
}

// Check new word
// new_word: word to be checked
// word2: goal word in the word ladder
// dictionary: user provided dictionary
// used: all words that have been previously used in the word ladders
// new_stack: the stack currently being tested
// word_queue: queue of all stacks of potential word ladders
// finished_stack: an empty stack.  If the goal word is found this will be populated
void check_new_word(string & new_word, string & word2, Lexicon & dictionary, Lexicon & used, Stack<string> & new_stack,
                    Queue<Stack<string>> & word_queue, Stack<string> & finished_stack) {
    // Check if the new words is the solution
    if (new_word == word2) {
        new_stack.add(new_word);
        finished_stack = new_stack;
        return;
    }
    // Check if we've already used this word
    if (used.contains(new_word)) {
        return;
    }
    // Check if it's a valid word, if so add it
    if (dictionary.contains(new_word)) {
        used.add(new_word);
        new_stack.add(new_word);
        word_queue.enqueue(new_stack);
    }
}

// Check all possible neighbors for a string, return new string
// word1: first word in the word ladder
// word2: goal word in the word ladder
// dictionary: user provided dictionary
// current_stack: the stack currently being tested
// word_queue: queue of all stacks of potential word ladders
// used: all words that have been previously used in the word ladders
// finished_stack: an empty stack.  If the goal word is found this will be populated
void check_neighbors(string word1, string word2, Lexicon & dictionary, Stack<string> current_stack,
                     Queue<Stack<string>> & word_queue, Lexicon & used, Stack<string> & finished_stack) {
    // Define all letters in alphabet to try
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    // Loop through each letter in the word, then try each letter in the alphabet
    for (int i=0; i < word1.length(); i++) {
        for(int a=0; a<alphabet.length(); a++) {
            Stack<string> new_stack = current_stack;
            // If the current letter equals the new letter, skip it
            if (word1[i] == alphabet[a]) {
                continue;
            }
            string new_word = word1;
            new_word[i] = alphabet[a];
            check_new_word(new_word, word2, dictionary, used, new_stack, word_queue, finished_stack);
            // finished_stack passed by reference.  It is is not empty a solution was found
            if (!finished_stack.isEmpty()) {
                break;
            }
        }
    }
}

// Build word ladders
// word1: first word in the word ladder
// word2: goal word in the word ladder
// dictionary: user provided dictionary
// best_path: an empty stack.  If the goal word is found this will be populated
void word_ladder(string & word1, string & word2, Lexicon & dictionary, Stack<string> & best_path) {
    // Setup
    Lexicon used;
    Queue<Stack<string>> word_queue;
    Stack<string> initial_stack;
    initial_stack.push(word1);
    word_queue.enqueue(initial_stack);
    used.add(word1);
    // As long as there is a stack in the queue, keep trying to find a solution
    while (!word_queue.isEmpty()) {
        Stack<string> current_stack = word_queue.dequeue();
        string last_word = current_stack.peek();
        if(last_word == word2) {
            continue;
        }
        check_neighbors(last_word, word2, dictionary, current_stack, word_queue, used, best_path);
        if(!best_path.isEmpty()) {
            break;
        }
    }
}

// Print stack
// word1: first word in the word ladder
// word2: goal word in the word ladder
// stack: stack to be printed
void print_stack(string word1, string word2, Stack<string> stack) {
    cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
    while (!stack.isEmpty()) {
        string s = stack.pop();
        cout << s << " ";
    }
    cout << "\n" << endl;
}

// Check the input words
// word1: first word in the word ladder
// word2: goal word in the word ladder
// dictionary: user provided dictionary
bool check_words(string & word1, string & word2, Lexicon & dictionary) {
    word1 = toLowerCase(word1);
    word2 = toLowerCase(word2);
    // Check that the words aren't equal
    if (word1 == word2) {
        cout << "The two words must be different.\n" << endl;
        return false;
    }
    // Check that the dictionary contains the words
    if (!dictionary.contains(word1) || !dictionary.contains(word2)) {
        cout << "The two words must be found in the dictionary.\n" << endl;
        return false;
    }
    // Check that they are the same length
    if (word1.length() != word2.length()) {
        cout << "The two words must be the same length.\n" << endl;
        return false;
    }
    return true;
}

// Execute the word ladder game.  Start by getting words to try then finding the best path.
// dictionary: user provided dictionary
bool play_word_ladder(Lexicon & dictionary) {
    string word1 = get_words(1);
    if (word1.length() == 0) {
        return false;
    }
    string word2 = get_words(2);
    if (word2.length() == 0) {
        return false;
    }
    if (!check_words(word1, word2, dictionary)) {
        return true;
    }
    Stack<string> best_path;
    word_ladder(word1, word2, dictionary, best_path);
    if (!best_path.isEmpty()) {
        print_stack(word1, word2, best_path);
    } else {
        cout << "No word ladder found from " << word2 << " back to " << word1 << ".\n" << endl;
    }
    return true;
}


