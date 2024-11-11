#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <regex>
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

// Operators and Separators
vector<string> operators = {"!=", ">=", "<=", "==", "=>", "=", ">", "<", "+-**", "+", "-", "*", "/"};
vector<string> separators = {"@)(", "@", "(", ")", "{", "}", ";", ","};

// Regular expressions for matching tokens
regex identifierRegex("^[a-zA-Z](?:[a-zA-Z0-9]*[a-zA-Z])?$");  
regex integerRegex("^[0-9]+$");         
regex realRegex("^[0-9]+\\.[0-9]+$");  

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

// Function to match the longest possible operator
string matchOperator(const string& input, size_t pos) {
    size_t maxLen = 0;
    string matchedOp;
    for (const string& op : operators) {
        size_t len = op.length();
        if (input.substr(pos, len) == op && len > maxLen) {
            maxLen = len;
            matchedOp = op;
        }
    }
    return matchedOp;
}

// Function to match the longest possible separator
string matchSeparator(const string& input, size_t pos) {
    size_t maxLen = 0;
    string matchedSep;
    for (const string& sep : separators) {
        size_t len = sep.length();
        if (input.substr(pos, len) == sep && len > maxLen) {
            maxLen = len;
            matchedSep = sep;
        }
    }
    return matchedSep;
}

// Lexical Analyzer
vector<Token> lexicalAnalyzer(const string& input) {
    vector<Token> tokens;
    string token;

    for (size_t i = 0; i < input.length();) {
        char ch = input[i];

        // Check for whitespace
        if (isspace(ch)) {
            if (!token.empty()) {
                // Process token
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
            ++i;
            continue;
        }

        // Check for operator
        string op = matchOperator(input, i);
        if (!op.empty()) {
            if (!token.empty()) {
                // Process token
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
            tokens.push_back({OPERATOR, op});
            i += op.length();
            continue;
        }

        // Check for separator
        string sep = matchSeparator(input, i);
        if (!sep.empty()) {
            if (!token.empty()) {
                // Process token
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
            tokens.push_back({SEPARATOR, sep});
            i += sep.length();
            continue;
        }

        // Append character to token
        token += ch;
        ++i;
    }

    // Process final token
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

// Syntax Analyzer
bool syntaxAnalyzer(const vector<Token>& tokens, ofstream& outfile) {
    if (!outfile.is_open()) {
        cerr << "Output file stream is not open!" << endl;
        return false;
    }

    // Write headers
    outfile << left << setw(15) << "Token" << "Lexeme" << endl;
    outfile << "-------------------------------" << endl;

    for (const Token& token : tokens) {
        outfile << left << setw(15) << tokenTypeToString(token.type) << token.value << endl;
    }
    return true;
}

// File processing
void processInputFromFile(const string &inputFilename, const string &outputFilename) {
    ifstream infile(inputFilename);
    if (!infile) {
        cerr << "Error opening input file: " << inputFilename << endl;
        return;
    }

    ofstream outfile(outputFilename);
    if (!outfile) {
        cerr << "Error opening output file: " << outputFilename << endl;
        infile.close();
        return;
    }

    string line, input;
    while (getline(infile, line)) {
        // Skip lines starting with "[*"
        if (line.rfind("[*", 0) == 0) {
            continue;
        }
        input += line + "\n";
    }
    infile.close();

    vector<Token> tokens = lexicalAnalyzer(input);
    if (!syntaxAnalyzer(tokens, outfile)) {
        cerr << "Syntax analysis failed." << endl;
    }
    outfile.close();
}

// Main function
int main() {
    string inputFilename = "t1.txt";
    string outputFilename = "t1.output.txt";
    processInputFromFile(inputFilename, outputFilename);
    cout << "Lexical and syntax analysis completed. Results are in " << outputFilename << endl;
    return 0;
}
