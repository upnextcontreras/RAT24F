#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <fstream>

using namespace std;

// Token types
enum TokenType {
    KEYWORD, IDENTIFIER, INTEGER, REAL, OPERATOR, SEPARATOR, UNKNOWN
};

// Token structure
struct Token {
    TokenType type;
    string value;
};

// List of Rat24F keywords
vector<string> keywords = {"function", "integer", "real", "boolean", "if", "else", "fi", "while", "return", "get", "put", "false", "true"};

// Regular expressions for matching tokens
regex identifierRegex("[a-zA-Z]+[a-zA-Z0-9]*");
regex integerRegex("[0-9]+");
regex realRegex("[0-9]+\\.[0-9]+");
regex operatorRegex("[=><!\\+-/*]");
regex separatorRegex("[(),{};@]");

// Define character types
enum CharType {
    ALPHA, DIGIT, OPERATOR_CHAR, SEPARATOR_CHAR, SPACE, OTHER
};

// Function to classify characters
CharType getCharType(char ch) {
    if (isalpha(ch)) return ALPHA;
    if (isdigit(ch)) return DIGIT;
    if (ch == ' ' || ch == '\t' || ch == '\n') return SPACE;
    if (string("+-=*/><!").find(ch) != string::npos) return OPERATOR_CHAR;
    if (string(",(){};@").find(ch) != string::npos) return SEPARATOR_CHAR;
    return OTHER;
}

// Transition table for character type and current token type
map<TokenType, map<CharType, TokenType>> transitionTable = {
    {IDENTIFIER, {
        {ALPHA, IDENTIFIER}, {DIGIT, IDENTIFIER}, {OPERATOR_CHAR, OPERATOR}, {SEPARATOR_CHAR, SEPARATOR}, {SPACE, IDENTIFIER}, {OTHER, UNKNOWN}
    }},
    {INTEGER, {
        {ALPHA, UNKNOWN}, {DIGIT, INTEGER}, {OPERATOR_CHAR, OPERATOR}, {SEPARATOR_CHAR, SEPARATOR}, {SPACE, INTEGER}, {OTHER, UNKNOWN}
    }},
    {REAL, {
        {ALPHA, UNKNOWN}, {DIGIT, REAL}, {OPERATOR_CHAR, OPERATOR}, {SEPARATOR_CHAR, SEPARATOR}, {SPACE, REAL}, {OTHER, UNKNOWN}
    }},
    {OPERATOR, {
        {ALPHA, IDENTIFIER}, {DIGIT, INTEGER}, {OPERATOR_CHAR, OPERATOR}, {SEPARATOR_CHAR, SEPARATOR}, {SPACE, OPERATOR}, {OTHER, UNKNOWN}
    }},
    {SEPARATOR, {
        {ALPHA, IDENTIFIER}, {DIGIT, INTEGER}, {OPERATOR_CHAR, OPERATOR}, {SEPARATOR_CHAR, SEPARATOR}, {SPACE, SEPARATOR}, {OTHER, UNKNOWN}
    }},
    {UNKNOWN, {
        {ALPHA, UNKNOWN}, {DIGIT, UNKNOWN}, {OPERATOR_CHAR, UNKNOWN}, {SEPARATOR_CHAR, UNKNOWN}, {SPACE, UNKNOWN}, {OTHER, UNKNOWN}
    }}
};

// Function to check if a string is a keyword
bool isKeyword(const string& word) {
    return find(keywords.begin(), keywords.end(), word) != keywords.end();
}

// Function to convert TokenType enum to string
string tokenTypeToString(TokenType type) {
    switch (type) {
        case KEYWORD: return "KEYWORD";
        case IDENTIFIER: return "IDENTIFIER";
        case INTEGER: return "INTEGER";
        case REAL: return "REAL";
        case OPERATOR: return "OPERATOR";
        case SEPARATOR: return "SEPARATOR";
        default: return "UNKNOWN";
    }
}

// Lexical Analyzer: Tokenize the input program
vector<Token> lexicalAnalyzer(const string& input) {
    vector<Token> tokens;
    string token;
    TokenType currentState = UNKNOWN;

    for (char ch : input) {
        CharType charType = getCharType(ch);

        if (isspace(ch)) {
            // Handle token at the end of whitespace
            if (!token.empty()) {
                if (isKeyword(token)) {
                    tokens.push_back({KEYWORD, token});
                } else if (regex_match(token, identifierRegex)) {
                    tokens.push_back({IDENTIFIER, token});
                } else if (regex_match(token, integerRegex)) {
                    tokens.push_back({INTEGER, token});
                } else if (regex_match(token, realRegex)) {
                    tokens.push_back({REAL, token});
                } else {
                    tokens.push_back({UNKNOWN, token});
                }
                token.clear();
                currentState = UNKNOWN; // Reset state
            }
        } else if (regex_match(string(1, ch), operatorRegex)) {
            // Handle operator
            if (!token.empty()) {
                // Classify previous token
                if (isKeyword(token)) {
                    tokens.push_back({KEYWORD, token});
                } else if (regex_match(token, identifierRegex)) {
                    tokens.push_back({IDENTIFIER, token});
                } else if (regex_match(token, integerRegex)) {
                    tokens.push_back({INTEGER, token});
                } else if (regex_match(token, realRegex)) {
                    tokens.push_back({REAL, token});
                } else {
                    tokens.push_back({UNKNOWN, token});
                }
                token.clear();
            }
            tokens.push_back({OPERATOR, string(1, ch)}); // Add operator as a token
            currentState = UNKNOWN; // Reset state
        } else if (regex_match(string(1, ch), separatorRegex)) {
            // Handle separator
            if (!token.empty()) {
                // Classify previous token
                if (isKeyword(token)) {
                    tokens.push_back({KEYWORD, token});
                } else if (regex_match(token, identifierRegex)) {
                    tokens.push_back({IDENTIFIER, token});
                } else if (regex_match(token, integerRegex)) {
                    tokens.push_back({INTEGER, token});
                } else if (regex_match(token, realRegex)) {
                    tokens.push_back({REAL, token});
                } else {
                    tokens.push_back({UNKNOWN, token});
                }
                token.clear();
            }
            // Check for multi-character operator (e.g., !=, >=, <=)
            if (i + 1 < input.length()) {
                string twoCharOp = string(1, ch) + input[i + 1];
                if (twoCharOp == "!=" || twoCharOp == ">=" || twoCharOp == "<=") {
                    tokens.push_back({OPERATOR, twoCharOp}); // Add multi-character operator
                    i++;  // Skip next character as it's part of the operator
                    continue;
                }
            }
            tokens.push_back({SEPARATOR, string(1, ch)}); // Add separator as a token
            currentState = UNKNOWN; // Reset state
        } else {
            // Continue building the token
            token += ch;
        }
    }

    // Final token check at the end of input
    if (!token.empty()) {
        if (isKeyword(token)) {
            tokens.push_back({KEYWORD, token});
        } else if (regex_match(token, identifierRegex)) {
            tokens.push_back({IDENTIFIER, token});
        } else if (regex_match(token, integerRegex)) {
            tokens.push_back({INTEGER, token});
        } else if (regex_match(token, realRegex)) {
            tokens.push_back({REAL, token});
        } else {
            tokens.push_back({UNKNOWN, token});
        }
    }

    return tokens;
}

// Syntax Analyzer: A simple parsing method (this can be extended as per the BNF rules)
bool syntaxAnalyzer(const vector<Token>& tokens) {
    // Print headers
    cout << left << setw(15) << "Token" << "Lexeme" << endl;
    cout << "-------------------------------" << endl;
    
    // Print each token and lexeme with alignment
    for (const Token& token : tokens) {
        cout << left << setw(15) << tokenTypeToString(token.type) << token.value << endl;
    }
    
    return true; // Assume it's always valid for now.
}

// Function to process input from a file
void processInputFromFile(const string &filename) {
    ifstream infile(filename);  // Open the file
    if (!infile) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string line, input;
    while (getline(infile, line)) {
        input += line + "\n";  // Append each line to input
    }
    
    infile.close();  // Close the file after processing

    // Perform lexical analysis
    vector<Token> tokens = lexicalAnalyzer(input);
    
    // Perform syntax analysis
    syntaxAnalyzer(tokens);
}

// Main function
int main() {
    string filename = "t1.txt";  // The .txt file containing test cases
    processInputFromFile(filename);  // Process the input from the file
    
    return 0;
}
