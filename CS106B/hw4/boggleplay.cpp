/* Greg McInnes
   CS106B
   Homework 4
   July 24, 2015
   This is the code that plays Boggle using functions from the Boggle class.
   A GUI and a Boggle object is created for each game.
*/

#include <iostream>
#include "lexicon.h"
#include "Boggle.h"
#include "simpio.h"
#include "console.h"
#include "bogglegui.h"
using namespace std;

// Declarations
Lexicon getDictionary();
bool randomChoice();
void messageAll(string text);
string userBoard();
string getEntry();
bool validWord(string word, Boggle & game);
void humanTurn(Boggle & game);
void computerTurn(Boggle & game);
void setupGUI(Boggle game);
void playOneGame(Lexicon& dictionary);

// Main playing function
void playOneGame(Lexicon& dictionary) {
    setConsoleClearEnabled(true);
    dictionary = getDictionary();
    string boardText;
    if (!randomChoice()) {
        boardText = userBoard();
    }
    Boggle game(dictionary, boardText);
    setupGUI(game);
    humanTurn(game);
    computerTurn(game);
}

// Set up dictionary from file
Lexicon getDictionary() {
    string file = "dictionary.txt";
    Lexicon dictionary;
    dictionary.addWordsFromFile(file);
    return dictionary;
}

// Choose whether a random board is desired
bool randomChoice() {
    bool response = getYesOrNo("Do you want to generate a random board? ");
    return response;
}

// Print messages to console and gui
void messageAll(string text) {
    cout << text << endl;
    BoggleGUI::setStatusMessage(text);
}

// Request a user board and check it
string userBoard() {
    bool valid = false;
    string line;
    while (!valid) {
        line = getLine("Type the 16 letters to appear on the board: ");
        valid = true;
        if (line.length() != 16) {
            valid = false;
        }
        for (int i=0; i<line.length(); i++) {
            if (!isalpha(line[i])) {
                valid = false;
            }
        }
        if (valid == false) {
            messageAll("That is not a valid 16-letter board string. Try again.");
        }
    }
    return line;
}

// Get a word from the user
string getEntry() {
    string word = getLine("Type a word (or Enter to stop): ");
    return toUpperCase(word);
}

// Check if a word is valid
bool validWord(string word, Boggle & game) {
    if (!game.checkWord(word)) {
        messageAll("You must enter an unfound 4+ letter word from the dictionary.");
        return false;
    } else if (!game.humanWordSearch(word)) {
        messageAll("That word can't be formed on this board.");
        return false;
    }
    return true;
}

// Main human turn handling
void humanTurn(Boggle & game) {
    clearConsole();
    messageAll("It's your turn!");
    bool finish = false;
    while (!finish) {
        cout << game << endl;
        cout << "Your words: (" << game.getHumanWords().size() << "): " << game.getHumanWords() << endl;
        cout << "Your score: " << game.getScoreHuman() << endl;
        BoggleGUI::setScore(game.getScoreHuman(), BoggleGUI::HUMAN);
        string word = getEntry();
        if (word.length() == 0) {
            BoggleGUI::clearHighlighting();
            return;
        }
        clearConsole();
        if (validWord(word, game)) {
            game.addHumanWord(word);
            BoggleGUI::setStatusMessage("You found a valid word \"" + word + "\"");
            BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
        }
    }
}

// Computer turn handling
void computerTurn(Boggle & game) {
    messageAll("It's my turn!");
    game.computerWordSearch();
    if (game.getComputerWords().size() > 0) {
        for (string word : game.getComputerWords()) {
            BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
        }
    }
    cout << "My words: (" << game.getComputerWords().size() << "): " << game.getComputerWords() << endl;
    cout << "My score: " << game.getScoreComputer() << endl;
    BoggleGUI::setScore(game.getScoreComputer(), BoggleGUI::COMPUTER);
    if (game.getScoreComputer() > game.getScoreHuman()) {
        messageAll("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    } else {
        messageAll("WOW, you defeated me! Congratulations!");
    }
}

// Set up GUI
void setupGUI(Boggle game) {
    if (BoggleGUI::isInitialized()) {
        BoggleGUI::reset();
    }
    BoggleGUI::initialize(4,4);
    BoggleGUI::labelAllCubes(game.printable());
    BoggleGUI::playSound("dice-rattle.wav");
}

