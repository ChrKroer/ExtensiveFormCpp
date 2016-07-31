#ifndef STRING_FUNCTIONS_H
#define STRING_FUNCTIONS_H
#include <string>
#include <vector>

class StringFunctions {
public:
  static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
  static std::vector<std::string> Split(const std::string &s, char delim=' ');
  static std::vector<std::string> SplitWithQuotes(const std::string &s, char delim=' ');
  static bool is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(),
                                      [](char c)
                                      { return std::isdigit(c) == 0; }) == s.end();
  }
};

#endif // STRING_FUNCTIONS_H
