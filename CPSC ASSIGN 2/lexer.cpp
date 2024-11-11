#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <regex>
#include <fstream>

using namespace std;

enum TokenType {
    KEYWORD, IDENTIFIER, INTEGER, REAL, OPERATOR, SEPARATOR, BOOLEAN_LITERAL, UNKNOWN
};


struct Token {
    TokenType type;
    string value;
};


vector<string> keywords = {
    "function", "integer", "real", "boolean", "if", "else", "fi",
    "while", "return", "get", "put", "true", "false", "then", "do", "od", "break"
};


vector<string> operators = {
    "!==", "!=", ">=", "<=", "==", "=>", "+=", "-=", "*=", "/=", "%=",
    "+", "-", "*", "/", "%", "=", ">", "<"
};
vector<string> separators = {
    "(", ")", "{", "}", ";", ","
};

regex identifierRegex("^[a-zA-Z][a-zA-Z0-9]*$");
regex integerRegex("^[0-9]+$");
regex realRegex("^[0-9]+\\.[0-9]+$");

bool isKeyword(const string& word) {
    return find(keywords.begin(), keywords.end(), word) != keywords.end();
}

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


string indent = "";


void printToken(const Token& token, ofstream& outfile) {
    outfile << "Token: " << tokenTypeToString(token.type) << " Lexeme: " << token.value << endl;
}


void printRule(const string& rule, ofstream& outfile) {
    outfile << indent << rule << endl;
}


void increaseIndent() { indent += " "; }
void decreaseIndent() { if (!indent.empty()) indent.pop_back(); }


vector<Token> lexicalAnalyzer(const string& input) {
    vector<Token> tokens;
    string token;
    size_t i = 0;

    while (i < input.length()) {
        char ch = input[i];

        if (isspace(ch)) {
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
            ++i;
            continue;
        }

        string op = matchOperator(input, i);
        if (!op.empty()) {
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
            tokens.push_back({OPERATOR, op});
            i += op.length();
            continue;
        }

        string sep = matchSeparator(input, i);
        if (!sep.empty()) {
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
            tokens.push_back({SEPARATOR, sep});
            i += sep.length();
            continue;
        }

        if (isalnum(ch) || ch == '.') {
            token += ch;
            ++i;
        }
        else {
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
            tokens.push_back({UNKNOWN, string(1, ch)});
            ++i;
        }
    }
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

bool syntaxAnalyzer(vector<Token>& tokens, const string& outputFilename) {
    ofstream outfile(outputFilename);
    if (!outfile) {
        cerr << "Error opening syntax output file: " << outputFilename << endl;
        return false;
    }

    size_t index = 0;
    parseProgram(tokens, index, outfile);

    outfile.close();
    return true;
}


void syntaxError(const string& message, const vector<Token>& tokens, size_t index, ofstream& outfile) {
    outfile << "Syntax Error: " << message << " at token '";
    if (index < tokens.size()) {
        outfile << tokens[index].value << "' (" << tokenTypeToString(tokens[index].type) << ")";
    } else {
        outfile << "EOF";
    }
    outfile << endl;
}


void parseProgram(vector<Token>& tokens, size_t& index, ofstream& outfile) {
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
    printRule("<VariableDeclaration> -> (integer | real | boolean) <IdentifierList> ;", outfile);
    increaseIndent();

    printToken(tokens[index], outfile);
    ++index;

    parseIdentifierList(tokens, index, outfile);

    if (index < tokens.size() && tokens[index].value == ";") {
        printToken(tokens[index], outfile);
        printRule(";", outfile);
        ++index;
    } else {
        syntaxError("Expected ';' after variable declaration", tokens, index, outfile);
    }

    decreaseIndent();
}

void parseIdentifierList(vector<Token>& tokens, size_t& index, ofstream& outfile) {

    printRule("<IdentifierList> -> <Identifier> { , <Identifier> }", outfile);
    increaseIndent();
    if (index < tokens.size() && tokens[index].type == IDENTIFIER) {

        printToken(tokens[index], outfile);
        ++index;
        while (index < tokens.size() && tokens[index].value == ",") {
            printToken(tokens[index], outfile);
            printRule(",", outfile);
            ++index;
            if (index < tokens.size() && tokens[index].type == IDENTIFIER) {
                printToken(tokens[index], outfile);
                ++index;
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
    printRule("<FunctionDeclaration> -> function <Identifier> ( [<ParameterList>] ) <FunctionBody>", outfile);
    increaseIndent();

    printToken(tokens[index], outfile);
    ++index;

    if (index < tokens.size() && tokens[index].type == IDENTIFIER) {
        printToken(tokens[index], outfile);
        ++index;
    } else {
        syntaxError("Expected identifier after 'function'", tokens, index, outfile);
    }

    if (index < tokens.size() && tokens[index].value == "(") {
        printToken(tokens[index], outfile);
        printRule("(", outfile);
        increaseIndent();
        ++index;

        if (index < tokens.size() && tokens[index].value != ")") {
            parseParameterList(tokens, index, outfile);
        }

        if (index < tokens.size() && tokens[index].value == ")") {
            printToken(tokens[index], outfile);
            printRule(")", outfile);
            decreaseIndent();
            ++index;
        } else {
            syntaxError("Expected ')' after parameters in function declaration", tokens, index, outfile);
        }
    } else {
        syntaxError("Expected '(' after function name", tokens, index, outfile);
    }

    parseFunctionBody(tokens, index, outfile);

    decreaseIndent();
}

void parseParameterList(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    printRule("<ParameterList> -> <Parameter> { , <Parameter> }", outfile);
    increaseIndent();
    parseParameter(tokens, index, outfile);
    while (index < tokens.size() && tokens[index].value == ",") {
        printToken(tokens[index], outfile);
        printRule(",", outfile);
        ++index;
        parseParameter(tokens, index, outfile);
    }
    decreaseIndent();
}

void parseParameter(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    printRule("<Parameter> -> <Identifier>", outfile);
    increaseIndent();
    if (index < tokens.size() && tokens[index].type == IDENTIFIER) {
        printToken(tokens[index], outfile);
        ++index;
    } else {
        syntaxError("Expected identifier in parameter list", tokens, index, outfile);
    }
    decreaseIndent();
}

void parseFunctionBody(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    printRule("<FunctionBody> -> { { <Declaration> | <Statement> } }", outfile);
    increaseIndent();
    if (index < tokens.size() && tokens[index].value == "{") {
        printToken(tokens[index], outfile);
        printRule("{", outfile);
        ++index;

        increaseIndent();

        while (index < tokens.size() && tokens[index].value != "}") {
            if (tokens[index].value == "function" || tokens[index].value == "integer" ||
                tokens[index].value == "real" || tokens[index].value == "boolean") {
                parseDeclaration(tokens, index, outfile);
            } else {
                parseStatement(tokens, index, outfile);
            }
        }

        if (index < tokens.size() && tokens[index].value == "}") {
            printToken(tokens[index], outfile);
            printRule("}", outfile);
            ++index;
        }
        else {
            syntaxError("Expected '}' to close function body", tokens, index, outfile);
        }

        decreaseIndent();
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
        if (index + 1 < tokens.size()) {
            if (tokens[index + 1].value == "=") {
                parseAssign(tokens, index, outfile);
            }
            else if (tokens[index + 1].value == "(") {
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
        printRule("<Statement> -> break ;", outfile);
        increaseIndent();

        printToken(tokens[index], outfile);
        ++index;

        if (index < tokens.size() && tokens[index].value == ";") {

            printToken(tokens[index], outfile);

            printRule(";", outfile);
            ++index;
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
    printRule("<Statement> -> <Assign>", outfile);
    printRule("<Assign> -> <Identifier> = <Expression> ;", outfile);
    increaseIndent();

    if (index < tokens.size() && tokens[index].type == IDENTIFIER) {
        printToken(tokens[index], outfile);
        ++index;
    } else {
        syntaxError("Expected identifier in assignment", tokens, index, outfile);
        return;
    }

    if (index < tokens.size() && tokens[index].value == "=") {

        printToken(tokens[index], outfile);
        ++index;
    } else {
        syntaxError("Expected '=' after identifier in assignment", tokens, index, outfile);
        return;
    }

    parseExpression(tokens, index, outfile);

    if (index < tokens.size() && tokens[index].value == ";") {
        printToken(tokens[index], outfile);
        printRule(";", outfile);
        ++index;
    }
    else {
        syntaxError("Expected ';' at the end of the assignment", tokens, index, outfile);
    }

    decreaseIndent();
}

void parseExpression(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    printRule("<Expression> -> <Term> <Expression Prime>", outfile);
    increaseIndent();

    parseTerm(tokens, index, outfile);

    parseExpressionPrime(tokens, index, outfile);

    decreaseIndent();
}

void parseExpressionPrime(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (index < tokens.size() && (tokens[index].value == "+" || tokens[index].value == "-")) {
        printRule("<Expression Prime> -> " + tokens[index].value + " <Term> <Expression Prime>", outfile);
        increaseIndent();

        printToken(tokens[index], outfile);
        ++index;

        parseTerm(tokens, index, outfile);

        parseExpressionPrime(tokens, index, outfile);

        decreaseIndent();
    }
    else {
        printRule("<Expression Prime> -> ε", outfile);
    }
}

void parseTerm(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    printRule("<Term> -> <Factor> <Term Prime>", outfile);
    increaseIndent();

    parseFactor(tokens, index, outfile);

    parseTermPrime(tokens, index, outfile);

    decreaseIndent();
}

void parseTermPrime(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (index < tokens.size() && (tokens[index].value == "*" || tokens[index].value == "/" || tokens[index].value == "%")) {
        printRule("<Term Prime> -> " + tokens[index].value + " <Factor> <Term Prime>", outfile);
        increaseIndent();

        printToken(tokens[index], outfile);
        ++index;

        parseFactor(tokens, index, outfile);

        parseTermPrime(tokens, index, outfile);

        decreaseIndent();
    }
    else {
        printRule("<Term Prime> -> ε", outfile);
    }
}

void parseFactor(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (index >= tokens.size()) {
        syntaxError("Unexpected end of input in factor", tokens, index, outfile);
        return;
    }

    if (tokens[index].type == IDENTIFIER) {
        printRule("<Factor> -> <Identifier>", outfile);
        increaseIndent();

        printToken(tokens[index], outfile);
        ++index;

        decreaseIndent();
    }
    else if (tokens[index].value == "(") {
        printRule("<Factor> -> ( <Expression> )", outfile);
        increaseIndent();

        printToken(tokens[index], outfile);
        ++index;

        parseExpression(tokens, index, outfile);

        if (index < tokens.size() && tokens[index].value == ")") {
            printToken(tokens[index], outfile);
            ++index;
        }
        else {
            syntaxError("Expected ')' after expression", tokens, index, outfile);
        }

        decreaseIndent();
    }
    else if (tokens[index].type == INTEGER || tokens[index].type == REAL) {
        printRule("<Factor> -> <" + tokenTypeToString(tokens[index].type) + ">", outfile);
        increaseIndent();

        printToken(tokens[index], outfile);
        ++index;

        decreaseIndent();
    }
    else if (tokens[index].type == BOOLEAN_LITERAL) {
        printRule("<Factor> -> BooleanLiteral", outfile);
        increaseIndent();

        printToken(tokens[index], outfile);
        ++index;

        decreaseIndent();
    }
    else {
        syntaxError("Invalid factor", tokens, index, outfile);
        ++index;
    }
}

void parseWhileStatement(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (tokens[index].value == "while") {
        printRule("<Statement> -> <WhileStatement>", outfile);
        printRule("<WhileStatement> -> while <Expression> do { <Statement> } od", outfile);
        increaseIndent();

        printToken(tokens[index], outfile);
        ++index;

        parseExpression(tokens, index, outfile);

        if (index < tokens.size() && tokens[index].value == "do") {
            printToken(tokens[index], outfile);
            printRule("do", outfile);
            ++index;
            if (index < tokens.size() && tokens[index].value == "{") {
                printToken(tokens[index], outfile);
                printRule("{", outfile);
                increaseIndent();

                ++index;

                while (index < tokens.size() && tokens[index].value != "}") {
                    parseStatement(tokens, index, outfile);
                }

                if (index < tokens.size() && tokens[index].value == "}") {
                    printToken(tokens[index], outfile);
                    printRule("}", outfile);
                    ++index;

                    if (index < tokens.size() && tokens[index].value == "od") {
                        printToken(tokens[index], outfile);
                        printRule("od", outfile);
                        ++index;
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

        decreaseIndent();
    }
}

void parseIfStatement(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (tokens[index].value == "if") {
        printRule("<Statement> -> <IfStatement>", outfile);
        printRule("<IfStatement> -> if <Expression> then { <Statement> } [ else { <Statement> } ] fi", outfile);
        increaseIndent();

        printToken(tokens[index], outfile);
        ++index;

        parseExpression(tokens, index, outfile);

        if (index < tokens.size() && tokens[index].value == "then") {

            printToken(tokens[index], outfile);

            printRule("then", outfile);
            ++index;

            if (index < tokens.size() && tokens[index].value == "{") {

                printToken(tokens[index], outfile);

                printRule("{", outfile);
                increaseIndent();

                ++index;

                while (index < tokens.size() && tokens[index].value != "}") {
                    parseStatement(tokens, index, outfile);
                }

                if (index < tokens.size() && tokens[index].value == "}") {

                    printToken(tokens[index], outfile);

                    printRule("}", outfile);
                    ++index;

                    if (index < tokens.size() && tokens[index].value == "else") {
                        printToken(tokens[index], outfile);
                        printRule("else", outfile);
                        ++index;

                        if (index < tokens.size() && tokens[index].value == "{") {
                            printToken(tokens[index], outfile);
                            printRule("{", outfile);
                            increaseIndent();
                            ++index;

                            while (index < tokens.size() && tokens[index].value != "}") {
                                parseStatement(tokens, index, outfile);
                            }

                            if (index < tokens.size() && tokens[index].value == "}") {
                                printToken(tokens[index], outfile);
                                printRule("}", outfile);
                                ++index;
                            }
                            else {
                                syntaxError("Expected '}' after else block", tokens, index, outfile);
                            }
                        }
                        else {
                            syntaxError("Expected '{' after 'else'", tokens, index, outfile);
                        }
                    }

                    if (index < tokens.size() && tokens[index].value == "fi") {
                        printToken(tokens[index], outfile);
                        printRule("fi", outfile);
                        ++index;
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

        decreaseIndent();
    }
}

void parseReturnStatement(vector<Token>& tokens, size_t& index, ofstream& outfile) {
    if (tokens[index].value == "return") {
        printRule("<Statement> -> <ReturnStatement>", outfile);
        printRule("<ReturnStatement> -> return <Expression> ;", outfile);
        increaseIndent();

        printToken(tokens[index], outfile);
        ++index;

        parseExpression(tokens, index, outfile);

        if (index < tokens.size() && tokens[index].value == ";") {
            printToken(tokens[index], outfile);
            printRule(";", outfile);
            ++index;
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
        printRule("<Statement> -> <PutStatement>", outfile);
        printRule("<PutStatement> -> put ( <Expression> ) ;", outfile);
        increaseIndent();

        printToken(tokens[index], outfile);
        ++index;
        if (index < tokens.size() && tokens[index].value == "(") {
            printToken(tokens[index], outfile);
            printRule("(", outfile);
            ++index;

            parseExpression(tokens, index, outfile);

            if (index < tokens.size() && tokens[index].value == ")") {
                printToken(tokens[index], outfile);
                printRule(")", outfile);
                ++index;
            }
            else {
                syntaxError("Expected ')' after expression in put statement", tokens, index, outfile);
            }

            if (index < tokens.size() && tokens[index].value == ";") {
                printToken(tokens[index], outfile);
                printRule(";", outfile);
                ++index;
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
        printRule("<Statement> -> <GetStatement>", outfile);
        printRule("<GetStatement> -> get ( <Identifier> ) ;", outfile);
        increaseIndent();

        printToken(tokens[index], outfile);
        ++index;

        if (index < tokens.size() && tokens[index].value == "(") {
            printToken(tokens[index], outfile);
            printRule("(", outfile);
            ++index;

            if (index < tokens.size() && tokens[index].type == IDENTIFIER) {
                printToken(tokens[index], outfile);
                printRule("<Identifier>", outfile);
                ++index;
            }
            else {
                syntaxError("Expected identifier after '(' in get statement", tokens, index, outfile);
            }

            if (index < tokens.size() && tokens[index].value == ")") {
                printToken(tokens[index], outfile);
                printRule(")", outfile);
                ++index;
            }
            else {
                syntaxError("Expected ')' after identifier in get statement", tokens, index, outfile);
            }

            if (index < tokens.size() && tokens[index].value == ";") {
                printToken(tokens[index], outfile);
                printRule(";", outfile);
                ++index;
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
        printRule("<Statement> -> <FunctionCall>", outfile);
        printRule("<FunctionCall> -> <Identifier> ( [<ArgumentList>] ) ;", outfile);
        increaseIndent();

        printToken(tokens[index], outfile);
        ++index;

        if (index < tokens.size() && tokens[index].value == "(") {
            printToken(tokens[index], outfile);
            printRule("(", outfile);
            ++index;

            if (index < tokens.size() && tokens[index].value != ")") {
                parseArgumentList(tokens, index, outfile);
            }

            if (index < tokens.size() && tokens[index].value == ")") {
                printToken(tokens[index], outfile);
                printRule(")", outfile);
                ++index;
            }
            else {
                syntaxError("Expected ')' after arguments in function call", tokens, index, outfile);
            }

            if (index < tokens.size() && tokens[index].value == ";") {
                printToken(tokens[index], outfile);
                printRule(";", outfile);
                ++index;
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
    printRule("<ArgumentList> -> <Expression> { , <Expression> }", outfile);
    increaseIndent();
    parseExpression(tokens, index, outfile);
    while (index < tokens.size() && tokens[index].value == ",") {
        printToken(tokens[index], outfile);
        printRule(",", outfile);
        ++index;
        parseExpression(tokens, index, outfile);
    }
    decreaseIndent();
}

void processInputFromFile(const string &inputFilename, const string &lexerOutputFilename, const string &syntaxOutputFilename) {
    ifstream infile(inputFilename);
    if (!infile) {
        cerr << "Error opening input file: " << inputFilename << endl;
        return;
    }

    string line, input;
    while (getline(infile, line)) {
        if (line.find("[*") != string::npos || line.find("*]") != string::npos) {
            continue;
        }
        input += line + "\n";
    }
    infile.close();

    lexicalAnalysisToFile(input, lexerOutputFilename);

    vector<Token> tokens = lexicalAnalyzer(input);
    syntaxAnalyzer(tokens, syntaxOutputFilename);
}

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
