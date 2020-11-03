#include <iostream>
#include <string>
#include "sin_analyzer.h"

using namespace std;

int main(int argc, char **argv) {
  if (argc < 2) {
    cerr << "Not enough arguments" << endl;
    return 1;
  }
  try{
/*
    Lex_analyzer SA(argv[1]);
    Lex current = SA.analyze();
    while (current.get_type() != LEX_FIN) {
      cerr << current;
      current = SA.analyze();
    }
    cerr << current;
*/
    Sin_analyzer sin(argv[1]);
    sin.sin_analyze();
  }
  catch (const Lex &ex) {
    cerr << ex << endl;
    return 1;
  }
  catch (const Rational_number_exception &ex) {
    cerr << ex.get_message() << endl;
    return 1;
  }
  catch (const Vector_exception &ex) {
    cerr << ex.get_message() << endl;
    return 1;
  }
  catch (const Matrix_exception &ex) {
    cerr << ex.get_message() << endl;
    return 1;
  }
  catch (...) {
    cerr << "unexpected error" << endl;
	return 1;
  }
  return 0;
}
