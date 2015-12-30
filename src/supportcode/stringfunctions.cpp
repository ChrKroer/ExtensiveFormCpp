#include <sstream>
#include "stringfunctions.h"
#include <boost/tokenizer.hpp>
using boost::tokenizer;
using boost::escaped_list_separator;

typedef tokenizer<escaped_list_separator<char> > so_tokenizer;

std::vector<std::string> &StringFunctions::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> StringFunctions::Split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::vector<std::string> StringFunctions::SplitWithQuotes(const std::string &s, char delim) {
    std::vector<std::string> elems;
    so_tokenizer tok(s, escaped_list_separator<char>('\\', ' ', '\''));
    for(so_tokenizer::iterator beg=tok.begin(); beg!=tok.end(); ++beg) {
        elems.push_back(*beg);
    }

    return elems;
}
