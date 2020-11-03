#ifndef LEX_ANALYZER_H
#define LEX_ANALYZER_H

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <inttypes.h>
#include <climits>
#include "lex.h"

class Lex_analyzer {
	enum state {S, Ident, Number, Double, MultiCom, Com, Quote, Znak};
    state CS;
    static std::string TW[];
    static type_of_lex words[];
    static std::string TD[];
    static type_of_lex dlms[];
    FILE *fp;
    char c;
    std::string buf;
    unsigned int row, column;

    void clear();
    void add();
    int look(const std::string, std::string*);
    void gc();
  public:
    Lex_analyzer(const std::string);
    ~Lex_analyzer();
    Lex analyze();
};

#endif
