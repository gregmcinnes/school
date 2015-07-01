// Gregory McInnes
// June 30, 2015
// CS106B

// Load packages
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"

// Set namespace
using namespace std;

void welcome_message() {
    // Print welcome message
    cout << "Welcome to the CS 106B Game of Life,\n"
            "a simulation of the lifecycle of a bacteria colony.\n"
            "Cells (X) live and die by the following rules:\n"
            "- A cell with 1 or fewer neighbors dies.\n"
            "- Locations with 2 neighbors remain stable.\n"
            "- Locations with 3 neighbors will create life.\n"
            "- A cell with 4 or more neighbors dies.\n" << endl;
}

void print_colony(Grid<string>& colony) {
    // Print colony. First convert grid to a string, then remove special characters, finally print.
    // Input: a Grid containing a file
    string printable = colony.toString2D();
    printable = stringReplace(printable, "{", "");
    printable = stringReplace(printable, "}", "");
    printable = stringReplace(printable, "\"", "");
    printable = stringReplace(printable, ",", "");
    printable = stringReplace(printable, " ", "");
    cout << printable << endl;
}

Grid<string> read_colony_file(string& filename){
    // Read in colony file.
    // Input: the name of the file to be read
    // Returns a Grid containing the colony from the input file
    // Declare some variables
    int line_number = 0;
    int rows;
    int cols;
    Grid<string> colony;
    // Read the file
    ifstream infile;
    infile.open(filename.c_str());
    string line;
    while (getline(infile, line)) {
        // First two rows of colony file define row and column numbers
        if (line_number == 0) {
            rows = stringToInteger(line);
            line_number++;
            continue;
        }
        else if (line_number == 1) {
            cols = stringToInteger(line);
            line_number++;
            // Once the rows and columns are set define the grid size
            colony.resize(rows, cols);
            continue;
        }
        // Exit file if the end of the colony has been reached
        if (line_number > rows+1) {
            break;
        }
        int row_number = line_number - 2;
        // Loop through each character in the colony row and add it to the grid.
        for (unsigned int i = 0; i < line.length(); i++) {
            colony.set(row_number, i, charToString(line[i]));
        }
        line_number++;
    }
    infile.close();
    print_colony(colony);
    return colony;
}

string get_move() {
    // Get user input for the next move
    // Returns the user input
    cout << "a)nimate, t)ick, q)uit? ";
    string move;
    cin >> move;
    return move;
}

void tick(Grid<string>& colony) {
    // Iterate one generation of a colony
    // Input: Colony Grid
    // initialize new colony to be edited within this function
    Grid<string> new_colony(colony.nRows, colony.nCols);
    // loop through existing colony to see what needs to be changed
    for (int r = 0; r < colony.numRows(); r++) {
        for (int c = 0; c < colony.numCols(); c++){
            if (!colony.inBounds(r,c)) {
                continue;
            }
            // Look at the surrounding cells and count the number of colonies
            int neighbors = 0;
            for (int x = r-1; x <= r+1; x++) {
                for (int y = c-1; y <= c+1; y++) {
                    // Skip self
                    if (x == r && y == c) {
                        continue;
                    }
                    // Skip out of bounds colonies
                    if (!colony.inBounds(x,y)) {
                        continue;
                    }
                    // If a colony is found add one
                    if (colony.get(x,y) == "X") {
                        neighbors++;
                    }
                }
            }
            // Positions with 1 of fewer neighbors die
            if (neighbors <= 1) {
                new_colony.set(r,c,"-");
            }
            // Positions with 2 neighbors persist
            else if (neighbors == 2) {
                new_colony.set(r,c,colony.get(r,c));
            }
            // Positions with 3 neighbors will have a colony
            else if (neighbors == 3) {
                new_colony.set(r,c,"X");
            }
            // Positions with 4 or more neighbors die
            else if (neighbors >= 4) {
                new_colony.set(r,c,"-");
            }
        }
    }
    colony = new_colony;
}

void animate(Grid<string>& colony, int count) {
    // Execute a predefined number of ticks
    // Input: Colony Grid, number of ticks to advance
    for (int i = 0; i < count; i++) {
        // If the count is more than 1, we're in an animation and need to wait 50ms and clear the console
        if (count > 1) {
            pause(50);
            clearConsole();
        }
        tick(colony);
        print_colony(colony);
    }
}

void move(Grid<string>& colony) {
    // Determine what move the user wants and execute it
    // Input: Colony Grid
    string move;
    // As long as the user has not pressed 'q', keep going
    while (!equalsIgnoreCase(move,"q")) {
        move = get_move();
        // Tick 1
        if (equalsIgnoreCase(move, "t")){
            animate(colony, 1);
        }
        // Animate
        else if (equalsIgnoreCase(move, "a")) {
            string count;
            // Keep asking for a new entry until the user enters a valid integer
            while (!stringIsInteger(count)) {
                cout << "How many frames? ";
                cin >> count;
                if (!stringIsInteger(count)){
                    cout << "Illegal integer format. Try again." << endl;
                    continue;
                }
            }
            animate(colony, stringToInteger(count));
        }
        // Exit if 'q' has been entered
        else if (equalsIgnoreCase(move, "q")) {
            break;
        }
        else {
            cout << "Invalid choice; please try again." << endl;
        }
    }
}

string get_filename() {
    // Ask the user name for the filename
    // Returns the filename entered by the user
    ifstream input;
    string filename = promptUserForFile(input, "Grid input file name?", "Unable to open that file. Try again.");
    return filename;
}

int main() {
    // Main
    setConsoleClearEnabled(true);
    // Print the welcome message
    welcome_message();
    // Get the colony file from the user
    string filename = get_filename();
    // Read in the contents of the colony file
    Grid<string> colony = read_colony_file(filename);
    // Execute requested moves
    move(colony);
    // Exit
    cout << "Have a nice Life!" << endl;
    return 0;
}
