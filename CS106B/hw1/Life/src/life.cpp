// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header!

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
#include "vector.h"
using namespace std;

void welcome_message() {
    // Print welcome message
    cout << "Welcome to the CS 106B Game of Life,\n"
            "a simulation of the lifecycle of a bacteria colony.\n"
            "Cells (X) live and die by the following rules:\n"
            "- A cell with 1 or fewer neighbors dies.\n"
            "- Locations with 2 neighbors remain stable.\n"
            "- Locations with 3 neighbors will create life.\n"
            "- A cell with 4 or more neighbors dies." << endl;
}

void check_colony(string colony) {
    if (colony != "-" && colony != "X") {
        cout << "INVALID COLONY TYPE " + colony << endl;
        exit (EXIT_FAILURE);
    }
}

Grid<string> read_colony_file(string filename){
    int line_number = 0;
    int rows;
    int cols;

    Grid<string> colony;

    std::ifstream infile;
    infile.open(filename.c_str());
    std::string line;
    while (std::getline(infile, line)) {

        if (line_number == 0) {
            rows = stringToInteger(line);
            line_number++;
            continue;
        }
        else if (line_number == 1) {
            cols = stringToInteger(line);
            line_number++;
            colony.resize(rows, cols);
            continue;
        }

        if (line_number > rows+2) {
            break;
        }

        int row_number = line_number - 2;

        for (unsigned int i = 0; i < line.length(); i++) {
            check_colony(charToString(line[i]));
            colony.set(row_number, i, charToString(line[i]));
        }

        line_number++;
    }
    return colony;
}

void print_colony(Grid<string> colony) {
    string printable = colony.toString2D();
    printable = stringReplace(printable, "{", "");
    printable = stringReplace(printable, "}", "");
    printable = stringReplace(printable, "\"", "");
    printable = stringReplace(printable, ",", "");
    printable = stringReplace(printable, " ", "");
    cout << printable << endl;
}

string get_move() {
    cout << "a)nimate, t)ick, q)uit? ";
    string move;
    cin >> move;
    return move;
}

Grid<string> tick(Grid<string> colony) {
    // initialize new colony
    Grid<string> new_colony(colony.nRows, colony.nCols);

    // loop through existing colony to see what needs to be changed
    for (int r = 0; r < colony.numRows(); r++) {

        for (int c = 0; c < colony.numCols(); c++){
            if (!colony.inBounds(r,c)) {
                continue;
            }
            int neighbors = 0;
            for (int x = r-1; x <= r+1; x++) {
                for (int y = c-1; y <= c+1; y++) {
                    if (x == r && y == c) {
                        continue;
                    }
                    if (!colony.inBounds(x,y)) {
                        continue;
                    }
                    if (colony.get(x,y) == "X") {
                        neighbors++;
                    }
                }
            }
            if (neighbors <= 1) {
                new_colony.set(r,c,"-");
            }
            else if (neighbors == 2) {
                new_colony.set(r,c,colony.get(r,c));
            }
            else if (neighbors == 3) {
                new_colony.set(r,c,"X");
            }
            else if (neighbors >= 4) {
                new_colony.set(r,c,"-");
            }
        }
    }
    return new_colony;
}

Grid<string> animate(Grid<string> colony, int count) {
    for (int i = 0; i < count; i++) {
        if (count > 1) {
            pause(50);
            clearConsole();
        }
        Grid<string> new_colony = tick(colony);
        print_colony(new_colony);
        colony = new_colony;
    }
    return colony;
}

void move(Grid<string> colony) {
    string move;
    while (!equalsIgnoreCase(move,"q")) {
        Grid<string> new_colony = colony;
        move = get_move();
        if (equalsIgnoreCase(move, "t")){
            new_colony = animate(colony, 1);
        }
        else if (equalsIgnoreCase(move, "a")) {
            string count;
            cout << "How many frames? ";
            cin >> count;
            if (!stringIsInteger(count)){
                cout << "Illegal integer format. Try again." << endl;
                continue;
            }
            new_colony = animate(colony, stringToInteger(count));
        }

        else if (equalsIgnoreCase(move, "q")) {
            break;
        }
        else {
            cout << "Invalid entry." << endl;
        }
        colony = new_colony;
    }
}

int main() {
    // Uncomment this line of code to enable console clearing
    setConsoleClearEnabled(true);


    welcome_message();
    ifstream input;
    string filename = promptUserForFile(input, "Grid input file name?", "Unable to open that file. Try again.");
    //string filename = "glider-gun.txt"; //todo replace this
    Grid<string> colony = read_colony_file(filename);
    print_colony(colony);

    move(colony);
    cout << "Have a nice Life!" << endl;
    return 0;
}


