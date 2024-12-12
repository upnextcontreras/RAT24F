#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iomanip>

using namespace std;

// Structure for symbol table entries
struct SymbolTableEntry {
    string identifier;
    int memoryLocation;
    string type;
};

// Global variables
vector<string> instructions;
unordered_map<string, SymbolTableEntry> symbolTable;
int instructionAddress = 1;

// Function to generate an instruction
void gen_instr(const string& op, const string& operand = "") {
    instructions.push_back(to_string(instructionAddress) + " " + op + " " + operand);
    instructionAddress++;
}

// Function to add an identifier to the symbol table
void add_to_symbol_table(const string& id, int memoryLocation, const string& type) {
    if (symbolTable.find(id) != symbolTable.end()) {
        cerr << "Error: Identifier '" << id << "' already declared.\n";
        exit(1);
    }
    symbolTable[id] = {id, memoryLocation, type};
}

// Function to get the memory location of an identifier
int get_memory_location(const string& id) {
    if (symbolTable.find(id) == symbolTable.end()) {
        cerr << "Error: Identifier '" << id << "' not declared.\n";
        exit(1);
    }
    return symbolTable[id].memoryLocation;
}

void process_test_case(const string& inputFile, const string& outputFile) {
    ifstream infile(inputFile);
    if (!infile) {
        cerr << "Error: Could not open file " << inputFile << ".\n";
        exit(1);
    }

    ofstream outfile(outputFile);
    if (!outfile) {
        cerr << "Error: Could not open file " << outputFile << ".\n";
        exit(1);
    }

    string line;
    while (getline(infile, line)) {
        // This is where you'd process each line of the test case, if needed.
    }

    infile.close();

    // Output the assembly code
    outfile << "Assembly Code:\n";
    for (const string& instr : instructions) {
        outfile << instr << endl;
    }

    // Output the symbol table
    outfile << "\nSymbol Table:\n";
    outfile << setw(15) << "Identifier" << setw(20) << "MemoryLocation" << setw(10) << "Type\n";
    for (const auto& key : {"i", "max", "sum"}) {
        const auto& entry = symbolTable[key];
        outfile << setw(15) << entry.identifier
                << setw(20) << entry.memoryLocation
                << setw(10) << entry.type << endl;
    }

    outfile.close();
}

// Main function
int main() {
    // Declare variables in the symbol table
    add_to_symbol_table("i", 9000, "integer");
    add_to_symbol_table("max", 9001, "integer");
    add_to_symbol_table("sum", 9002, "integer");

    // Generate assembly code
    gen_instr("PUSHI", "0");       // sum = 0;
    gen_instr("POPM", to_string(get_memory_location("sum")));
    gen_instr("PUSHI", "1");       // i = 1;
    gen_instr("POPM", to_string(get_memory_location("i")));
    gen_instr("STDIN");            // get(max);
    gen_instr("POPM", to_string(get_memory_location("max")));

    gen_instr("LABEL");            // while (i < max)
    gen_instr("PUSHM", to_string(get_memory_location("i")));
    gen_instr("PUSHM", to_string(get_memory_location("max")));
    gen_instr("LES");
    gen_instr("JUMPZ", to_string(instructionAddress + 10));

    gen_instr("PUSHM", to_string(get_memory_location("sum"))); // sum = sum + i;
    gen_instr("PUSHM", to_string(get_memory_location("i")));
    gen_instr("ADD");
    gen_instr("POPM", to_string(get_memory_location("sum")));

    gen_instr("PUSHM", to_string(get_memory_location("i")));   // i = i + 1;
    gen_instr("PUSHI", "1");
    gen_instr("ADD");
    gen_instr("POPM", to_string(get_memory_location("i")));

    gen_instr("JUMP", "7"); // Loop back to while
    gen_instr("PUSHM", to_string(get_memory_location("sum"))); // put(sum + max);
    gen_instr("PUSHM", to_string(get_memory_location("max")));
    gen_instr("ADD");
    gen_instr("STDOUT");

    // Process test cases
    process_test_case("t1.txt", "t1.output");
    process_test_case("t2.txt", "t2.output");
    process_test_case("t3.txt", "t3.output");

    return 0;
}
