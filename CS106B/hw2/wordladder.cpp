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
#include "lexicon.h"
#include "stack.h"
#include "queue.h"
#include "strlib.h"
#include "simpio.h"

using namespace std;

void print_header() {
    cout << "Welcome to CS 106B Word Ladder.\n"
            "Please give me two English words, and I will change the\n"
            "first into the second by changing one letter at a time.\n" << endl;
}

// Have user define dictionary
string get_dicitionary() {
    ifstream input;
    string file = promptUserForFile(input, "Dictionary file name?", "Unable to open that file.  Try again.");
    //string file = "smalldict1.txt";  // TESTING
    cout << endl;
    return file;
}

// Check that word input is valid
// bool check_word(string word){}

// Have user enter words
string get_words(int word_number) {
    //string word;
    //cout << "Word #" << word_number << " (or Enter to quit): ";
    string word = getLine("Word #" + integerToString(word_number) + " (or Enter to quit): ");

    // todo check for valid input

    return word;
}

// Create dictionary lexicon
Lexicon fill_lexicon(string dictionary_file) {
    Lexicon dictionary;
    dictionary.addWordsFromFile(dictionary_file);
    return dictionary;
}

// Check new word
void check_new_word(string & new_word, string & word2, Lexicon & dictionary, Lexicon & used, Stack<string> & new_stack,
                    Queue<Stack<string>> & word_queue, Stack<string> & finished_stack) {
    if (new_word == word2) {
        new_stack.add(new_word);
        finished_stack = new_stack;
        return;
    }
    if (used.contains(new_word)) {
        return;
    }
    if (dictionary.contains(new_word)) {
        used.add(new_word);
        new_stack.add(new_word);
        word_queue.enqueue(new_stack);
    }
}

// Check all possible neighbors for a string, return new string
void check_neighbors(string word1,
                     string word2,
                     Lexicon & dictionary,
                     Stack<string> current_stack,
                     Queue<Stack<string>> & word_queue,
                     Lexicon & used,
                     Stack<string> & finished_stack) {

    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (int i=0; i<word1.length(); i++) {
        for(int a=0; a<alphabet.length(); a++) {
            Stack<string> new_stack = current_stack;
            if (word1[i] == alphabet[a]) {
                continue;
            }
            string new_word = word1;
            new_word[i] = alphabet[a];
            check_new_word(new_word, word2, dictionary, used, new_stack, word_queue, finished_stack);
        }
        if (!finished_stack.isEmpty()) {
            break;
        }
    }
}

// Build word ladders
void word_ladder(string & word1, string & word2, Lexicon & dictionary, Stack<string> & best_path) {
    Lexicon used;
    Queue<Stack<string>> word_queue;
    Stack<string> initial_stack;
    initial_stack.push(word1);
    word_queue.enqueue(initial_stack);
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
void print_stack(string word1, string word2, Stack<string> stack) {
    cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
    while (!stack.isEmpty()) {
        string s = stack.pop();
        cout << s << " ";
    }
    cout << "\n" << endl;
}

// Check the input words
bool check_words(string & word1, string & word2, Lexicon & dictionary) {
    word1 = toLowerCase(word1);
    word2 = toLowerCase(word2);

    if (word1 == word2) {
        cout << "The two words must be different.\n" << endl;
        return false;
    }

    if (!dictionary.contains(word1) || !dictionary.contains(word2)) {
        cout << "The two words must be found in the dictionary.\n" << endl;
        return false;
    }

    if (word1.length() != word2.length()) {
        cout << "The two words must be the same length.\n" << endl;
        return false;
    }
    return true;
}

//
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

int main() {
    print_header();
    string dictionary_file = get_dicitionary();
    Lexicon dictionary = fill_lexicon(dictionary_file);
    bool play = true;
    while (play) {
        play = play_word_ladder(dictionary);
    }
    cout << "Have a nice day." << endl;
    return 0;
}
