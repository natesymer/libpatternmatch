#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <iostream>
#include <list>
#include <map>

namespace LibPM {
  using namespace std;
  
  class pattern {
  public:
    pattern(const char *ptrn) { create(string(ptrn)); }
    pattern(string ptrn) { create(ptrn); }
    
    bool operator==(const string &rhs) const;
    bool operator!=(const string &rhs) const;
  
    // returns true if there are any wilcards/splats/etc in this
    bool is_dynamic() const;
    
    // check if a string matches the this pattern
    bool matches(string comp) const;
    
    // extract the wildcards in comp into a map
    map<string, string> match_wildcards(string comp) const;
    
    // extract the splats in comp into a list
    list<string> match_splats(string comp) const;
  
  private:
    string _pattern;
    list<unsigned> _splat_indeces;
    list<unsigned> _wildcard_indeces;
  
    void create(string ptrn);
    
    // advance ptr to the next occurance of str
    unsigned _advance_to_str(char *&ptr, char *str, unsigned len) const;
    
    // advance ptr to the next occurance of c
    unsigned _advance_to_char(char *&ptr, char c) const;

    // advance to first char of wildcard sequence
    unsigned _advance_to_wildcard(char *&ptr) const;
  
    // advance ptr to the first char after a wildcard sequence
    unsigned _advance_to_succ(char *&ptr) const;
  
    // compares a string to a pattern
    bool _ptrncmp(char *pattern, char *comp) const;
  
    list<string> _get_splats(char *pattern, char *comp) const;
    map<string, string> _get_wildcards(char *pattern, char *comp) const;
    list<unsigned> _get_splat_indeces_prime(char *str, char *ptr) const; 
    inline list<unsigned> _get_splat_indeces(char *str) const { return _get_splat_indeces_prime(str,str); }
    list<unsigned> _get_wildcard_indeces_prime(char *str, char *ptr) const;
    inline list<unsigned> _get_wildcard_indeces(char *str) const { return _get_wildcard_indeces_prime(str,str); }
    
    string _read_until_wildcard(char *ptr) const;
    string _read_until(char *ptr, char c) const;
  };
}
