#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Lexer function prototype
void lex(const string& input);

// Test case functions
void test_case_1();
void test_case_2();
void test_case_3();

// Main function
int main() {
    cout << "Running Test Case 1..." << endl;
    test_case_1();
    cout << "\nRunning Test Case 2..." << endl;
    test_case_2();
    cout << "\nRunning Test Case 3..." << endl;
    test_case_3();
    
    return 0;
}

// Test Case 1: Simple Variable Declaration
void test_case_1() {
    string input = R"(int a = 5;
float b = 10.0;)";
    lex(input);
}

// Test Case 2: Control Structures
void test_case_2() {
    string input = R"(if (x > 5) {
    output x;
}
else {
    input y;
})";
    lex(input);
}

// Test Case 3: Loops and Functions
void test_case_3() {
    string input = R"(while (i < 10) {
    do {
        function();
    } whileend;
})";
    lex(input);
}

// Mock-up lexer function to illustrate usage
// Replace with your actual lexer logic
void lex(const string& input) {
    cout << "Lexing input: " << endl << input << endl;
    // Call your lexer logic here to process the input
    // and output tokens
}
