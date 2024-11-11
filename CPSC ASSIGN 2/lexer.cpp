#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <regex>
#include <fstream>

using namespace std;

// Token types
enum TokenType {
    KEYWORD, IDENTIFIER, INTEGER, REAL, OPERATOR, SEPARATOR, BOOLEAN_LITERAL, UNKNOWN
};

// Token structure
struct Token {
    TokenType type;
    string value;
};

// List of Rat24F keywords
vector<string> keywords = {
    "function", "integer", "real", "boolean", "if", "else", "fi",
    "while", "return", "get", "put", "true", "false", "then", "do", "od", "break"
};

// Operators and Separators
vector<string> operators = {
    "!==", "!=", ">=", "<=", "==", "=>", "+=", "-=", "*=", "/=", "%=",
    "+", "-", "*", "/", "%", "=", ">", "<"
};
vector<string> separators = {
    "(", ")", "{", "}", ";", ","
};

// Regular expressions for matching tokens
regex identifierRegex("^[a-zA-Z][a-zA-Z0-9]*$");
regex integerRegex("^[0-9]+$");
regex realRegex("^[0-9]+\\.[0-9]+$");

// Function to check if a string is a keyword
bool isKeyword(const string& word) {
    return find(keywords.begin(), keywords.end(), word) != keywords.end();
}

// Function to convert TokenType enum to user-friendly string
string tokenTypeToString(TokenType type) {
    switch (type) {
        case KEYWORD:           return "Keyword";
        case IDENTIFIER:        return "Identifier";
        case INTEGER:           return "Integer";
        case REAL:              return "Real";
        case OPERATOR:          return "Operator";
        case SEPARATOR:         return "Separator";
        case BOOLEAN_LITERAL:   return "BooleanLiteral";
        default:                return "Unknown";
    }
}

// Function to match the longest possible operator
string matchOperator(const string& input, size_t pos) {
    size_t maxLen = 0;
    string matchedOp;
    for (const string& op : operators) {
        size_t len = op.length();
        if (pos + len <= input.length() && input.substr(pos, len) == op && len > maxLen) {
            matchedOp = op;
            maxLen = len;
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
        if (pos + len <= input.length() && input.substr(pos, len) == sep && len > maxLen) {
            matchedSep = sep;
            maxLen = len;
        }
    }
    return matchedSep;
}

// Global indentation variable
string indent = "";

// Function to print a token without indentation
void printToken(const Token& token, ofstream& outfile) {
    outfile << "Token: " << tokenTypeToString(token.type) << " Lexeme: " << token.value << endl;
}

// Function to print a rule with current indentation
void printRule(const string& rule, ofstream& outfile) {
    outfile << indent << rule << endl;
}

// Functions to adjust indentation depth
void increaseIndent() { indent += " "; }
void decreaseIndent() { if (!indent.empty()) indent.pop_back(); }

// Lexical Analyzer
vector<Token> lexicalAnalyzer(const string& input) {
    vector<Token> tokens;
    string token;
    size_t i = 0;

    while (i < input.length()) {
        char ch = input[i];

        // Process accumulated token on whitespace
        if (isspace(ch)) {
            if (!token.empty()) {
                // Process the accumulated token
                if (isKeyword(token)) {
                    if (token == "true" || token == "false") {
                        tokens.push_back({BOOLEAN_LITERAL, token});
                    } else {
                        tokens.push_back({KEYWORD, token});
                    }
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
                // Process the accumulated token before the operator
                if (isKeyword(token)) {
                    if (token == "true" || token == "false") {
                        tokens.push_back({BOOLEAN_LITERAL, token});
                    } else {
                        tokens.push_back({KEYWORD, token});
                    }
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
                // Process the accumulated token before the separator
                if (isKeyword(token)) {
                    if (token == "true" || token == "false") {
                        tokens.push_back({BOOLEAN_LITERAL, token});
                    } else {
                        tokens.push_back({KEYWORD, token});
                    }
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

        // Accumulate characters for tokens
        if (isalnum(ch) || ch == '.') {
            token += ch;
            ++i;
        }
        else {
            // Process accumulated token before unknown character
            if (!token.empty()) {
                if (isKeyword(token)) {
                    if (token == "true" || token == "false") {
                        tokens.push_back({BOOLEAN_LITERAL, token});
                    } else {
                        tokens.push_back({KEYWORD, token});
                    }
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
            // Handle unknown characters
            tokens.push_back({UNKNOWN, string(1, ch)});
            ++i;
        }
    }

    // Process any remaining token
    if (!token.empty()) {
        if (isKeyword(token)) {
            if (token == "true" || token == "false") {
                tokens.push_back({BOOLEAN_LITERAL, token});
            } else {
                tokens.push_back({KEYWORD, token});
            }
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

// Function to perform lexical analysis and write to output file
void lexicalAnalysisToFile(const string &input, const string &outputFilename) {
    vector<Token> tokens = lexicalAnalyzer(input);
    ofstream outfile(outputFilename);

    if (!outfile) {
        cerr << "Error opening lexical output file: " << outputFilename << endl;
        return;
    }

    outfile << left << "Token          Lexeme" << endl;
    outfile << "-------------------------------" << endl;
    for (const Token& token : tokens) {
        outfile << left << setw(15) << tokenTypeToString(token.type) << token.value << endl;
    }
    outfile.close();
}

// Function prototypes
bool syntaxAnalyzer(vector<Token>& tokens, const string& outputFilename);
void parseProgram(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseDeclaration(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseVariableDeclaration(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseFunctionDeclaration(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseIdentifierList(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseParameterList(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseParameter(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseFunctionBody(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseStatement(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseAssign(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseExpression(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseExpressionPrime(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseTerm(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseTermPrime(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseFactor(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseWhileStatement(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseIfStatement(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseReturnStatement(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parsePutStatement(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseGetStatement(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseFunctionCall(vector<Token>& tokens, size_t& index, ofstream& outfile);
void parseArgumentList(vector<Token>& tokens, size_t& index, ofstream& outfile);
void syntaxError(const string& message, const vector<Token>& tokens, size_t index, ofstream& outfile);
void processInputFromFile(const string &inputFilename, const string &lexerOutputFilename, const string &syntaxOutputFilename);

// Syntax Analyzer - writes to specified output file
bool syntaxAnalyzer(vector<Token>& tokens, const string& outputFilename) {
    ofstream outfile(outputFilename);
    if (!outfile) {
        cerr << "Error opening syntax output file: " << outputFilename << endl;
        return false;
    }

    size_t index = 0;
    parseProgram(tokens, index, outfile); // Start parsing from <Program>

    outfile.close();
    return true;
}

// Error handling function for syntax errors, outputting to file
void syntaxError(const string& message, const vector<Token>& tokens, size_t index, ofstream& outfile) {
    outfile << "Syntax Error: " << message << " at token '";
    if (index < tokens.size()) {
        outfile << tokens[index].value << "' (" << tokenTypeToString(tokens[index].type) << ")";
    } else {
        outfile << "EOF";
    }
    outfile << endl;
}

// Parsing functions

void parseProgram(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    // Start parsing by determining if it's a Declaration or Statement
    while (index < tokens.size()) {
        if (tokens[index].value == "function" || tokens[index].value == "integer" ||
            tokens[index].value == "real" || tokens[index].value == "boolean") {
            parseDeclaration(tokens, index, outfile);
        } else {
            parseStatement(tokens, index, outfile);
        }
    }
}

void parseDeclaration(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (tokens[index].value == "function") {
        parseFunctionDeclaration(tokens, index, outfile);
    } else if (tokens[index].value == "integer" || tokens[index].value == "real" || tokens[index].value == "boolean") {
        parseVariableDeclaration(tokens, index, outfile);
    } else {
        syntaxError("Expected declaration", tokens, index, outfile);
        ++index;
    }
}

void parseVariableDeclaration(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    // Print the grammar rule before parsing components
    printRule("<VariableDeclaration> -> (integer | real | boolean) <IdentifierList> ;", outfile);
    increaseIndent();

    // Print the token: Keyword (integer/real/boolean)
    printToken(tokens[index], outfile);
    ++index; // Consume the type keyword

    // Parse <IdentifierList>
    parseIdentifierList(tokens, index, outfile);

    // Expect Separator ';'
    if (index < tokens.size() && tokens[index].value == ";") {
        // Print the token: Separator ';'
        printToken(tokens[index], outfile);
        // Print the grammar rule for ';'
        printRule(";", outfile);
        ++index; // Consume ';'
    } else {
        syntaxError("Expected ';' after variable declaration", tokens, index, outfile);
    }

    decreaseIndent();
}

void parseIdentifierList(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    // Print the grammar rule
    printRule("<IdentifierList> -> <Identifier> { , <Identifier> }", outfile);
    increaseIndent();
    if (index < tokens.size() && tokens[index].type == IDENTIFIER) {
        // Print the token: Identifier
        printToken(tokens[index], outfile);
        ++index; // Consume identifier
        while (index < tokens.size() && tokens[index].value == ",") {
            // Print the token: Separator ','
            printToken(tokens[index], outfile);
            // Print the grammar rule for ','
            printRule(",", outfile);
            ++index; // Consume ','
            if (index < tokens.size() && tokens[index].type == IDENTIFIER) {
                // Print the next identifier token
                printToken(tokens[index], outfile);
                ++index; // Consume identifier
            } else {
                syntaxError("Expected identifier after ','", tokens, index, outfile);
                break;
            }
        }
    } else {
        syntaxError("Expected identifier in declaration", tokens, index, outfile);
    }
    decreaseIndent();
}

void parseFunctionDeclaration(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    // Print the grammar rule before parsing components
    printRule("<FunctionDeclaration> -> function <Identifier> ( [<ParameterList>] ) <FunctionBody>", outfile);
    increaseIndent();

    // Print the token: Keyword 'function'
    printToken(tokens[index], outfile);
    ++index; // Consume 'function'

    // Expect Identifier (function name)
    if (index < tokens.size() && tokens[index].type == IDENTIFIER) {
        // Print the token: Identifier (function name)
        printToken(tokens[index], outfile);
        ++index; // Consume function name
    } else {
        syntaxError("Expected identifier after 'function'", tokens, index, outfile);
    }

    // Expect Separator '('
    if (index < tokens.size() && tokens[index].value == "(") {
        // Print the token: Separator '('
        printToken(tokens[index], outfile);
        // Print the grammar rule for '('
        printRule("(", outfile);
        increaseIndent();
        ++index; // Consume '('

        // Parse <ParameterList> if not ')'
        if (index < tokens.size() && tokens[index].value != ")") {
            parseParameterList(tokens, index, outfile);
        }

        // Expect Separator ')'
        if (index < tokens.size() && tokens[index].value == ")") {
            // Print the token: Separator ')'
            printToken(tokens[index], outfile);
            // Print the grammar rule for ')'
            printRule(")", outfile);
            decreaseIndent();
            ++index; // Consume ')'
        } else {
            syntaxError("Expected ')' after parameters in function declaration", tokens, index, outfile);
        }
    } else {
        syntaxError("Expected '(' after function name", tokens, index, outfile);
    }

    // Parse <FunctionBody>
    parseFunctionBody(tokens, index, outfile);

    decreaseIndent();
}

void parseParameterList(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    // Print the grammar rule
    printRule("<ParameterList> -> <Parameter> { , <Parameter> }", outfile);
    increaseIndent();
    parseParameter(tokens, index, outfile);
    while (index < tokens.size() && tokens[index].value == ",") {
        // Print the token: Separator ','
        printToken(tokens[index], outfile);
        // Print the grammar rule for ','
        printRule(",", outfile);
        ++index; // Consume ','
        parseParameter(tokens, index, outfile);
    }
    decreaseIndent();
}

void parseParameter(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    // Print the grammar rule
    printRule("<Parameter> -> <Identifier>", outfile);
    increaseIndent();
    if (index < tokens.size() && tokens[index].type == IDENTIFIER) {
        // Print the token: Identifier
        printToken(tokens[index], outfile);
        ++index; // Consume identifier
    } else {
        syntaxError("Expected identifier in parameter list", tokens, index, outfile);
    }
    decreaseIndent();
}

void parseFunctionBody(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    // Print the grammar rule
    printRule("<FunctionBody> -> { { <Declaration> | <Statement> } }", outfile);
    increaseIndent();
    if (index < tokens.size() && tokens[index].value == "{") {
        // Print the token: Separator '{'
        printToken(tokens[index], outfile);
        // Print the grammar rule for '{'
        printRule("{", outfile);
        ++index; // Consume '{'

        increaseIndent(); // Indent for declarations and statements

        while (index < tokens.size() && tokens[index].value != "}") {
            if (tokens[index].value == "function" || tokens[index].value == "integer" ||
                tokens[index].value == "real" || tokens[index].value == "boolean") {
                parseDeclaration(tokens, index, outfile);
            } else {
                parseStatement(tokens, index, outfile);
            }
        }

        // Expect Separator '}'
        if (index < tokens.size() && tokens[index].value == "}") {
            // Print the token: Separator '}'
            printToken(tokens[index], outfile);
            // Print the grammar rule for '}'
            printRule("}", outfile);
            ++index; // Consume '}'
        }
        else {
            syntaxError("Expected '}' to close function body", tokens, index, outfile);
        }

        decreaseIndent(); // End of declarations and statements
    }
    else {
        syntaxError("Expected '{' to start function body", tokens, index, outfile);
    }
    decreaseIndent();
}

void parseStatement(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (index >= tokens.size()) {
        return;
    }

    if (tokens[index].type == IDENTIFIER) {
        // Peek ahead to determine if it's an assignment or a function call
        if (index + 1 < tokens.size()) {
            if (tokens[index + 1].value == "=") {
                // Parse Assign Statement
                parseAssign(tokens, index, outfile);
            }
            else if (tokens[index + 1].value == "(") {
                // Parse Function Call Statement
                parseFunctionCall(tokens, index, outfile);
            }
            else {
                syntaxError("Unexpected token after identifier in statement", tokens, index, outfile);
                ++index;
            }
        }
        else {
            syntaxError("Unexpected end after identifier in statement", tokens, index, outfile);
            ++index;
        }
    }
    else if (tokens[index].value == "while") {
        parseWhileStatement(tokens, index, outfile);
    }
    else if (tokens[index].value == "if") {
        parseIfStatement(tokens, index, outfile);
    }
    else if (tokens[index].value == "return") {
        parseReturnStatement(tokens, index, outfile);
    }
    else if (tokens[index].value == "put") {
        parsePutStatement(tokens, index, outfile);
    }
    else if (tokens[index].value == "get") {
        parseGetStatement(tokens, index, outfile);
    }
    else if (tokens[index].value == "break") {
        // Print the grammar rule before parsing components
        printRule("<Statement> -> break ;", outfile);
        increaseIndent();

        // Print the token: Keyword 'break'
        printToken(tokens[index], outfile);
        ++index; // Consume 'break'

        // Expect Separator ';'
        if (index < tokens.size() && tokens[index].value == ";") {
            // Print the token: Separator ';'
            printToken(tokens[index], outfile);
            // Print the grammar rule for ';'
            printRule(";", outfile);
            ++index; // Consume ';'
        }
        else {
            syntaxError("Expected ';' after 'break'", tokens, index, outfile);
        }

        decreaseIndent();
    }
    else {
        syntaxError("Unexpected token in statement", tokens, index, outfile);
        ++index;
    }
}

void parseAssign(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    // Print the grammar rule before parsing components
    printRule("<Statement> -> <Assign>", outfile);
    printRule("<Assign> -> <Identifier> = <Expression> ;", outfile);
    increaseIndent(); // Indent nested rules

    // Ensure the current token is an Identifier
    if (index < tokens.size() && tokens[index].type == IDENTIFIER) {
        // Print the token: Identifier 'a'
        printToken(tokens[index], outfile);
        ++index; // Consume 'a'
    } else {
        syntaxError("Expected identifier in assignment", tokens, index, outfile);
        return;
    }

    // Expect Operator '='
    if (index < tokens.size() && tokens[index].value == "=") {
        // Print the token: Operator '='
        printToken(tokens[index], outfile);
        ++index; // Consume '='
    } else {
        syntaxError("Expected '=' after identifier in assignment", tokens, index, outfile);
        return;
    }

    // Parse <Expression>
    parseExpression(tokens, index, outfile);

    // Expect Separator ';'
    if (index < tokens.size() && tokens[index].value == ";") {
        // Print the token: Separator ';'
        printToken(tokens[index], outfile);
        // Print the grammar rule for ';'
        printRule(";", outfile);
        ++index; // Consume ';'
    }
    else {
        syntaxError("Expected ';' at the end of the assignment", tokens, index, outfile);
    }

    decreaseIndent();
}

void parseExpression(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    // Print the grammar rule before parsing components
    printRule("<Expression> -> <Term> <Expression Prime>", outfile);
    increaseIndent(); // Indent nested rules

    // Parse <Term>
    parseTerm(tokens, index, outfile);

    // Parse <Expression Prime>
    parseExpressionPrime(tokens, index, outfile);

    decreaseIndent(); // End of <Expression>
}

void parseExpressionPrime(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (index < tokens.size() && (tokens[index].value == "+" || tokens[index].value == "-")) {
        // Print the grammar rule
        printRule("<Expression Prime> -> " + tokens[index].value + " <Term> <Expression Prime>", outfile);
        increaseIndent(); // Indent nested rules

        // Print the operator token
        printToken(tokens[index], outfile);
        ++index; // Consume '+' or '-'

        // Parse the next <Term>
        parseTerm(tokens, index, outfile);

        // Continue with <Expression Prime>
        parseExpressionPrime(tokens, index, outfile);

        decreaseIndent();
    }
    else {
        // Print epsilon production
        printRule("<Expression Prime> -> ε", outfile);
    }
}

void parseTerm(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    // Print the grammar rule before parsing components
    printRule("<Term> -> <Factor> <Term Prime>", outfile);
    increaseIndent(); // Indent nested rules

    // Parse <Factor>
    parseFactor(tokens, index, outfile);

    // Parse <Term Prime>
    parseTermPrime(tokens, index, outfile);

    decreaseIndent(); // End of <Term>
}

void parseTermPrime(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (index < tokens.size() && (tokens[index].value == "*" || tokens[index].value == "/" || tokens[index].value == "%")) {
        // Print the grammar rule
        printRule("<Term Prime> -> " + tokens[index].value + " <Factor> <Term Prime>", outfile);
        increaseIndent(); // Indent nested rules

        // Print the operator token
        printToken(tokens[index], outfile);
        ++index; // Consume '*', '/', or '%'

        // Parse the next <Factor>
        parseFactor(tokens, index, outfile);

        // Continue with <Term Prime>
        parseTermPrime(tokens, index, outfile);

        decreaseIndent();
    }
    else {
        // Print epsilon production
        printRule("<Term Prime> -> ε", outfile);
    }
}

void parseFactor(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (index >= tokens.size()) {
        syntaxError("Unexpected end of input in factor", tokens, index, outfile);
        return;
    }

    if (tokens[index].type == IDENTIFIER) {
        // Print the grammar rule before consuming the identifier
        printRule("<Factor> -> <Identifier>", outfile);
        increaseIndent();

        // Print the token: Identifier
        printToken(tokens[index], outfile);
        ++index; // Consume identifier

        decreaseIndent();
    }
    else if (tokens[index].value == "(") {
        // Print the grammar rule before parsing components
        printRule("<Factor> -> ( <Expression> )", outfile);
        increaseIndent();

        // Print the token: Separator '('
        printToken(tokens[index], outfile);
        ++index; // Consume '('

        // Parse <Expression>
        parseExpression(tokens, index, outfile);

        // Expect Separator ')'
        if (index < tokens.size() && tokens[index].value == ")") {
            // Print the token: Separator ')'
            printToken(tokens[index], outfile);
            ++index; // Consume ')'
        }
        else {
            syntaxError("Expected ')' after expression", tokens, index, outfile);
        }

        decreaseIndent();
    }
    else if (tokens[index].type == INTEGER || tokens[index].type == REAL) {
        // Print the grammar rule before consuming the number
        printRule("<Factor> -> <" + tokenTypeToString(tokens[index].type) + ">", outfile);
        increaseIndent();

        // Print the token: Integer or Real
        printToken(tokens[index], outfile);
        ++index; // Consume number

        decreaseIndent();
    }
    else if (tokens[index].type == BOOLEAN_LITERAL) {
        // Print the grammar rule before consuming the boolean literal
        printRule("<Factor> -> BooleanLiteral", outfile);
        increaseIndent();

        // Print the token: BooleanLiteral
        printToken(tokens[index], outfile);
        ++index; // Consume boolean literal

        decreaseIndent();
    }
    else {
        syntaxError("Invalid factor", tokens, index, outfile);
        ++index; // Skip invalid token to avoid infinite loop
    }
}

void parseWhileStatement(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (tokens[index].value == "while") {
        // Print the grammar rule before parsing components
        printRule("<Statement> -> <WhileStatement>", outfile);
        printRule("<WhileStatement> -> while <Expression> do { <Statement> } od", outfile);
        increaseIndent(); // Indent nested rules

        // Print the token: Keyword 'while'
        printToken(tokens[index], outfile);
        ++index; // Consume 'while'

        // Parse <Expression>
        parseExpression(tokens, index, outfile);

        // Expect Keyword 'do'
        if (index < tokens.size() && tokens[index].value == "do") {
            // Print the token: Keyword 'do'
            printToken(tokens[index], outfile);
            // Print the grammar rule for 'do'
            printRule("do", outfile);
            ++index; // Consume 'do'

            // Expect Separator '{'
            if (index < tokens.size() && tokens[index].value == "{") {
                // Print the token: Separator '{'
                printToken(tokens[index], outfile);
                // Print the grammar rule for '{'
                printRule("{", outfile);
                increaseIndent(); // Indent for statements inside the loop

                ++index; // Consume '{'

                // Parse Statements inside the loop
                while (index < tokens.size() && tokens[index].value != "}") {
                    parseStatement(tokens, index, outfile);
                }

                // Expect Separator '}'
                if (index < tokens.size() && tokens[index].value == "}") {
                    // Print the token: Separator '}'
                    printToken(tokens[index], outfile);
                    // Print the grammar rule for '}'
                    printRule("}", outfile);
                    ++index; // Consume '}'

                    // Expect Keyword 'od'
                    if (index < tokens.size() && tokens[index].value == "od") {
                        // Print the token: Keyword 'od'
                        printToken(tokens[index], outfile);
                        // Print the grammar rule for 'od'
                        printRule("od", outfile);
                        ++index; // Consume 'od'
                    }
                    else {
                        syntaxError("Expected 'od' to close while loop", tokens, index, outfile);
                    }
                }
                else {
                    syntaxError("Expected '}' to close while loop", tokens, index, outfile);
                }
            }
            else {
                syntaxError("Expected '{' after 'do' in while statement", tokens, index, outfile);
            }
        }
        else {
            syntaxError("Expected 'do' after while condition", tokens, index, outfile);
        }

        decreaseIndent(); // End of <WhileStatement>
    }
}

void parseIfStatement(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (tokens[index].value == "if") {
        // Print the grammar rule before parsing components
        printRule("<Statement> -> <IfStatement>", outfile);
        printRule("<IfStatement> -> if <Expression> then { <Statement> } [ else { <Statement> } ] fi", outfile);
        increaseIndent(); // Indent nested rules

        // Print the token: Keyword 'if'
        printToken(tokens[index], outfile);
        ++index; // Consume 'if'

        // Parse <Expression>
        parseExpression(tokens, index, outfile);

        // Expect Keyword 'then'
        if (index < tokens.size() && tokens[index].value == "then") {
            // Print the token: Keyword 'then'
            printToken(tokens[index], outfile);
            // Print the grammar rule for 'then'
            printRule("then", outfile);
            ++index; // Consume 'then'

            // Expect Separator '{'
            if (index < tokens.size() && tokens[index].value == "{") {
                // Print the token: Separator '{'
                printToken(tokens[index], outfile);
                // Print the grammar rule for '{'
                printRule("{", outfile);
                increaseIndent(); // Indent for statements inside the if

                ++index; // Consume '{'

                // Parse Statements inside the if
                while (index < tokens.size() && tokens[index].value != "}") {
                    parseStatement(tokens, index, outfile);
                }

                // Expect Separator '}'
                if (index < tokens.size() && tokens[index].value == "}") {
                    // Print the token: Separator '}'
                    printToken(tokens[index], outfile);
                    // Print the grammar rule for '}'
                    printRule("}", outfile);
                    ++index; // Consume '}'

                    // Check for optional 'else'
                    if (index < tokens.size() && tokens[index].value == "else") {
                        // Print the token: Keyword 'else'
                        printToken(tokens[index], outfile);
                        // Print the grammar rule for 'else'
                        printRule("else", outfile);
                        ++index; // Consume 'else'

                        // Expect Separator '{'
                        if (index < tokens.size() && tokens[index].value == "{") {
                            // Print the token: Separator '{'
                            printToken(tokens[index], outfile);
                            // Print the grammar rule for '{'
                            printRule("{", outfile);
                            increaseIndent(); // Indent for statements inside the else

                            ++index; // Consume '{'

                            // Parse Statements inside the else
                            while (index < tokens.size() && tokens[index].value != "}") {
                                parseStatement(tokens, index, outfile);
                            }

                            // Expect Separator '}'
                            if (index < tokens.size() && tokens[index].value == "}") {
                                // Print the token: Separator '}'
                                printToken(tokens[index], outfile);
                                // Print the grammar rule for '}'
                                printRule("}", outfile);
                                ++index; // Consume '}'
                            }
                            else {
                                syntaxError("Expected '}' after else block", tokens, index, outfile);
                            }
                        }
                        else {
                            syntaxError("Expected '{' after 'else'", tokens, index, outfile);
                        }
                    }

                    // Expect Keyword 'fi'
                    if (index < tokens.size() && tokens[index].value == "fi") {
                        // Print the token: Keyword 'fi'
                        printToken(tokens[index], outfile);
                        // Print the grammar rule for 'fi'
                        printRule("fi", outfile);
                        ++index; // Consume 'fi'
                    }
                    else {
                        syntaxError("Expected 'fi' to close if statement", tokens, index, outfile);
                    }
                }
                else {
                    syntaxError("Expected '}' to close if block", tokens, index, outfile);
                }
            }
            else {
                syntaxError("Expected '{' after 'then' in if statement", tokens, index, outfile);
            }
        }
        else {
            syntaxError("Expected 'then' after if condition", tokens, index, outfile);
        }

        decreaseIndent(); // End of <IfStatement>
    }
}

void parseReturnStatement(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (tokens[index].value == "return") {
        // Print the grammar rule before parsing components
        printRule("<Statement> -> <ReturnStatement>", outfile);
        printRule("<ReturnStatement> -> return <Expression> ;", outfile);
        increaseIndent(); // Indent nested rules

        // Print the token: Keyword 'return'
        printToken(tokens[index], outfile);
        ++index; // Consume 'return'

        // Parse <Expression>
        parseExpression(tokens, index, outfile);

        // Expect Separator ';'
        if (index < tokens.size() && tokens[index].value == ";") {
            // Print the token: Separator ';'
            printToken(tokens[index], outfile);
            // Print the grammar rule for ';'
            printRule(";", outfile);
            ++index; // Consume ';'
        }
        else {
            syntaxError("Expected ';' after return statement", tokens, index, outfile);
        }

        decreaseIndent();
    }
    else {
        syntaxError("Expected 'return' keyword", tokens, index, outfile);
    }
}

void parsePutStatement(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (tokens[index].value == "put") {
        // Print the grammar rule before parsing components
        printRule("<Statement> -> <PutStatement>", outfile);
        printRule("<PutStatement> -> put ( <Expression> ) ;", outfile);
        increaseIndent(); // Indent nested rules

        // Print the token: Keyword 'put'
        printToken(tokens[index], outfile);
        ++index; // Consume 'put'

        // Expect Separator '('
        if (index < tokens.size() && tokens[index].value == "(") {
            // Print the token: Separator '('
            printToken(tokens[index], outfile);
            // Print the grammar rule for '('
            printRule("(", outfile);
            ++index; // Consume '('

            // Parse <Expression>
            parseExpression(tokens, index, outfile);

            // Expect Separator ')'
            if (index < tokens.size() && tokens[index].value == ")") {
                // Print the token: Separator ')'
                printToken(tokens[index], outfile);
                // Print the grammar rule for ')'
                printRule(")", outfile);
                ++index; // Consume ')'
            }
            else {
                syntaxError("Expected ')' after expression in put statement", tokens, index, outfile);
            }

            // Expect Separator ';'
            if (index < tokens.size() && tokens[index].value == ";") {
                // Print the token: Separator ';'
                printToken(tokens[index], outfile);
                // Print the grammar rule for ';'
                printRule(";", outfile);
                ++index; // Consume ';'
            }
            else {
                syntaxError("Expected ';' after put statement", tokens, index, outfile);
            }
        }
        else {
            syntaxError("Expected '(' after 'put'", tokens, index, outfile);
        }

        decreaseIndent();
    }
    else {
        syntaxError("Expected 'put' keyword", tokens, index, outfile);
    }
}

void parseGetStatement(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (tokens[index].value == "get") {
        // Print the grammar rule before parsing components
        printRule("<Statement> -> <GetStatement>", outfile);
        printRule("<GetStatement> -> get ( <Identifier> ) ;", outfile);
        increaseIndent(); // Indent nested rules

        // Print the token: Keyword 'get'
        printToken(tokens[index], outfile);
        ++index; // Consume 'get'

        // Expect Separator '('
        if (index < tokens.size() && tokens[index].value == "(") {
            // Print the token: Separator '('
            printToken(tokens[index], outfile);
            // Print the grammar rule for '('
            printRule("(", outfile);
            ++index; // Consume '('

            // Expect Identifier
            if (index < tokens.size() && tokens[index].type == IDENTIFIER) {
                // Print the token: Identifier
                printToken(tokens[index], outfile);
                // Print the grammar rule for <Identifier>
                printRule("<Identifier>", outfile);
                ++index; // Consume identifier
            }
            else {
                syntaxError("Expected identifier after '(' in get statement", tokens, index, outfile);
            }

            // Expect Separator ')'
            if (index < tokens.size() && tokens[index].value == ")") {
                // Print the token: Separator ')'
                printToken(tokens[index], outfile);
                // Print the grammar rule for ')'
                printRule(")", outfile);
                ++index; // Consume ')'
            }
            else {
                syntaxError("Expected ')' after identifier in get statement", tokens, index, outfile);
            }

            // Expect Separator ';'
            if (index < tokens.size() && tokens[index].value == ";") {
                // Print the token: Separator ';'
                printToken(tokens[index], outfile);
                // Print the grammar rule for ';'
                printRule(";", outfile);
                ++index; // Consume ';'
            }
            else {
                syntaxError("Expected ';' after get statement", tokens, index, outfile);
            }
        }
        else {
            syntaxError("Expected '(' after 'get'", tokens, index, outfile);
        }

        decreaseIndent();
    }
    else {
        syntaxError("Expected 'get' keyword", tokens, index, outfile);
    }
}

void parseFunctionCall(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (tokens[index].type == IDENTIFIER) {
        // Print the grammar rule before parsing components
        printRule("<Statement> -> <FunctionCall>", outfile);
        printRule("<FunctionCall> -> <Identifier> ( [<ArgumentList>] ) ;", outfile);
        increaseIndent(); // Indent nested rules

        // Print the token: Identifier (function name)
        printToken(tokens[index], outfile);
        ++index; // Consume function name

        // Expect Separator '('
        if (index < tokens.size() && tokens[index].value == "(") {
            // Print the token: Separator '('
            printToken(tokens[index], outfile);
            // Print the grammar rule for '('
            printRule("(", outfile);
            ++index; // Consume '('

            // Parse <ArgumentList> if not ')'
            if (index < tokens.size() && tokens[index].value != ")") {
                parseArgumentList(tokens, index, outfile);
            }

            // Expect Separator ')'
            if (index < tokens.size() && tokens[index].value == ")") {
                // Print the token: Separator ')'
                printToken(tokens[index], outfile);
                // Print the grammar rule for ')'
                printRule(")", outfile);
                ++index; // Consume ')'
            }
            else {
                syntaxError("Expected ')' after arguments in function call", tokens, index, outfile);
            }

            // Expect Separator ';'
            if (index < tokens.size() && tokens[index].value == ";") {
                // Print the token: Separator ';'
                printToken(tokens[index], outfile);
                // Print the grammar rule for ';'
                printRule(";", outfile);
                ++index; // Consume ';'
            }
            else {
                syntaxError("Expected ';' after function call", tokens, index, outfile);
            }
        }
        else {
            syntaxError("Expected '(' after function name in function call", tokens, index, outfile);
        }

        decreaseIndent();
    }
    else {
        syntaxError("Expected function name in function call", tokens, index, outfile);
    }
}

void parseArgumentList(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    // Print the grammar rule
    printRule("<ArgumentList> -> <Expression> { , <Expression> }", outfile);
    increaseIndent();
    parseExpression(tokens, index, outfile);
    while (index < tokens.size() && tokens[index].value == ",") {
        // Print the token: Separator ','
        printToken(tokens[index], outfile);
        // Print the grammar rule for ','
        printRule(",", outfile);
        ++index; // Consume ','
        parseExpression(tokens, index, outfile);
    }
    decreaseIndent();
}

// Adjusted processInputFromFile function to accept filenames
void processInputFromFile(const string &inputFilename, const string &lexerOutputFilename, const string &syntaxOutputFilename) {
    ifstream infile(inputFilename);
    if (!infile) {
        cerr << "Error opening input file: " << inputFilename << endl;
        return;
    }

    string line, input;
    while (getline(infile, line)) {
        // Skip lines containing [* and *]
        if (line.find("[*") != string::npos || line.find("*]") != string::npos) {
            continue;
        }
        input += line + "\n";
    }
    infile.close();

    // Lexical analysis - output to lexerOutputFilename
    lexicalAnalysisToFile(input, lexerOutputFilename);

    // Perform lexical analysis and use tokens for syntax analysis
    vector<Token> tokens = lexicalAnalyzer(input);
    syntaxAnalyzer(tokens, syntaxOutputFilename);
}

// Main function
int main() {
    vector<string> testFiles = {"t1.txt", "t2.txt", "t3.txt"};
    for (size_t i = 0; i < testFiles.size(); ++i) {
        string inputFilename = testFiles[i];
        string lexerOutputFilename = "lexer" + to_string(i + 1) + ".output";
        string syntaxOutputFilename = "syntax" + to_string(i + 1) + ".output";

        cout << "Processing file: " << inputFilename << endl;
        processInputFromFile(inputFilename, lexerOutputFilename, syntaxOutputFilename);
        cout << "Lexical analysis completed. Results are in " << lexerOutputFilename << endl;
        cout << "Syntax analysis completed. Results are in " << syntaxOutputFilename << endl << endl;
    }
    return 0;
} 
