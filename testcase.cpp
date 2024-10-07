#include "lexer.cpp"

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
