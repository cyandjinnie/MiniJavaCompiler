#pragma once

#include <map>
#include <string>
#include <fstream>
#include "scanner.h"
#include "parser.hh"
#include "Program.h"

class Driver {
 public:
    Driver();
    std::map<std::string, int> variables;
    int result;
    int Parse(const std::string& f);
    std::string file;
    bool trace_parsing;

    void scan_begin();
    void scan_end();

    bool trace_scanning;
    yy::location location;
    
    Program* program;
    void PrintAST(const std::string& filename);
    void PrintIR(const std::string& filename);
    int Run();

    friend class Scanner;
    Scanner scanner;
    yy::parser parser;
 private:
    std::ifstream stream;

};
