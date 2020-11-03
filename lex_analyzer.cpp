#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <inttypes.h>
#include "lex_analyzer.h"

using namespace std;

string Lex_analyzer::TW[] = {
  "", //0
  "declare:", //1
  "process:", //2
  "matrix:", //3
  "vector:", //4
  "rational:", //5
  "float:", //6
  "integer:", //7
  "print", //8
  "make_canonical", //9
  "rotate", //10
  "read", //11
  "write", //12
  "row", //13
  "column", //14
  "info",  //15
  ""
};

string Lex_analyzer::TD[] = {
  "", //0
  "/", //1
  ":", //2
  ";", //3
  ",", //4
  "(", //5
  ")", //6
  "[", //7
  "]", //8
  "=", //9
  "*", //10
  "-", //11
  "+", //12
  "^", //13
  "--", //14
  "++", //15
  ""
};

Tabl_ident TID(100);

type_of_lex Lex_analyzer::words[] = {
  LEX_NULL, //0
  LEX_DECLARE, //1
  LEX_PROCESS, //2
  LEX_MATRIX, //3
  LEX_VECTOR, //4
  LEX_RATIONAL, //5 
  LEX_FLOAT, //6
  LEX_INTEGER, //7
  LEX_PRINT, //8
  LEX_MAKE_CANONICAL, //9
  LEX_ROTATE, //10
  LEX_READ, //11
  LEX_WRITE, //12
  LEX_ROW, //13
  LEX_COLUMN, //14
  LEX_INFO, //15
  LEX_NULL
};

type_of_lex Lex_analyzer::dlms[] = {
  LEX_NULL, //0
  LEX_SLASH, //1
  LEX_COLON, //2
  LEX_SEMICOLON, //3
  LEX_COMMA, //4
  LEX_OPEN_PARENTHESIS, //5
  LEX_CLOSE_PARENTHESIS, //6
  LEX_OPEN_SQUARE_BRACKET, //7
  LEX_CLOSE_SQUARE_BRACKET, //8
  LEX_EQUAL, //9
  LEX_MULTI, //10
  LEX_MINUS, //11
  LEX_PLUS, //12
  LEX_POWER, //13
  LEX_UNARY_MINUS, //14
  LEX_UNARY_PLUS, //15
  LEX_NULL
};

void Lex_analyzer::clear() {buf.clear();}

void Lex_analyzer::add() {buf.push_back(c);}

int Lex_analyzer::look(const std::string buf, std::string *list) {
  int i = 0;
  while(i <= 15) {
    if (buf == list[i]) {return i;}
    ++i;
  }
  return 0;
}

void Lex_analyzer::gc() {
  c = fgetc(fp);
  if (((int)c == -56) || ((int)c == 13)) {return;}
  column++;
  if (c == '\n') {
    row++;
    column = 0;
  }
}

Lex_analyzer::Lex_analyzer(const std::string f) {
  row = 0;
  column = 0;
  CS = S;
  fp = fopen(f.c_str(), "r"); 
  clear();
  gc();
}

Lex_analyzer::~Lex_analyzer(){fclose(fp);}

Lex Lex_analyzer::analyze() {
  int j, k;
  CS = S;
  std::string str;
  do {
    switch(CS) {
      case S:
        if ((c == ' ') || (c == '\n') || (c == '\t') || ((int)c == 13) || ((int)c == -56)) {gc();}
        else if (isalpha(c)) {
		  clear();
		  add();
		  gc();
		  CS = Ident;
	    }
	    else if (isdigit(c)) {
		  clear();
		  add();
		  gc();
		  CS = Number;
	    }
	    else if (c == '/') {
		  gc();
		  if (c == '*') {
			gc();
			CS = MultiCom;
		  }
		  else {return Lex(row, column, LEX_SLASH, "/");}
		}
        else if (c == '#') {
		  gc();
		  CS = Com;
		}
		else if (c == '"') {
		  gc();
                  clear();
		  CS = Quote;
		}
	    else if ((int)c == EOF) {return Lex(row, column, LEX_FIN);}
	    else {
		  clear();
		  add();
		  CS = Znak;
		}
	    break;
	  case Ident:
	    if ((isalpha(c)) || (isdigit(c))) {
	      add();
	      gc();
	    }
	    else if (c == ':') {
	      add();
	      if (j = look(buf, TW)) {
		    gc();
		    return Lex(row, column, words[j]);
		  }
	      else {
		    buf.resize(buf.size() - 1);
            TID.put(buf);
            return Lex(row, column, LEX_VAR, buf);
          }
        }
	    else if (j = look(buf, TW)) {return Lex(row, column, words[j]);}
	    else {
          TID.put(buf);
          return Lex(row, column, LEX_VAR, buf);
        }
	    break;
	  case Number:
	    if (isdigit(c)) {
		  add();
		  gc();
	    }
	    else if (c == '.') {
			add();
			gc();
			if (isdigit(c)) {CS = Double;}
			else {
			  buf.resize(buf.size() - 1);
			  return Lex(row, column, LEX_DIGIT, buf);
			}
		}
		else{return Lex(row, column, LEX_DIGIT, buf);}
	    break;
	  case Double:
	    if (isdigit(c)) {
		  add();
		  gc();
	    }
	    else{return Lex(row, column, LEX_DOUBLE, buf);}
	    break;
	  case MultiCom:
	    if (c == '*') {
	      gc();
	      if (c == '/') {
                    buf.resize(buf.size() - 1);
		    gc();
			return Lex(row, column, LEX_MULTI_COMMENT);
	      }
	    }
	    else if (c == '\\') {
		  gc();
		  if ((int)c == EOF) {throw Lex(row, column, LEX_FIN, "multi_comment");}
		  else {gc();}
		}
        else if ((int)c == EOF) {throw Lex(row, column, LEX_FIN, "multi_comment");}
        else {gc();}
        break;
      case Com:
        if (c == '\n') {
		  gc();
		  return Lex(row, column, LEX_COMMENT);
		}
        else if ((int)c == EOF) {throw Lex(row, column, LEX_COMMENT);}
        else {gc();}
        break;
      case Quote:
        if (c == '\\') {
		  add();
	      gc();
	      if ((int)c == EOF) {throw Lex(row, column, LEX_FIN, "string");}
	      else {
                add();
                gc();
              }
	    }
	    else if (c == '"') {
		  gc();
		  return Lex(row, column, LEX_STRING, buf);
		}
	    else if ((int)c == EOF) {throw Lex(row, column, LEX_FIN, "string");}
	    else {
	      add();
	      gc();
	    }
	    break;
	  case Znak:
	    if (j = look(buf, TD)){
		  gc();
                  clear();
                  add();
		  if ((TD[j] == "+") || (TD[j] == "-")) {
                    k = look(buf, TD);
	            if (TD[k] == "+") {
                      gc();
                      return Lex(row, column, LEX_UNARY_PLUS, "++");
                      }
                          else if (TD[k] == "-") {
                            gc();
                            return Lex(row, column, LEX_UNARY_MINUS, "--");
                            }
                         else {return Lex(row, column, dlms[j], TD[j]);}
			}
	           else {return Lex(row, column, dlms[j], TD[j]);}
	    }
	    else {throw Lex(row, column, LEX_FIN, buf);}
	    break;
	  default:
	    cerr << "Undefinded state: Unpossible to use lexical analyzer" << endl;
	}
  } while(true);
}
