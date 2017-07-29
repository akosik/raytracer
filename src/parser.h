#include "variables.h"
#include "Transform.h"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stack>

bool readvals (std::stringstream &s, const int numvals, float * values) ;
void readfile (const char * filename) ;
