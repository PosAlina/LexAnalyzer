#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <inttypes.h>
#include "matrix.h"
#include "vector.h"
#include "rational_number.h"
#include "exception.h"
#include "sin_analyzer.h"

using namespace std;

//AUXILIARY FUNCTIONS
void Sin_analyzer::gl() {
  curr_lex = l_a.analyze();
  c_type = curr_lex.get_type();
  c_val = curr_lex.get_value();
  c_row = curr_lex.get_row();
  c_column = curr_lex.get_column();
}

void Sin_analyzer::err(const string b, type_of_lex a) {throw Lex(c_row, c_column, a, b);}

void Sin_analyzer::fin_prog() {
  if (c_val.empty()) {return;}
  if (!strcmp(c_val.c_str(), "multi_comment")) {err("unclosed", LEX_MULTI_COMMENT);}
  else if (!strcmp(c_val.c_str(), "string")) {err("unclosed", LEX_STRING);}
  else {err(c_val, LEX_FIN);}
}


//MAIN FUNCTIONS
void Sin_analyzer::sin_analyze() {
  S();
  cout << "Correct programm" << endl;
}

void Sin_analyzer::S() {
  com();
  if (c_type == LEX_FIN) {fin_prog();}
  else {
    if (c_type == LEX_DECLARE) {
      gl();
      declare();
    }
    switch (c_type) {
      case LEX_PROCESS:
        gl();
        process();
        break;
      case LEX_FIN:
        fin_prog();
        break;
      default:
        err("unexpected lexem: " + c_val, c_type);
    }
    if (c_type == LEX_FIN) {fin_prog();}
    else {err("unexpected lexem: " + c_val, c_type);}
  }
}

void Sin_analyzer::declare() {
  com();
  while ((c_type == LEX_MATRIX) || (c_type == LEX_VECTOR) || (c_type == LEX_RATIONAL) || (c_type == LEX_FLOAT) || (c_type == LEX_INTEGER)) {
  switch (c_type) {
    case LEX_MATRIX:
      gl();
      com();
      matrix_declaration();
      break;
    case LEX_VECTOR:
      gl();
      com();
      vector_declaration();
      break;
    case LEX_RATIONAL:
      gl();
      com();
      rational_declaration();
      break;
    case LEX_FLOAT:
      gl();
      com();
      float_declaration();
      break;
    case LEX_INTEGER:
      gl();
      com();
      integer_declaration();
      break;
    case LEX_FIN:
      fin_prog();
    }
    com();
  }
}
//Declarations of classes in the end of program

void Sin_analyzer::float_declaration() {
  Ident_Float *arguments;
  Lex temp(c_row, c_column, c_type, c_val);
  while (c_type != LEX_SEMICOLON) {
    temp.set_row(c_row);
    temp.set_column(c_column);
    temp.set_type(c_type);
    temp.set_value(c_val);
    if (c_type != LEX_VAR) {err("it is should be name of variable", c_type);}
    if (dec_var.find(c_val) != dec_var.end()) {err("redeclaration of variable " + c_val, c_type);}
	gl();
	com();
    if (c_type == LEX_OPEN_PARENTHESIS) {
	  gl();
	  constructor();
	}
    switch (constr_arg.size()) {
      case 0:
        arguments = new Ident_Float(temp.get_value(), temp.get_row(), temp.get_column());
        break;
      case 1:
        if (constr_arg[0].get_type() == LEX_DOUBLE) {
		  arguments = new Ident_Float(temp.get_value(), temp.get_row(), temp.get_column(), atof((constr_arg[0].get_value()).c_str()));
		}
	else if (constr_arg[0].get_type() == LEX_DIGIT) {
		  arguments = new Ident_Float(temp.get_value(), temp.get_row(), temp.get_column(), atoi((constr_arg[0].get_value()).c_str()));
		}
	else {err("not such float constructor", c_type);}
        break;
      case 2:
        if ((constr_arg[0].get_type() == LEX_MINUS) && (constr_arg[1].get_type() == LEX_DOUBLE)) {
		  arguments = new Ident_Float(temp.get_value(), temp.get_row(), temp.get_column(), -1*atof((constr_arg[1].get_value()).c_str()));
		}
	else if ((constr_arg[0].get_type() == LEX_MINUS) && (constr_arg[1].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Float(temp.get_value(), temp.get_row(), temp.get_column(), -1*atoi((constr_arg[1].get_value()).c_str()));
		}
	else {err("not such float constructor", c_type);}
        break;
      default:
        err("not such float constructor", c_type);
    }
    constr_arg.clear();
	dec_var[temp.get_value()] = arguments;
	if ((c_type != LEX_COMMA) && (c_type != LEX_SEMICOLON)) {err("unexpected separator in float declration: " + c_val, c_type);}
	if (c_type == LEX_COMMA) {
	  gl();
	  com();
	}
  } 
  gl();
  com();
}

void Sin_analyzer::integer_declaration() {
  Ident_Integer *arguments;
  Lex temp(c_row, c_column, c_type, c_val);
  while (c_type != LEX_SEMICOLON) {
	temp.set_row(c_row);
	temp.set_column(c_column);
	temp.set_type(c_type);
	temp.set_value(c_val);
    if (c_type != LEX_VAR) {err("it is should be name of variable", c_type);}
	if (dec_var.find(c_val) != dec_var.end()) {err("redeclaration of variable " + c_val, c_type);}
	gl();
	com();
	if (c_type == LEX_OPEN_PARENTHESIS) {
	  gl();
	  constructor();
	}
    switch (constr_arg.size()) {
      case 0:
        arguments = new Ident_Integer(temp.get_value(), temp.get_row(), temp.get_column());
        break;
      case 1:
        if (constr_arg[0].get_type() == LEX_DIGIT) {
		  arguments = new Ident_Integer(temp.get_value(), temp.get_row(), temp.get_column(), atoi((constr_arg[0].get_value()).c_str()));
		}
		else {err("not such integer constructor", c_type);}
        break;
     case 2:
        if ((constr_arg[0].get_type() == LEX_MINUS) && (constr_arg[1].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Integer(temp.get_value(), temp.get_row(), temp.get_column(), -1*atoi((constr_arg[1].get_value()).c_str()));
		}
		else {err("not such integer constructor", c_type);}
        break;
      default:
        err("not such integer constructor", c_type);
    }
    constr_arg.clear();
	dec_var[temp.get_value()] = arguments;
	if ((c_type != LEX_COMMA) && (c_type != LEX_SEMICOLON)) {err("unexpected separator in integer declration: " + c_val, c_type);}
	if (c_type == LEX_COMMA) {
	  gl();
	  com();
	}
  } 
  gl();
  com();
}

void Sin_analyzer::constructor() {
  com();
  if (c_type == LEX_CLOSE_PARENTHESIS) {
	gl();
	com();
    return;
  }
  Lex temp(c_row, c_column, c_type, c_val);
  std::string arg;
  while(c_type != LEX_CLOSE_PARENTHESIS) {
	temp.set_row(c_row);
	temp.set_column(c_column);
	temp.set_type(c_type);
	temp.set_value(c_val);
	arg.clear();
    if (c_type == LEX_MINUS) {
      gl();
	  com();
      switch (c_type) {
        case LEX_DOUBLE:
          arg.push_back('-');
        temp.set_value(arg);
	constr_arg.push_back(temp);
        temp.set_row(c_row);
	temp.set_column(c_column);
	temp.set_type(c_type);
	temp.set_value(c_val);
	arg.clear();
          arg = c_val;
          temp.set_type(LEX_DOUBLE);
          gl();
		  com();
          break;
        case LEX_DIGIT:
                  arg.push_back('-');
        temp.set_value(arg);
	constr_arg.push_back(temp);
        temp.set_row(c_row);
	temp.set_column(c_column);
	temp.set_type(c_type);
	temp.set_value(c_val);
	arg.clear();
		  arg = c_val;
          temp.set_type(LEX_DIGIT);
          gl();
		  com();
          if (c_type == LEX_SLASH) {
        temp.set_value(arg);
	constr_arg.push_back(temp);
        temp.set_row(c_row);
	temp.set_column(c_column);
	temp.set_type(c_type);
	temp.set_value(c_val);
	arg.clear();
            arg.push_back('/');
            gl();
			com();
            if (c_type == LEX_MINUS) {
                      temp.set_value(arg);
	constr_arg.push_back(temp);
        temp.set_row(c_row);
	temp.set_column(c_column);
	temp.set_type(c_type);
	temp.set_value(c_val);
	arg.clear();
              arg.push_back('-');
              gl();
			  com();
             }
            if (c_type != LEX_DIGIT) {err("it is not integer number in integer constructor", c_type);}
        temp.set_value(arg);
	constr_arg.push_back(temp);
        temp.set_row(c_row);
	temp.set_column(c_column);
	temp.set_type(c_type);
	temp.set_value(c_val);
	arg.clear();
              arg = c_val;
              temp.set_type(LEX_DIGIT);
              gl();
			  com();
            }
          break;
        default:
		  err("it is not number", c_type);
      }
    }
    else {
      switch (c_type) {
        case LEX_DOUBLE:
          arg = c_val;
          temp.set_type(LEX_DIGIT); 
          gl();
		  com();
          break;
        case LEX_DIGIT:
		  arg = c_val;
          temp.set_type(LEX_DIGIT);
          gl();
		  com();
          if (c_type == LEX_SLASH) {
        temp.set_value(arg);
	constr_arg.push_back(temp);
        temp.set_row(c_row);
	temp.set_column(c_column);
	temp.set_type(c_type);
	temp.set_value(c_val);
	arg.clear();
            arg.push_back('/');
            gl();
			com();
            if (c_type == LEX_MINUS) {
        temp.set_value(arg);
	constr_arg.push_back(temp);
        temp.set_row(c_row);
	temp.set_column(c_column);
	temp.set_type(c_type);
	temp.set_value(c_val);
	arg.clear();
              arg.push_back('-');
              gl();
			  com();
             }
            if (c_type != LEX_DIGIT) {err("it is not integer number in integer constructor", c_type);}
        temp.set_value(arg);
	constr_arg.push_back(temp);
        temp.set_row(c_row);
	temp.set_column(c_column);
	temp.set_type(c_type);
	temp.set_value(c_val);
	arg.clear();
              arg = c_val;
              temp.set_type(LEX_DIGIT);
              gl();
			  com();
          }
          break;
		case LEX_STRING:
		  arg = c_val;
		  temp.set_type(LEX_STRING);
		  gl();
		  com();
                  break;
        default:
		  err("it is not number or string", c_type);
      }
    }
    temp.set_value(arg);
	constr_arg.push_back(temp);
	if (c_type == LEX_CLOSE_PARENTHESIS) {
	  gl();
	  com();
	  return;
	}
    if (c_type != LEX_COMMA) {err("uncorrect separator", c_type);}
    gl();
	com();
  }
}

//EXPRESSIONS IN PROCESS
void Sin_analyzer::process() {
  waste();
  while (c_type != LEX_FIN) {
	while (c_type == LEX_SEMICOLON) {
	  gl();
	  waste();
	}  
	expression();
        waste();
  }
  fin_prog();
}

void Sin_analyzer::expression() {
  waste();
  fin_exp();
  while ((c_type != LEX_FIN) && (c_type != LEX_SEMICOLON)) {
    unar();
	sign();
	fin_exp();
    if (c_type == LEX_OPEN_PARENTHESIS) {
      gl();
      parenth();
	  fin_exp();
	  gl();
      unar();
	  while ((c_type == LEX_COLON) || (c_type == LEX_OPEN_SQUARE_BRACKET)) {
        if (c_type == LEX_COLON) {
          gl();
	      waste();
          func();
  	    }
	    else {
	  	  gl();
          waste();
          square_brackets();
          if (c_type != LEX_CLOSE_SQUARE_BRACKET) {err("unclosed", LEX_CLOSE_SQUARE_BRACKET);}
          gl();
          waste();
        }
	    unar();
        if (c_type == LEX_SEMICOLON) {
		  gl();
                  waste();
		  return;
		}
	    fin_exp();
      }
     waste();
      znak();
	  gl();
	  waste();
      if (c_type == LEX_SEMICOLON) {err("it is should be variable", c_type);}
	}
	else {
	  var();
	  unar();
	  if (c_type == LEX_SEMICOLON) {
		gl();
		return;
	  }
	  while ((c_type == LEX_COLON) || (c_type == LEX_OPEN_SQUARE_BRACKET)) {
        if (c_type == LEX_COLON) {
          gl();
	      waste();
          func();
  	    }
	    else {
	  	  gl();
          waste();
          square_brackets();
          if (c_type != LEX_CLOSE_SQUARE_BRACKET) {err("unclosed", LEX_CLOSE_SQUARE_BRACKET);}
          gl();
        }
	    unar();
        if (c_type == LEX_SEMICOLON) {
		  gl();
		  return;
		}
	    fin_exp();
      }
      waste();
      znak();
	  gl();
	  waste();
      if (c_type == LEX_SEMICOLON) {err("it is should be variable", c_type);}
	}
  }
}

//UNREADABLE EXPRESSIONS
void Sin_analyzer::fin_exp() {
  if (c_type == LEX_FIN) {err("last expression did not finished", c_type);}
}

void Sin_analyzer::info() {
  gl();
  com();
  if (c_type != LEX_OPEN_PARENTHESIS) {err("error open parenthesis of info()", c_type);}
  gl();
  com();
  if (c_type != LEX_STRING) {err("error string in info()", c_type);}
  gl();
  com();
  if (c_type != LEX_CLOSE_PARENTHESIS) {err("error close parenthesis of info()", c_type);}
  gl();
  com();
}

void Sin_analyzer::com() {while ((c_type == LEX_COMMENT) || (c_type == LEX_MULTI_COMMENT)) {gl();}}

void Sin_analyzer::waste() {
  com();
  while (c_type == LEX_INFO) {
	info();
	com();
  }
}

//WORK WITH CLASS FUNCTIONS
void Sin_analyzer::func() {
  Lex temp(c_row, c_column, c_type, "error open parenthesis in function");
  switch (c_type) {
    case LEX_MAKE_CANONICAL:
    case LEX_ROTATE:
    case LEX_PRINT:
      break;
    case LEX_ROW:
    case LEX_COLUMN:
	  gl();
	  waste();
      if (c_type != LEX_OPEN_PARENTHESIS) {throw temp;}
	  parenth();
	  break;
    case LEX_READ:
    case LEX_WRITE:
	  gl();
      waste();
      if (c_type != LEX_OPEN_PARENTHESIS) {throw temp;}
      gl();
      waste();
      if (c_type != LEX_STRING) {
		temp.set_value("error argument of function");
		throw temp;
	  }
      gl();
      waste();
      if (c_type != LEX_CLOSE_PARENTHESIS) {
		temp.set_value("error close parenthesis in function");
		throw temp;
	  }
    break;
    default:
      err("unexpected function: " + c_val, c_type);
  }
  gl();
}

//DETERMINER OF LEXEMS
void Sin_analyzer::unar() {
  waste();
  while ((c_type == LEX_UNARY_MINUS) || (c_type == LEX_UNARY_PLUS)) {
	gl();
	waste();
  }
}

void Sin_analyzer::sign() {
  waste();
  if ((c_type == LEX_MINUS) || (c_type == LEX_PLUS)) {
	gl();
	waste();
  }
}

void Sin_analyzer::var() {
  waste();
  switch (c_type) {
    case LEX_VAR:
      if (dec_var.find(c_val) == dec_var.end()) {err("This variable did not declare: " + c_val, c_type);}
    case LEX_DIGIT:
    case LEX_DOUBLE:
      gl();
	  waste();
	  break;
    default:
	  err("it is should be variable", c_type);
    }
}

void Sin_analyzer::znak() {
  switch (c_type) {
    case LEX_EQUAL:
    case LEX_SLASH:
    case LEX_MULTI:
    case LEX_MINUS:
	case LEX_PLUS:
    case LEX_POWER:
      break;
    default:
      err("it is not znak", c_type);
  }
}

//PARENTHESIS
void Sin_analyzer::parenth() {
  waste();
  if (c_type == LEX_CLOSE_PARENTHESIS) {err("not argument in parenthesis", c_type);}
  while (c_type != LEX_FIN) {
    waste();
    unar();
	sign();
    if (c_type == LEX_OPEN_PARENTHESIS) {
      gl();
      parenth();
      if (c_type != LEX_CLOSE_PARENTHESIS) {err("unclose parenthesis", LEX_CLOSE_PARENTHESIS);}
      gl();
      waste();
    }
    else {waste();var();}
    waste();
    if (c_type == LEX_CLOSE_PARENTHESIS) {return;}
	unar();
	while ((c_type == LEX_COLON) || (c_type == LEX_OPEN_SQUARE_BRACKET)) {
      if (c_type == LEX_COLON) {
        gl();
	    waste();
        func();
  	  }
	  else {
		gl();
        waste();
        square_brackets();
        if (c_type != LEX_CLOSE_SQUARE_BRACKET) {err("unclosed", LEX_CLOSE_SQUARE_BRACKET);}
        gl();
      }
	  unar();
      if (c_type == LEX_CLOSE_PARENTHESIS) {return;}
    }
    waste();
    znak();
	gl();
	waste();
    if (c_type == LEX_CLOSE_PARENTHESIS) {err("it is should be variable", c_type);}
  }
}

void Sin_analyzer::multi_parenth() {
  waste();
  if (c_type == LEX_CLOSE_PARENTHESIS) {err("not argument in parenthesis", c_type);}
  while (c_type != LEX_FIN) {
    waste();
    unar();
	sign();
    if (c_type == LEX_OPEN_PARENTHESIS) {
      gl();
      parenth();
      if (c_type != LEX_CLOSE_PARENTHESIS) {err("unclose parenthesis", LEX_CLOSE_PARENTHESIS);}
      gl();
      waste();
    }
    else {var();}
    if ((c_type == LEX_CLOSE_PARENTHESIS) || (c_type == LEX_COMMA)) {return;}
	unar();
	while ((c_type == LEX_COLON) || (c_type == LEX_OPEN_SQUARE_BRACKET)) {
      if (c_type == LEX_COLON) {
        gl();
	    waste();
        func();
  	  }
	  else {
		gl();
        waste();
        square_brackets();
        if (c_type != LEX_CLOSE_SQUARE_BRACKET) {err("unclosed", LEX_CLOSE_SQUARE_BRACKET);}
        gl();
      }
	  unar();
      if ((c_type == LEX_CLOSE_PARENTHESIS) || (c_type == LEX_COMMA)) {return;}
    }
    waste();
    znak();
	gl();
	waste();
    if ((c_type == LEX_CLOSE_PARENTHESIS) || (c_type == LEX_COMMA)) {err("it is should be variable", c_type);}
  }
}

//SQUARE BRACKETS
void Sin_analyzer::square_brackets() {
  waste();
  if (c_type == LEX_CLOSE_SQUARE_BRACKET) {err("not argument in square_brackets", c_type);}
  while ((c_type != LEX_CLOSE_SQUARE_BRACKET) && (c_type != LEX_FIN)) {
    waste();
    unar();
	sign();
    if (c_type == LEX_OPEN_PARENTHESIS) {
      gl();
      waste();
	  parenth();
	  if (c_type != LEX_CLOSE_PARENTHESIS) {err("unclosed", LEX_CLOSE_PARENTHESIS);}
      gl();
      waste();
    }
    else {
      var();
    }
	unar();
	while ((c_type == LEX_COLON) || (c_type == LEX_OPEN_SQUARE_BRACKET)) {
      if (c_type == LEX_COLON) {
        gl();
	    waste();
        func();
  	  }
	  else {
		gl();
        waste();
        square_brackets();
        if (c_type != LEX_CLOSE_SQUARE_BRACKET) {err("unclosed", LEX_CLOSE_SQUARE_BRACKET);}
        gl();
      }
    }
    if (c_type == LEX_CLOSE_SQUARE_BRACKET) {return;}
    if (c_type == LEX_COMMA) {gl();}
    else {znak();}
  }
}

//DECLARATIONS OF CLASSES
void Sin_analyzer::matrix_declaration() {
  Ident_Matrix *arguments;
  Lex temp(c_row, c_column, c_type, c_val);
  while (c_type != LEX_SEMICOLON) {
    temp.set_row(c_row);
    temp.set_column(c_column);
    temp.set_type(c_type);
    temp.set_value(c_val);
    if (c_type != LEX_VAR) {err("it is should be name of variable", c_type);}
    if (dec_var.find(c_val) != dec_var.end()) {err("redeclaration of variable " + c_val, c_type);}
      gl();
      com();
      if (c_type == LEX_OPEN_PARENTHESIS) {
        gl();
	constructor();
      }
    switch (constr_arg.size()) {
      case 0:
        arguments = new Ident_Matrix(temp.get_value(), temp.get_row(), temp.get_column());
        break;
      case 1:
        if (constr_arg[0].get_type() == LEX_STRING) { //From the file
arguments = new Ident_Matrix(temp.get_value(), temp.get_row(), temp.get_column(), Matrix(0, 0, 0));
		}
        else {err("not such matrix constructor", LEX_VAR);}
        break;
      case 2:
        if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_DIGIT)) { //Zero matrix
          arguments = new Ident_Matrix(temp.get_value(), temp.get_row(), temp.get_column(), Matrix(atoi((constr_arg[0].get_value()).c_str())));
        }
        else {err("not such matrix constructor", LEX_VAR);}
        break;
      case 3:
        if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_DIGIT) && (constr_arg[2].get_type() == LEX_DIGIT)) {
          arguments = new Ident_Matrix(temp.get_value(), temp.get_row(), temp.get_column(), Matrix(atoi((constr_arg[0].get_value()).c_str()),
		  atoi((constr_arg[1].get_value()).c_str()), atoi((constr_arg[2].get_value()).c_str())));
        }
        else {err("not such matrix constructor", LEX_VAR);}
        break;
      case 4:
        if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_DIGIT) && (constr_arg[2].get_type() == LEX_MINUS) && (constr_arg[3].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Matrix(temp.get_value(), temp.get_row(), temp.get_column(), Matrix(atoi((constr_arg[0].get_value()).c_str()),
		  atoi((constr_arg[1].get_value()).c_str()), -1*atoi((constr_arg[2].get_value()).c_str())));
		}
        else if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_DIGIT) && (constr_arg[2].get_type() == LEX_DIGIT) && (constr_arg[3].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Matrix(temp.get_value(), temp.get_row(), temp.get_column(), Matrix(atoi((constr_arg[0].get_value()).c_str()),
		  atoi((constr_arg[1].get_value()).c_str()), Rational_number(atoi((constr_arg[2].get_value()).c_str()), atoi((constr_arg[3].get_value()).c_str()))));
		}
	else {err("not such matrix constructor", c_type);}
        break;
      case 5:
        if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_DIGIT) && (constr_arg[2].get_type() == LEX_DIGIT) && (constr_arg[3].get_type() == LEX_SLASH) && (constr_arg[4].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Matrix(temp.get_value(), temp.get_row(), temp.get_column(), Matrix(atoi((constr_arg[0].get_value()).c_str()),
		  atoi((constr_arg[2].get_value()).c_str()), Rational_number(atoi((constr_arg[3].get_value()).c_str()), atoi((constr_arg[4].get_value()).c_str()))));
		}
         else if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_DIGIT) && (constr_arg[2].get_type() == LEX_DIGIT) && (constr_arg[3].get_type() == LEX_MINUS) && (constr_arg[4].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Matrix(temp.get_value(), temp.get_row(), temp.get_column(), Matrix(atoi((constr_arg[0].get_value()).c_str()),
		  atoi((constr_arg[1].get_value()).c_str()), Rational_number(atoi((constr_arg[2].get_value()).c_str()), -1*atoi((constr_arg[4].get_value()).c_str()))));
		}
else if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_DIGIT) && (constr_arg[2].get_type() == LEX_MINUS) && (constr_arg[3].get_type() == LEX_DIGIT) && (constr_arg[4].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Matrix(temp.get_value(), temp.get_row(), temp.get_column(), Matrix(atoi((constr_arg[0].get_value()).c_str()),
		  atoi((constr_arg[1].get_value()).c_str()), Rational_number(-1*atoi((constr_arg[3].get_value()).c_str()), atoi((constr_arg[4].get_value()).c_str()))));
		}
	else {err("not such matrix constructor", c_type);}
        break;
      case 6:
        if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_DIGIT) &&(constr_arg[2].get_type() == LEX_DIGIT) && (constr_arg[3].get_type() == LEX_SLASH) && (constr_arg[4].get_type() == LEX_MINUS) && (constr_arg[5].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Matrix(temp.get_value(), temp.get_row(), temp.get_column(), Matrix(atoi((constr_arg[0].get_value()).c_str()),
		  atoi((constr_arg[1].get_value()).c_str()), Rational_number(atoi((constr_arg[2].get_value()).c_str()), -1*atoi((constr_arg[5].get_value()).c_str()))));
}
        else if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_DIGIT) &&(constr_arg[2].get_type() == LEX_MINUS) && (constr_arg[3].get_type() == LEX_DIGIT) && (constr_arg[4].get_type() == LEX_SLASH) && (constr_arg[5].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Matrix(temp.get_value(), temp.get_row(), temp.get_column(), Matrix(atoi((constr_arg[0].get_value()).c_str()),
		  atoi((constr_arg[1].get_value()).c_str()), Rational_number(-1*atoi((constr_arg[3].get_value()).c_str()), atoi((constr_arg[5].get_value()).c_str()))));
		}
        else if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_DIGIT) &&(constr_arg[2].get_type() == LEX_MINUS) && (constr_arg[3].get_type() == LEX_DIGIT) && (constr_arg[4].get_type() == LEX_MINUS) && (constr_arg[5].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Matrix(temp.get_value(), temp.get_row(), temp.get_column(), Matrix(atoi((constr_arg[0].get_value()).c_str()),
		  atoi((constr_arg[1].get_value()).c_str()), Rational_number(-1*atoi((constr_arg[3].get_value()).c_str()), -1*atoi((constr_arg[5].get_value()).c_str()))));
		}
	else {err("not such matrix constructor", c_type);}
        break;
      case 7:
        if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_DIGIT) &&(constr_arg[2].get_type() == LEX_MINUS) && (constr_arg[3].get_type() == LEX_DIGIT) && (constr_arg[4].get_type() == LEX_SLASH) && (constr_arg[5].get_type() == LEX_MINUS) && (constr_arg[6].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Matrix(temp.get_value(), temp.get_row(), temp.get_column(), Matrix(atoi((constr_arg[0].get_value()).c_str()),
		  atoi((constr_arg[1].get_value()).c_str()), Rational_number(-1*atoi((constr_arg[3].get_value()).c_str()), -1*atoi((constr_arg[6].get_value()).c_str()))));
}
	else {err("not such matrix constructor", c_type);}
        break;
      default:
        err("not such matrix constructor", LEX_VAR);
    }
    constr_arg.clear();
	dec_var[temp.get_value()] = arguments;
	if ((c_type != LEX_COMMA) && (c_type != LEX_SEMICOLON)) {err("unexpected separator in matrix declration: " + c_val, c_type);}
	if (c_type == LEX_COMMA) {
	  gl();
	  com();
	}
  } 
  gl();
  com();
}

void Sin_analyzer::vector_declaration() {
  Ident_Vector *arguments;
  Lex temp(c_row, c_column, c_type, c_val);
  while (c_type != LEX_SEMICOLON) {
    temp.set_row(c_row);
    temp.set_column(c_column);
    temp.set_type(c_type);
    temp.set_value(c_val);
    if (c_type != LEX_VAR) {err("it is should be name of variable", c_type);}
	if (dec_var.find(c_val) != dec_var.end()) {err("redeclaration of variable " + c_val, c_type);}
	gl();
	com();
	if (c_type == LEX_OPEN_PARENTHESIS) {
	  gl();
	  constructor();
	}
    switch (constr_arg.size()) {
      case 0:
        arguments = new Ident_Vector(temp.get_value(), temp.get_row(), temp.get_column());
        break;
      case 1:
        if (constr_arg[0].get_type() == LEX_STRING) { //From the file
		  arguments = new Ident_Vector(temp.get_value(), temp.get_row(), temp.get_column(), Vector(0, 0));
		}
        else {err("not such vector constructor", c_type);}
        break;
      case 2:
        if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_DIGIT)) {
          arguments = new Ident_Vector(temp.get_value(), temp.get_row(), temp.get_column(), Vector(atoi((constr_arg[0].get_value()).c_str()), atoi((constr_arg[1].get_value()).c_str())));
        }
        else {err("not such vector constructor", c_type);}
        break;
      case 3:
        if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_MINUS) && (constr_arg[2].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Vector(temp.get_value(), temp.get_row(), temp.get_column(), Vector(atoi((constr_arg[0].get_value()).c_str()), -1*atoi((constr_arg[2].get_value()).c_str())));
		}
        else if ((constr_arg[0].get_type() == LEX_DIGIT) &&(constr_arg[1].get_type() == LEX_DIGIT) && (constr_arg[2].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Vector(temp.get_value(), temp.get_row(), temp.get_column(), Vector(atoi((constr_arg[0].get_value()).c_str()), Rational_number(atoi((constr_arg[1].get_value()).c_str()), atoi((constr_arg[2].get_value()).c_str()))));
		}
	else {err("not such vector constructor", c_type);}
        break;
      case 4:
        if ((constr_arg[0].get_type() == LEX_DIGIT) &&(constr_arg[1].get_type() == LEX_DIGIT) && (constr_arg[2].get_type() == LEX_SLASH) && (constr_arg[3].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Vector(temp.get_value(), temp.get_row(), temp.get_column(), Vector(atoi((constr_arg[0].get_value()).c_str()), Rational_number(atoi((constr_arg[1].get_value()).c_str()), atoi((constr_arg[3].get_value()).c_str()))));
		}
         else if ((constr_arg[0].get_type() == LEX_DIGIT) &&(constr_arg[1].get_type() == LEX_DIGIT) && (constr_arg[2].get_type() == LEX_MINUS) && (constr_arg[3].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Vector(temp.get_value(), temp.get_row(), temp.get_column(), Vector(atoi((constr_arg[0].get_value()).c_str()), Rational_number(atoi((constr_arg[1].get_value()).c_str()), -1*atoi((constr_arg[3].get_value()).c_str()))));
		}
else if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_MINUS) && (constr_arg[2].get_type() == LEX_DIGIT) && (constr_arg[3].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Vector(temp.get_value(), temp.get_row(), temp.get_column(), Vector(atoi((constr_arg[0].get_value()).c_str()), Rational_number(-1*atoi((constr_arg[2].get_value()).c_str()), atoi((constr_arg[3].get_value()).c_str()))));
		}
	else {err("not such vector constructor", c_type);}
        break;
      case 5:
        if ((constr_arg[0].get_type() == LEX_DIGIT) &&(constr_arg[1].get_type() == LEX_DIGIT) && (constr_arg[2].get_type() == LEX_SLASH) && (constr_arg[3].get_type() == LEX_MINUS) && (constr_arg[4].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Vector(temp.get_value(), temp.get_row(), temp.get_column(), Vector(atoi((constr_arg[0].get_value()).c_str()), Rational_number(atoi((constr_arg[1].get_value()).c_str()), -1*atoi((constr_arg[4].get_value()).c_str()))));
}
        else if ((constr_arg[0].get_type() == LEX_DIGIT) &&(constr_arg[1].get_type() == LEX_MINUS) && (constr_arg[2].get_type() == LEX_DIGIT) && (constr_arg[3].get_type() == LEX_SLASH) && (constr_arg[4].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Vector(temp.get_value(), temp.get_row(), temp.get_column(), Vector(atoi((constr_arg[0].get_value()).c_str()), Rational_number(-1*atoi((constr_arg[2].get_value()).c_str()), atoi((constr_arg[4].get_value()).c_str()))));
		}
        else if ((constr_arg[0].get_type() == LEX_DIGIT) &&(constr_arg[1].get_type() == LEX_MINUS) && (constr_arg[2].get_type() == LEX_DIGIT) && (constr_arg[3].get_type() == LEX_MINUS) && (constr_arg[4].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Vector(temp.get_value(), temp.get_row(), temp.get_column(), Vector(atoi((constr_arg[0].get_value()).c_str()), Rational_number(-1*atoi((constr_arg[2].get_value()).c_str()), -1*atoi((constr_arg[4].get_value()).c_str()))));
		}
	else {err("not such vector constructor", c_type);}
        break;
      case 6:
        if ((constr_arg[0].get_type() == LEX_DIGIT) &&(constr_arg[1].get_type() == LEX_MINUS) && (constr_arg[2].get_type() == LEX_DIGIT) && (constr_arg[3].get_type() == LEX_SLASH) && (constr_arg[4].get_type() == LEX_MINUS) && (constr_arg[5].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Vector(temp.get_value(), temp.get_row(), temp.get_column(), Vector(atoi((constr_arg[0].get_value()).c_str()), Rational_number(-1*atoi((constr_arg[2].get_value()).c_str()), -1*atoi((constr_arg[5].get_value()).c_str()))));
}
	else {err("not such vector constructor", c_type);}
        break;
      default:
        err("not such vector constructor", c_type);
    }
    constr_arg.clear();
	dec_var[temp.get_value()] = arguments;
	if ((c_type != LEX_COMMA) && (c_type != LEX_SEMICOLON)) {err("unexpected separator in vector declration: " + c_val, c_type);}
	if (c_type == LEX_COMMA) {
	  gl();
	  com();
	}
  } 
  gl();
  com();
}

void Sin_analyzer::rational_declaration() {
  Ident_Rational *arguments;
  Lex temp(c_row, c_column, c_type, c_val);
  while (c_type != LEX_SEMICOLON) {
    temp.set_row(c_row);
    temp.set_column(c_column);
    temp.set_type(c_type);
    temp.set_value(c_val);
    if (c_type != LEX_VAR) {err("it is should be name of variable", c_type);}
	if (dec_var.find(c_val) != dec_var.end()) {err("redeclaration of variable " + c_val, c_type);}
	gl();
	com();
	if (c_type == LEX_OPEN_PARENTHESIS) {
	  gl();
	  constructor();
	}
    switch (constr_arg.size()) {
      case 0:
        arguments = new Ident_Rational(temp.get_value(), temp.get_row(), temp.get_column());
        break;
      case 1:
        if (constr_arg[0].get_type() == LEX_DIGIT) {
		  arguments = new Ident_Rational(temp.get_value(), temp.get_row(), temp.get_column(), Rational_number(atoi((constr_arg[0].get_value()).c_str())));
		}
	else {err("not such rational constructor", c_type);}
        break;
      case 2:
        if ((constr_arg[0].get_type() == LEX_MINUS) && (constr_arg[1].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Rational(temp.get_value(), temp.get_row(), temp.get_column(), Rational_number(-1*atoi((constr_arg[1].get_value()).c_str())));
		}
        else if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Rational(temp.get_value(), temp.get_row(), temp.get_column(), Rational_number(atoi((constr_arg[0].get_value()).c_str()), atoi((constr_arg[1].get_value()).c_str())));
		}
	else {err("not such rational constructor", c_type);}
        break;
      case 3:
        if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_SLASH) && (constr_arg[2].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Rational(temp.get_value(), temp.get_row(), temp.get_column(), Rational_number(atoi((constr_arg[0].get_value()).c_str()), atoi((constr_arg[2].get_value()).c_str())));
		}
         else if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_MINUS) && (constr_arg[2].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Rational(temp.get_value(), temp.get_row(), temp.get_column(), Rational_number(atoi((constr_arg[0].get_value()).c_str()), -1*atoi((constr_arg[2].get_value()).c_str())));
		}
else if ((constr_arg[0].get_type() == LEX_MINUS) && (constr_arg[1].get_type() == LEX_DIGIT) && (constr_arg[2].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Rational(temp.get_value(), temp.get_row(), temp.get_column(), Rational_number(-1*atoi((constr_arg[1].get_value()).c_str()), atoi((constr_arg[2].get_value()).c_str())));
		}
	else {err("not such rational constructor", c_type);}
        break;
      case 4:
        if ((constr_arg[0].get_type() == LEX_DIGIT) && (constr_arg[1].get_type() == LEX_SLASH) && (constr_arg[2].get_type() == LEX_MINUS) && (constr_arg[3].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Rational(temp.get_value(), temp.get_row(), temp.get_column(), Rational_number(atoi((constr_arg[0].get_value()).c_str()), -1*atoi((constr_arg[3].get_value()).c_str())));
}
        else if ((constr_arg[0].get_type() == LEX_MINUS) && (constr_arg[1].get_type() == LEX_DIGIT) && (constr_arg[2].get_type() == LEX_SLASH) && (constr_arg[3].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Rational(temp.get_value(), temp.get_row(), temp.get_column(), Rational_number(-1*atoi((constr_arg[1].get_value()).c_str()), atoi((constr_arg[3].get_value()).c_str())));
		}
        else if ((constr_arg[0].get_type() == LEX_MINUS) && (constr_arg[1].get_type() == LEX_DIGIT) && (constr_arg[2].get_type() == LEX_MINUS) && (constr_arg[3].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Rational(temp.get_value(), temp.get_row(), temp.get_column(), Rational_number(-1*atoi((constr_arg[1].get_value()).c_str()), -1*atoi((constr_arg[3].get_value()).c_str())));
		}
	else {err("not such rational constructor", c_type);}
        break;
      case 5:
        if ((constr_arg[0].get_type() == LEX_MINUS) && (constr_arg[1].get_type() == LEX_DIGIT) && (constr_arg[2].get_type() == LEX_SLASH) && (constr_arg[3].get_type() == LEX_MINUS) && (constr_arg[4].get_type() == LEX_DIGIT)) {
		  arguments = new Ident_Rational(temp.get_value(), temp.get_row(), temp.get_column(), Rational_number(-1*atoi((constr_arg[1].get_value()).c_str()), -1*atoi((constr_arg[4].get_value()).c_str())));
}
	else {err("not such rational constructor", c_type);}
        break;
      default:
        err("not such rational constructor", c_type);
    }
    constr_arg.clear();
	dec_var[temp.get_value()] = arguments;
	if ((c_type != LEX_COMMA) && (c_type != LEX_SEMICOLON)) {err("unexpected separator in rational declration: " + c_val, c_type);}
	if (c_type == LEX_COMMA) {
	  gl();
	  com();
	}
  } 
  gl();
  com();
}

