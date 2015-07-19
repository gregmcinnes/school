/* Greg McInnes
   CS106B
   Homework 3
   July 14, 2015

   This file contains functions to solve the recursion problems in homework 3 of CS106B.  These
   functions are called from a main file that includes tests to check correctness.  See the
   assignment page on the CS106B website for more information.
*/

#include "recursionproblems.h"
#include <cmath>
#include <iostream>
#include "hashmap.h"
#include "map.h"
#include "random.h"
#include "vector.h"
#include "math.h"
#include "strlib.h"
using namespace std;

// Helper function prototype declarations
void checkIsInteger(int value);
void recurseSierpinskiTriangle(GWindow& gw, double x, double y, int size, int order);
void recurseFloodFill(GBufferedImage& image, int x, int y, int & color, int & oldColor, int & count);
Map<string, Vector<string>> grammarMap(istream& input);
string recurseGrammarGenerate(Map<string, Vector<string>> & ruleMap, string symbol);

// Optional - didn't do it
int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

// Helper function that checks if a string -> integer conversion was valid
void checkIsInteger(int value) {
    if (value > 9 || value < 0) {
        throw "Invalid integer format";
    }
}

// Recusively convert a string to an integer
int convertStringToInteger(string exp) {
    if(exp.length() < 1) {
        return 0;
    } else if(exp.length() == 1) {
        int integer = exp[0] - '0';
        checkIsInteger(integer);
        return integer;
    } else {
        int sign = 1;
        if (exp[0]=='-') {
            sign = -1;
            exp = exp.substr(1, exp.length());
        }
        int rest = convertStringToInteger(exp.substr(0, exp.length()-1));
        int current = exp[exp.length()-1] - '0';
        checkIsInteger(current);
        int result = (current + (10*rest));
        return result * sign;
    }
    return 0;
}

// Recusively check if a string is balanced. A string is balanced if each open brace has a corresponding close brace.
bool isBalanced(string exp) {
    if (exp.length() == 0) {
        return true;
    } else if (stringContains(exp, "[]")) {
        return isBalanced(exp.replace(exp.find("[]"), 2, ""));
    } else if (stringContains(exp, "()")) {
        return isBalanced(exp.replace(exp.find("()"), 2, ""));
    } else if (stringContains(exp, "{}")) {
        return isBalanced(exp.replace(exp.find("{}"), 2, ""));
    } else if (stringContains(exp, "<>")) {
        return isBalanced(exp.replace(exp.find("<>"), 2, ""));
    } else {
        return false;
    }
}

// Recursively calculate the weight on the knees of a person in a human pyramid.
// Takes in a vector of vectors of doubles of each persons weight in the pyramid.
double weightOnKnees(int row, int col, Vector<Vector<double> >& weights) {
    if (row == 0 && col == 0) {
        return weights[0][0];
    } else if (row < 0 || col < 0 || row > weights.size()-1 || col > weights[row].size()-1) {
        return 0.0;
    } else {
        return (weightOnKnees(row-1, col, weights)/2) + (weightOnKnees(row-1, col-1, weights)/2) + weights[row][col];
    }
    return 0.0;
}

// Helper function that recursively draws the inner triangles of a Sierpinski triangle
void recurseSierpinskiTriangle(GWindow& gw, double x, double y, int size, int order) {
    if (order == 0) {
        return;
    } else if (order < 0) {
        throw "Invalid order";
    }
    double leftX = x + (0.5 * size) / 2;
    double rightX = leftX + size / 2;
    double bottomY = y + (sqrt((size * size) - (0.25 * size * size))) / 2;
    double topX = x + size / 2;
    gw.drawLine(leftX, bottomY, topX, y);
    gw.drawLine(topX, y, rightX, bottomY);
    gw.drawLine(rightX, bottomY, leftX, bottomY);
    recurseSierpinskiTriangle(gw, x, y, size/2, order-1);
    recurseSierpinskiTriangle(gw, x+size/2, y, size/2, order-1);
    recurseSierpinskiTriangle(gw, leftX, bottomY, size/2, order-1);
}

// Main function that draws a Sierpinki triangle.  This portion draws the outside of the triangle then calls
// the helper function to draw the insides.
void drawSierpinskiTriangle(GWindow& gw, double x, double y, int size, int order) {
    if (order <= 0) {
        throw "Invalid order!";
    }
    double bottomX = x + (0.5 * size);
    double bottomY = y + (sqrt((size * size) - (0.25 * size * size)));
    double rightX = x + size;
    gw.drawLine(x, y, rightX, y);
    gw.drawLine(x, y, bottomX, bottomY);
    gw.drawLine(bottomX, bottomY, rightX, y);
    recurseSierpinskiTriangle(gw, x, y, size, order-1);
}

// Helper function that fills an area of an image with a new color recursively.
void recurseFloodFill(GBufferedImage& image, int x, int y, int & color, int & oldColor, int & count) {
    if (!image.inBounds(x, y)) {
        return;
    } else if (image.getRGB(x, y) != oldColor) {
        return;
    }
    image.setRGB(x, y, color);
    count++;
    recurseFloodFill(image, x+1, y, color, oldColor, count);
    recurseFloodFill(image, x+1, y+1, color, oldColor, count);
    recurseFloodFill(image, x+1, y-1, color, oldColor, count);
    recurseFloodFill(image, x, y+1, color, oldColor, count);
    recurseFloodFill(image, x, y-1, color, oldColor, count);
    recurseFloodFill(image, x-1, y, color, oldColor, count);
    recurseFloodFill(image, x-1, y+1, color, oldColor, count);
    recurseFloodFill(image, x-1, y-1, color, oldColor, count);
}

// Main function that calls the recursive fill function.  Secondary function is required to get
// the number of pixels colored.
int floodFill(GBufferedImage& image, int x, int y, int color) {
    if (!image.inBounds(x, y)) {
        throw "Out of bounds!";
    }
    int currentColor = image.getRGB(x, y);
    int count = 0;
    recurseFloodFill(image, x, y, color, currentColor, count);
    return count;
}

// Create a map of the grammar rules from a file
Map<string, Vector<string>> grammarMap(istream& input) {
    Map<string, Vector<string>> ruleMap;
    string line;
    while (getline(input, line)) {
        Vector<string> splitLine = stringSplit(line, "::=");
        string nt = splitLine[0];
        if (ruleMap.containsKey(nt)) {
            throw "Duplicate non-terminals";
        }
        Vector<string> rules = stringSplit(splitLine[1], "|");
        ruleMap[nt] = rules;
    }
    return ruleMap;
}

// Generate a random sentence from a grammar map given a provided non-terminal symbol.
string recurseGrammarGenerate(Map<string, Vector<string>> & ruleMap, string symbol) {
    if (!ruleMap.containsKey(symbol)) {
        return trim(symbol);
    } else {
        int rand = randomInteger(0, ruleMap[symbol].size()-1);
        string rule = ruleMap[symbol][rand];

        Vector<string> symbols = stringSplit(rule, " ");
        Vector<string> resultVector;
        for (string s : symbols) {
            resultVector.add(recurseGrammarGenerate(ruleMap, s));
        }
        string result;
        for (string r : resultVector) {
            result += r + " ";
        }
        return trim(result);
    }
}

// Main function used to generate grammatical structures.  Loops over
// the recursive function a specified number of times to generate 'times'
// terminal phrases.
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Map<string, Vector<string>> ruleMap = grammarMap(input);
    Vector<string> v;
    for (int i = 0; i < times; i++) {
        v.add(recurseGrammarGenerate(ruleMap, symbol));
    }
    return v;
}
