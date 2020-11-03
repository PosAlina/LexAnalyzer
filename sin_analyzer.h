#ifndef SIN_ANALIZAROT_H
#define SIN_ANALIZAROT_H

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <inttypes.h>
#include <map>
#include <vector>
#include "matrix.h"
#include "vector.h"
#include "rational_number.h"
#include "exception.h"
#include "lex_analyzer.h"

class Sin_analyzer {
    Lex curr_lex;
    type_of_lex c_type;
    std::string c_val;
    Lex_analyzer l_a;
    void gl();
    unsigned int c_row, c_column;
    std::map<std::string, Ident*> dec_var;//Declared variables
    std::map<std::string, Ident*>::iterator it;
    std::vector<Lex> constr_arg;//Arguments in constructors
    void err(const string, const type_of_lex a);
    void fin_prog();
    void fin_exp();
    void info();
    void com();
    void waste();
    void func();
    void unar();
    void sign();
    void var();
    void znak();
    void parenth();
    void multi_parenth();
    void square_brackets();
    void S();
    void declare();
    void matrix_declaration();
    void vector_declaration();
    void rational_declaration();
    void float_declaration();
    void integer_declaration();
    void constructor();
    void process();
    void expression();
  public:
    void sin_analyze();
    Sin_analyzer(const std::string filename):l_a(filename){ 
      curr_lex = l_a.analyze();
      c_type = curr_lex.get_type();
      c_val = curr_lex.get_value();
      c_row = curr_lex.get_row();
      c_column = curr_lex.get_column();
    }
};
#endif
