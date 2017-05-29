#ifndef NUMBERSEQUENCETODOUBLELIST_H
#define NUMBERSEQUENCETODOUBLELIST_H

#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <fstream>

#define GCC_VERSION (__GNUC__ * 10000 \
    + __GNUC_MINOR__ * 100 \
    + __GNUC_PATCHLEVEL__)

#if GCC_VERSION >= 40900 // gcc 4.9.0
#include <regex>
#else
//  #include <boost/regex.hpp>
#endif

using std::cout;
using std::cerr;
using std::endl;

class NumberSequenceToDoubleList
{  
public:


    NumberSequenceToDoubleList(const std::string &expression);

    std::vector<double> getStdVector();
    std::string getErrorString();
    bool error();

    size_t size();

private:
    int parse( const std::string s);
    std::vector<double> sequence;

    bool _error;
    std::string errstr;
};

#endif // NUMBERSEQUENCETODOUBLELIST_H
