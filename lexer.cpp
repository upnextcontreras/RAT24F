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

// List of keywords
vector<string> keywords = {"function", "integer", "real", "boolean", "if", "else", "fi", "while", "return", "get", "put", "true", "false"};

// Updated regular expressions
regex identifierRegex("[a-zA-Z][a-zA-Z0-9]*");  // Starts with a letter, followed by letters or digits
regex integerRegex("^[0-9]+$");                 // Only digits for integers
regex realRegex("^[0-9]+\\.[0-9]+$");           // Format for real numbers: digits followed by a decimal point and more digits
regex operatorRegex("[=><!\\+-/*]+");
regex separatorRegex("[(),{};]@");

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
    size_t i = 0;

    while (i < input.size()) {
        char ch = input[i];
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
            i++; // Move to next character
        } else if (i + 3 < input.size() && input.substr(i, 4) == "+-**") {
            // Check for multi-character operator `+-**`
            tokens.push_back({OPERATOR, "+-**"});
            i += 4; // Skip all 4 characters
            currentState = UNKNOWN; // Reset state
        } else if (regex_match(string(1, ch), operatorRegex)) {
            // Handle single or two-character operators
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
            
            // Check for two-character operators (e.g., `!=`, `>=`, `<=`)
            string twoCharOp = i + 1 < input.size() ? string(1, ch) + input[i + 1] : "";
            if (twoCharOp == "!=" || twoCharOp == ">=" || twoCharOp == "<=") {
                tokens.push_back({OPERATOR, twoCharOp});
                i += 2; // Skip both characters
            } else {
                tokens.push_back({OPERATOR, string(1, ch)}); // Add single-character operator
                i++; // Move to next character
            }
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
            tokens.push_back({SEPARATOR, string(1, ch)}); // Add separator as a token
            i++; // Move to next character
            currentState = UNKNOWN; // Reset state
        } else {
            // Continue building the token
            token += ch;
            i++; // Move to next character
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
