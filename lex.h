#ifndef LEX_H
#define LEX_H
#include <ostream>
#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <inttypes.h>
#include <climits>
#include <string>
#include "matrix.h"
#include "vector.h"
#include "rational_number.h"
#include "exception.h"

enum type_of_lex {
  LEX_NULL, //0
  LEX_DECLARE, //1
  LEX_PROCESS, //2
  LEX_MULTI_COMMENT, //3
  LEX_COMMENT, //4
  LEX_MATRIX, //5
  LEX_VECTOR, //6
  LEX_RATIONAL, //7
  LEX_FLOAT, //8
  LEX_INTEGER, //9

  LEX_VAR, //10
  LEX_DIGIT, //11
  LEX_DOUBLE, //12
  LEX_STRING, //13

  LEX_PRINT, //14
  LEX_MAKE_CANONICAL, //15
  LEX_ROTATE, //16
  LEX_READ, //17
  LEX_WRITE, //18
  LEX_ROW, //19
  LEX_COLUMN, //20
  LEX_INFO, //21

  LEX_SLASH, //22
  LEX_COLON, //23
  LEX_SEMICOLON, //24
  LEX_COMMA, //25
  LEX_OPEN_PARENTHESIS, //26
  LEX_CLOSE_PARENTHESIS, //27
  LEX_OPEN_SQUARE_BRACKET, //28
  LEX_CLOSE_SQUARE_BRACKET, //29
  LEX_EQUAL, //30
  LEX_MULTI, //31
  LEX_MINUS, //32
  LEX_PLUS, //33
  LEX_POWER, //34
  LEX_UNARY_MINUS, //35
  LEX_UNARY_PLUS, //36

  LEX_FIN //37
};

class Lex {
    type_of_lex t_lex;
    std::string v_lex ;
    unsigned int r_lex;
    unsigned int c_lex;
  public:
    Lex() {
      t_lex = LEX_NULL;
      v_lex = "";
      r_lex = 0;
      c_lex = 0;
    }
    Lex(unsigned int rw, unsigned int cn, type_of_lex t = LEX_NULL, const std::string v = "") {
      t_lex = t;
	  v_lex = v;
      r_lex = rw;
      c_lex = cn;
    }
    Lex(const Lex &lx) {
      t_lex = lx.get_type();
	  v_lex = lx.get_value();
      r_lex = lx.get_row();
      c_lex = lx.get_column();
    }
    void set_type(type_of_lex t) {t_lex = t;}
	void set_value(int v) {v_lex = v;}
	void set_value(std::string v) {v_lex = v;}
    void set_row(unsigned int r) {r_lex = r;}
	void set_column(unsigned int c) {c_lex = c;}
	type_of_lex get_type() const {return t_lex;}
	std::string get_value() const {return v_lex;}
    unsigned int get_row() const {return r_lex;}
    unsigned int get_column() const {return c_lex;}
    std::string get_string(type_of_lex t) const {
      switch(t) {
		case LEX_NULL: return "NULL";
        case LEX_DECLARE: return "WORD declare:";
        case LEX_PROCESS: return "WORD process:";
        case LEX_MULTI_COMMENT: return "MULTI_COMMENT";
        case LEX_COMMENT: return "COMMENT";
        case LEX_MATRIX: return "WORD matrix:";
		case LEX_VECTOR: return "WORD vector:";
	    case LEX_RATIONAL: return "WORD rational:";
		case LEX_FLOAT: return "WORD float:";
        case LEX_INTEGER: return "WORD integer:";

        case LEX_VAR: return "VAR";
        case LEX_DIGIT: return "DIGIT";
        case LEX_DOUBLE: return "DOUBLE";
        case LEX_STRING: return "STRING";

        case LEX_PRINT: return "FUNCTION print";
        case LEX_MAKE_CANONICAL: return "FUNCTION make_canonical";
		case LEX_ROTATE: return "FUNCTION rotate";
		case LEX_READ: return "FUNCTION read";
	    case LEX_WRITE: return "FUNCTION write";
		case LEX_ROW: return "FUNCTION row";
        case LEX_COLUMN: return "FUNCTION column";
		case LEX_INFO: return "COMMAND info";

        case LEX_SLASH: return "SLASH";
        case LEX_COLON: return "COLON";
        case LEX_SEMICOLON: return "SEMICOLON";
		case LEX_COMMA: return "COMMA";
	    case LEX_OPEN_PARENTHESIS: return "OPEN_PARENTHESIS";
		case LEX_CLOSE_PARENTHESIS: return "CLOSE_PARENTHESIS";
        case LEX_OPEN_SQUARE_BRACKET: return "OPEN_SQUARE_BRACKET";
        case LEX_CLOSE_SQUARE_BRACKET: return "CLOSE_SQUARE_BRACKET";
        case LEX_EQUAL: return "EQUAL";
        case LEX_MULTI: return "MULTI";
        case LEX_MINUS: return "MINUS";
		case LEX_PLUS: return "PLUS";
		case LEX_POWER: return "POWER";
		case LEX_UNARY_MINUS: return "UNARY_MINUS";
		case LEX_UNARY_PLUS: return "UNARY_PLUS";
                case LEX_FIN: return "End of file";
        default: return "UNDEFINED";
      }
    }
    friend std::ostream& operator << (std::ostream& str, const Lex &lx) {
      str << "[" << lx.get_row() << ", " << lx.get_column() << "] = " << lx.get_string(lx.get_type());
      if (!((lx.get_value()).empty())) {str << ": " << lx.get_value();}
      str << "\n";
      return str;
    }
};

class Ident {
    bool declare;
    bool assign;
	type_of_lex type;
    std::string name;
    unsigned int row;
    unsigned int column;
  public:
    Ident() {
      declare = false;
      assign = false;
    }
	Ident(type_of_lex t, const std::string n, unsigned int r = 0, unsigned int c = 0, bool d = false, bool a = false) {
      declare = d;
      assign = a;
	  type = t;
	  name = n;
	  row = r;
	  column = c;
    }
  virtual std::string get_t(){}
	virtual ~Ident(){}
    void set_name(const std::string n) {name = n;}
    void set_declare() {declare = true;}
	void set_assign() {assign = true;}
	void set_type(type_of_lex t) {type = t;}
	bool get_declare() {return declare;}
    type_of_lex get_type() {return type;}
    bool get_assign() {return assign;}
    std::string get_name() {return name;}
};

class Tabl_ident {
    Ident *p;
    unsigned int size;
    unsigned int top;
  public:
    Tabl_ident(int max_size) {
	  p = new Ident[size = max_size];
      top = 0;
    }
    ~Tabl_ident() {delete []p;}
    Ident& operator[](unsigned int k) {return p[k];}
    unsigned int put(const std::string buf) {
	  for(unsigned int i = 0; i < top; i++) {
	    if (buf == p[i].get_name()) {return i;}
      }
      p[top].set_name(buf);
      top++;
      return top;
    }
};

class Ident_Matrix: public Ident {
    Matrix value;
  public:
    Ident_Matrix(const std::string &n, unsigned int r = 0, unsigned int c = 0, const Matrix &v = Matrix(0, 0)):Ident(LEX_MATRIX, n, r, c) {value = v;}
    std::string get_t() {return "matrix";}
    void set_value(const Matrix& v) {value = v;}
    const Matrix &get_value() const {return value;}
    friend std::ostream& operator << (std::ostream& s, const Ident_Matrix& v) {return s << to_string(v.get_value());}
};

class Ident_Vector: public Ident {
    Vector value;
  public:
    Ident_Vector(const std::string &n, unsigned int r = 0, unsigned int c = 0, const Vector &v = Vector(0, 0)):Ident(LEX_VECTOR, n, r, c) {value = v;}
    std::string get_t() {return "vector";}
    void set_value(const Vector& v) {value = v;}
	const Vector &get_value() const {return value;}
    friend std::ostream& operator << (std::ostream& s, const Ident_Vector& v) {return s << to_string(v.get_value());}
};

class Ident_Rational: public Ident {
    Rational_number value;
  public:
    Ident_Rational(const std::string &n, unsigned int r = 0, unsigned int c = 0, const Rational_number &v = Rational_number(0)):Ident(LEX_RATIONAL, n, r, c) {value = v;}
    std::string get_t() {return "rational";}
    void set_value(const Rational_number& v) {value = v;}
	const Rational_number &get_value() const {return value;}
    friend std::ostream& operator << (std::ostream& s, const Ident_Rational& v) {return s << to_string(v.get_value());}
};

class Ident_Float: public Ident {
    double value;
  public:
    Ident_Float(const std::string &n, unsigned int r = 0, unsigned int c = 0, const double &v = 0): Ident(LEX_FLOAT, n, r, c) {value = v;}
    std::string get_t() {return "float";}
    void set_value(const double& v) {value = v;}
	const double& get_value() const {return value;}
    friend std::ostream& operator << (std::ostream& s, const Ident_Float& v) {return s << v.get_value();}
};

class Ident_Integer: public Ident {
    int value;
  public:
    Ident_Integer(const std::string &n, unsigned int r = 0 , unsigned int c = 0, const int &v = 0):Ident(LEX_INTEGER, n, r, c) {value = v;}
    std::string get_t() {return "integer";}
    void set_value(const int& v) {value = v;}
    const int& get_value() const {return value;}
    friend std::ostream& operator << (std::ostream& s, const Ident_Integer& v) {return s << v.get_value();}
};

#endif
