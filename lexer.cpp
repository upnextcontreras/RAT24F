#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

// STATE TYPES
const int ST_OPERATOR = 0;
const int ST_SEPARATOR = 1;
const int ST_IDENTIFIER = 2;
const int ST_KEYWORD = 3;
const int ST_INT = 4;
const int ST_REAL = 5;
const int ST_COMMENT = 6;
const int ST_DECIMAL = 7;
const int ST_SPACE = 8;
const int ST_ERROR = 9;

const string get_token_string[] = {
    "OPERATOR", "SEPARATOR", "IDENTIFIER",
    "KEYWORD", "INT", "REAL", "COMMENT",
    "DECIMAL", "SPACE", "ERROR"
};

// CHARACTER TYPES
const int CT_ALPHA = 0;
const int CT_SPACE = 1;
const int CT_DIGIT = 2;
const int CT_BANG = 3;
const int CT_DECIMAL = 4;
const int CT_DOLLAR = 5;
const int CT_OPERATOR = 6;
const int CT_SEPARATOR = 7;

// GROUP TYPE DEFINITIONS
const string SEPARATORS = "'()}{[],.:;";
const string OPERATORS = "*+-=/><%";
const vector<string> KEYWORDS = {
    "int", "float", "bool", "if", "else", "then",
    "endif", "while", "whileend", "do", "doend", "for",
    "forend", "input", "output", "and", "or", "function"
};

// Transition table as a 2D array
int transition_table[10][8] = {
    // CT_ALPHA, CT_SPACE, CT_DIGIT, CT_BANG, CT_DECIMAL, CT_DOLLAR, CT_OPERATOR, CT_SEPARATOR
    { ST_ERROR, ST_SPACE, ST_INT, ST_COMMENT, ST_DECIMAL, ST_ERROR, ST_OPERATOR, ST_SEPARATOR }, // ST_INT
    { ST_ERROR, ST_SPACE, ST_REAL, ST_COMMENT, ST_ERROR, ST_ERROR, ST_OPERATOR, ST_SEPARATOR },  // ST_REAL
    { ST_ERROR, ST_ERROR, ST_REAL, ST_ERROR, ST_ERROR, ST_ERROR, ST_ERROR, ST_ERROR },           // ST_DECIMAL
    { ST_KEYWORD, ST_SPACE, ST_INT, ST_COMMENT, ST_ERROR, ST_ERROR, ST_OPERATOR, ST_SEPARATOR }, // ST_SPACE
    { ST_KEYWORD, ST_SPACE, ST_INT, ST_COMMENT, ST_ERROR, ST_ERROR, ST_OPERATOR, ST_SEPARATOR }, // ST_SEPARATOR
    { ST_KEYWORD, ST_SPACE, ST_INT, ST_COMMENT, ST_ERROR, ST_ERROR, ST_ERROR, ST_SEPARATOR },    // ST_OPERATOR
    { ST_KEYWORD, ST_SPACE, ST_IDENTIFIER, ST_COMMENT, ST_ERROR, ST_IDENTIFIER, ST_OPERATOR, ST_SEPARATOR }, // ST_KEYWORD
    { ST_IDENTIFIER, ST_SPACE, ST_IDENTIFIER, ST_COMMENT, ST_ERROR, ST_IDENTIFIER, ST_OPERATOR, ST_SEPARATOR }, // ST_IDENTIFIER
    { ST_COMMENT, ST_COMMENT, ST_COMMENT, ST_SPACE, ST_COMMENT, ST_COMMENT, ST_COMMENT, ST_COMMENT }, // ST_COMMENT
    { ST_ERROR, ST_SPACE, ST_ERROR, ST_ERROR, ST_ERROR, ST_ERROR, ST_OPERATOR, ST_SEPARATOR } // ST_ERROR
};

// Function to determine the character type
int get_char_type(char ch) {
    if (isspace(ch) || ch == '\0') return CT_SPACE;
    if (isdigit(ch)) return CT_DIGIT;
    if (ch == '!') return CT_BANG;
    if (ch == '.') return CT_DECIMAL;
    if (ch == '$') return CT_DOLLAR;
    if (OPERATORS.find(ch) != string::npos) return CT_OPERATOR;
    if (SEPARATORS.find(ch) != string::npos) return CT_SEPARATOR;
    if (isalpha(ch)) return CT_ALPHA;
    return CT_SPACE;
}

// Function to check if the token is a keyword
bool is_keyword(const string& token) {
    for (const string& keyword : KEYWORDS) {
        if (keyword == token) return true;
    }
    return false;
}

// Main function for lexing
void lex(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        cout << "Error opening file: " << path << endl;
        return;
    }

    int line_number = 1;
    string token = "";
    int current_state = ST_SPACE;

    cout << "TOKENS\t\t\tLexemes" << endl;

    char ch;
    while (file.get(ch)) {
        if (ch == '\n') {
            line_number++;
        }

        int char_type = get_char_type(ch);
        int new_state = transition_table[current_state][char_type];

        if (current_state != new_state) {
            // Handle state transitions
            string return_token = token;
            int return_state = current_state;

            current_state = new_state;
            token = string(1, ch);

            if (return_state == ST_KEYWORD && !is_keyword(return_token)) {
                cout << "IDENTIFIER\t\t" << return_token << endl;
            } else if (return_state == ST_ERROR) {
                cout << "ERROR on line " << line_number << ": " << return_token << endl;
            } else {
                cout << get_token_string[return_state] << "\t\t" << return_token << endl;
            }
        } else {
            token += ch;
        }

        current_state = new_state;
    }

    file.close();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Please specify a file path to lex as the first argument." << endl;
        return 1;
    }

    string path = argv[1];
    lex(path);

    return 0;
}
