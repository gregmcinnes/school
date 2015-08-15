/* Greg McInnes
   CS106B
   Homework 4
   July 24, 2015
   Boggle class file.  All functions associated with fundamental Boggle game play
   are stored within the class.
*/

#include "Boggle.h"
#include "grid.h"
#include "strlib.h"
#include "shuffle.h"
#include "random.h"
#include "vector.h"
#include "set.h"
#include "bogglegui.h"
using namespace BoggleGUI;

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

// Declarations
Grid<char> randomBoard();
Grid<char> playerBoard(string boardText);

// Boggle main
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    if (boardText.length() == 0) {
        board = randomBoard();
    } else {
        board = playerBoard(boardText);
    }
    dict = dictionary;
}

// generate random board
Grid<char> randomBoard() {
    Vector<string> cubes;
    for(int i=0; i<16; i++) { // change to size of array
        cubes.add(CUBES[i]);
    }
    shuffle(cubes);
    Grid<char> board(4,4);
    for(int r=0; r<4; r++) {
        for(int c=0; c<4; c++) {
            string cube = cubes[cubes.size()-1];
            int rand = randomInteger(0,cube.length()-1);
            board.set(r,c,cube[rand]);
            cubes.remove(cubes.size()-1);
        }
    }
    return board;
}

// player defined board
Grid<char> playerBoard(string boardText) {
    if (!boardText.length() == 16) {
        throw "Invalid board!";
    }
    boardText = toUpperCase(boardText);
    Grid<char> board(4,4);
    for (int r=0; r<4; r++) {
        for (int c=0; c<4; c++) {
            board.set(r,c,boardText[0]);
            boardText.erase(0,1);
        }
    }
    return board;
}

// Get a letter from the board
char Boggle::getLetter(int row, int col) {
    if (!board.inBounds(row, col)) {
        throw "Index out of bounds";
    }
    return board.get(row, col);
}

// Check if a word is valid
bool Boggle::checkWord(string word) {
    BoggleGUI::clearHighlighting();
    if (humanWords.contains(word) || word.length() < 4 || !dict.contains(word)) {
        return false;
    }
    return true;
}

// Search for a human word on the board
bool Boggle::humanWordSearch(string word) {
    BoggleGUI::clearHighlighting();
    for (int r=0; r<4; r++) {
        for (int c=0; c<4; c++) {
            if(board.get(r,c) == word[0]) {
                Set<Vector<int>> checked;
                BoggleGUI::setHighlighted(r, c, true);
                bool found = humanWordSearchHelper(word, r, c, checked);
                if (found == true) {
                    return true;
                }
                BoggleGUI::setHighlighted(r, c, false);
            }
        }
    }
    BoggleGUI::clearHighlighting();
    return false;
}

// Recursive backtracking helper on the board
bool Boggle::humanWordSearchHelper(string word, int r, int c, Set<Vector<int>> & checked) {
    if(word.length() == 0) {
        return true;
    } else {
        Vector<int> v;
        v.add(r);
        v.add(c);
        if (!board.inBounds(r,c) || checked.contains(v)) {
            return false;
        } else if (board.get(r,c) == word[0]) {
            BoggleGUI::setAnimationDelay(100);
            BoggleGUI::setHighlighted(r, c, true);
            for (int rnew=r-1; rnew<r+2; rnew++) {
                for (int cnew=c-1; cnew<c+2; cnew++) {
                    checked.add(v);
                    bool found = humanWordSearchHelper(word.substr(1), rnew, cnew, checked);
                    if (found == true) {
                        return true;
                    }
                    checked.remove(v);
                }
            }
            BoggleGUI::setHighlighted(r, c, false);

        }
    }
    return false;
}

// Return the human score
int Boggle::getScoreHuman() {
    int score = 0;
    if (humanWords.size() == 0) {
        return score;
    }
    for (string w : humanWords) {
        score += w.length()-3;
    }
    return score;
}

// Return the words that the human has found
Lexicon Boggle::getHumanWords() {
    return humanWords;
}

// Search the board for the rest of the words
Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    for (int r=0; r<4; r++) {
        for (int c=0; c<4; c++) {
            string word;
            Set<Vector<int>> checked;
            computerWordSearchHelper(r, c, word, result, checked);
        }
    }
    for (string r:result) {
        computerWords.add(r);
    }
    return result;
}

// Recursive backtracking to find all the words
void Boggle::computerWordSearchHelper(int r, int c, string word, Set<string> & result, Set<Vector<int>> & checked) {
    Vector<int> v;
    v.add(r);
    v.add(c);
    if (dict.contains(word) && word.length() >= 4 && !humanWords.contains(word)) {
        result.add(toUpperCase(word));
    }
    if (!dict.containsPrefix(word) || !board.inBounds(r, c) || checked.contains(v)) {
        return;
    }
    for (int rnew=r-1; rnew<r+2; rnew++) {
        for (int cnew=c-1; cnew<c+2; cnew++) {
            checked.add(v);
            computerWordSearchHelper(rnew, cnew, word+board.get(r, c), result, checked);
            checked.remove(v);
        }
    }

}

// Return the computer score
int Boggle::getScoreComputer() {
    int score = 0;
    for (string w:computerWords) {
        score += w.length()-3;
    }
    return score;
}

// Return the computer words
Lexicon Boggle::getComputerWords() {
    return computerWords;
}

// Add a human word to the found list
void Boggle::addHumanWord(string word) {
    humanWords.add(toUpperCase(word));
}

// Create a printable string for the boggle board
string Boggle::printable() {
    string printable = board.toString2D();
    printable = stringReplace(printable, "{", "");
    printable = stringReplace(printable, "}", "");
    printable = stringReplace(printable, "\"", "");
    printable = stringReplace(printable, ",", "");
    printable = stringReplace(printable, " ", "");
    return printable;
}

// Ostream operator
ostream& operator<<(ostream& out, Boggle& boggle) {
    out << boggle.printable() << endl;
    return out;
}
