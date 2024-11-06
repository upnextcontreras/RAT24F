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
regex identifierRegex("^[a-zA-Z][a-zA-Z0-9]*$"); // Identifiers start with a letter, followed by alphanumeric characters
regex integerRegex("^(0|[1-9][0-9]*)$");         // Only integers without decimals, handling leading zeros as "0"
regex realRegex("^(0|[1-9][0-9]*)?\\.[0-9]+$");  // Real numbers with optional leading integer part
regex operatorRegex("(!=|>=|<=|==|[=><!\\+-/*])"); // Recognizes compound operators
regex separatorRegex("[(),{};@]");               // Separators

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

    for (size_t i = 0; i < input.length(); ++i) {
        char ch = input[i];
        CharType charType = getCharType(ch);

        // Handle compound operators (lookahead for next character)
        if (regex_match(string(1, ch), operatorRegex) && i + 1 < input.length()) {
            string potentialOp = string(1, ch) + input[i + 1];
            if (regex_match(potentialOp, operatorRegex)) {
                tokens.push_back({OPERATOR, potentialOp});
                ++i; // Skip the next character
                continue;
            }
        }

        // Check for whitespace to separate tokens
        if (isspace(ch)) {
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
                currentState = UNKNOWN;
            }
            continue;
        }

        // Handle separators
        if (regex_match(string(1, ch), separatorRegex)) {
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
            }
            tokens.push_back({SEPARATOR, string(1, ch)});
            continue;
        }

        // Handle potential real numbers (e.g., 234.567 or 0.001)
        if (isdigit(ch) || (ch == '.' && !token.empty() && isdigit(token.back()))) {
            token += ch;

            // Check ahead for a complete real number pattern
            size_t j = i + 1;
            while (j < input.length() && (isdigit(input[j]) || input[j] == '.')) {
                token += input[j];
                ++j;
            }
            i = j - 1;  // Update index to skip processed characters

            // Determine if the entire token matches a real or integer
            if (regex_match(token, realRegex)) {
                tokens.push_back({REAL, token});
            } else if (regex_match(token, integerRegex)) {
                tokens.push_back({INTEGER, token});
            } else {
                tokens.push_back({UNKNOWN, token});
            }
            token.clear();
            continue;
        }

        // Append character to token if it's part of an identifier or operator
        token += ch;
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
    cout << left << setw(15) << "Token" << "Lexeme" << endl;
    cout << "-------------------------------" << endl;

    for (const Token& token : tokens) {
        cout << left << setw(15) << tokenTypeToString(token.type) << token.value << endl;
    }
    return true;
}

// Function to process input from a file, ignoring comments
void processInputFromFile(const string &filename) {
    ifstream infile(filename);  // Open the file
    if (!infile) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string line, input;
    while (getline(infile, line)) {
        // Ignore lines starting with "[*"
        if (line.rfind("[*", 0) == 0) {
            continue;
        }
        input += line + "\n";  // Append each line to input if it doesn't start with "[*"
    }

    infile.close();  // Close the file after processing

    // Perform lexical analysis
    vector<Token> tokens = lexicalAnalyzer(input);

    // Perform syntax analysis
    syntaxAnalyzer(tokens);
}

// Main function
int main() {
    string filename = "t1.txt";
    processInputFromFile(filename);
    return 0;
}
