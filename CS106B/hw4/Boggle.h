/* Greg McInnes
   CS106B
   Homework 4
   July 24, 2015
   Boggle class header file.
*/

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
using namespace std;

class Boggle {
public:
    // Boggle board.  Provide a dictionary and optional board text to create board.
    // Returns a Boggle object.
    Boggle(Lexicon& dictionary, string boardText = "");
    // Get a specific letter from the board at r,c
    char getLetter(int row, int col);
    // Check if a word exists on the board and is valid
    bool checkWord(string word);
    // Add a word to the human set
    void addHumanWord(string word);
    // Search for a word on the board for the human
    bool humanWordSearch(string word);
    // Search for the rest of the words for the computer
    Set<string> computerWordSearch();
    // Get the humans word score
    int getScoreHuman();
    // Get the computer score
    int getScoreComputer();
    // Get a printable string of the board
    string printable();
    // Get a lexicon of the human words
    Lexicon getHumanWords();
    // Get a lexicon of the computer words
    Lexicon getComputerWords();
    // ostream operator
    friend ostream& operator<<(ostream& out, Boggle& boggle);

private:
    Grid<char> board;
    Lexicon dict;
    bool humanWordSearchHelper(string word, int r, int c, Set<Vector<int>> & checked);
    void computerWordSearchHelper(int r, int c, string word, Set<string> & result, Set<Vector<int>> & checked);
    Lexicon humanWords;
    Lexicon computerWords;
};

#endif // _boggle_h
