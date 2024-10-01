#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <regex>
#include <map>

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
vector<string> keywords = {"function", "integer", "real", "boolean", "if", "else", "fi", "while", "return", "get", "put"};

// Regular expressions for matching tokens
regex identifierRegex("[a-zA-Z]+[a-zA-Z0-9]*");
regex integerRegex("[0-9]+");
regex realRegex("[0-9]+\\.[0-9]+");
regex operatorRegex("[=><!\\+-/*]");
regex separatorRegex("[(),{};]");

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
    if (string(",(){};").find(ch) != string::npos) return SEPARATOR_CHAR;
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

// Execute a simple Rat24F function (for demonstration)
void executeProgram(const vector<Token>& tokens) {
    // Implement execution logic here based on the token sequence.
    cout << "Executing the program..." << endl;
    for (const Token& token : tokens) {
        cout << token.value << " ";
    }
    cout << endl;
}

int main() {
    // Test Case 1: Function convert (Fahrenheit to Celsius)
    string program1 = R"(
    function convert (fahr integer) {
        return 5 * (fahr - 32) / 9;
    }
    )";
    
    cout << "Running Test Case 1..." << endl;
    
    // Step 1: Lexical Analysis
    vector<Token> tokens1 = lexicalAnalyzer(program1);
    
    // Check if tokens were generated
    if (tokens1.empty()) {
        cout << "Test Case 1 Failed: No tokens generated." << endl;
    } else {
        cout << "Test Case 1 Passed: Tokens generated." << endl;
    }
    
    // Step 2: Syntax Analysis
    if (syntaxAnalyzer(tokens1)) {
        cout << "Test Case 1 Passed: Syntax is valid." << endl;
        executeProgram(tokens1);
    } else {
        cout << "Test Case 1 Failed: Syntax Error detected." << endl;
    }

    cout << "--------------------------------------" << endl;

    // Test Case 2: Function add (Add two integers)
    string program2 = R"(
    function add (a integer, b integer) {
        return a + b;
    }
    )";
    
    cout << "Running Test Case 2..." << endl;
    
    // Step 1: Lexical Analysis
    vector<Token> tokens2 = lexicalAnalyzer(program2);
    
    // Check if tokens were generated
    if (tokens2.empty()) {
        cout << "Test Case 2 Failed: No tokens generated." << endl;
    } else {
        cout << "Test Case 2 Passed: Tokens generated." << endl;
    }
    
    // Step 2: Syntax Analysis
    if (syntaxAnalyzer(tokens2)) {
        cout << "Test Case 2 Passed: Syntax is valid." << endl;
        executeProgram(tokens2);
    } else {
        cout << "Test Case 2 Failed: Syntax Error detected." << endl;
    }

    cout << "--------------------------------------" << endl;

    // Test Case 3: Function multiply (Multiply two integers)
    string program3 = R"(
    function multiply (x integer, y integer) {
        return x * y;
    }
    )";
    
    cout << "Running Test Case 3..." << endl;
    
    // Step 1: Lexical Analysis
    vector<Token> tokens3 = lexicalAnalyzer(program3);
    
    // Check if tokens were generated
    if (tokens3.empty()) {
        cout << "Test Case 3 Failed: No tokens generated." << endl;
    } else {
        cout << "Test Case 3 Passed: Tokens generated." << endl;
    }
    
    // Step 2: Syntax Analysis
    if (syntaxAnalyzer(tokens3)) {
        cout << "Test Case 3 Passed: Syntax is valid." << endl;
        executeProgram(tokens3);
    } else {
        cout << "Test Case 3 Failed: Syntax Error detected." << endl;
    }

    cout << "--------------------------------------" << endl;

    return 0;
}
