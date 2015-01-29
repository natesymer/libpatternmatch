#include "pattern.h"

// TODO: Match last occurance of the 'succ' string when reading wildcards

//
// Public
//

bool pattern::is_dynamic() {
  if (_wildcard_indeces.size() > 0) return true;
  if (_splat_indeces.size() > 0) return true;
  return false;
}

bool pattern::matches(string comp) { 
  if (_pattern[0] == ':') return true;
  return _ptrncmp((char *)_pattern.c_str(), (char *)comp.c_str());
}

map<string, string> pattern::match_wildcards(string comp) {
  if (_pattern[0] == ':') {
    map<string, string> m;
    m[_pattern.substr(1, _pattern.length()-1)] = comp;
    return m;
  }
  
  map<string, string> m;
  char *pattern = (char *)_pattern.c_str();
  char *str = (char *)comp.c_str();
  int index_delta = 0;
  
  for (list<unsigned>::iterator i = _wildcard_indeces.begin(); i != _wildcard_indeces.end(); ++i) {
    unsigned idx = *i+index_delta;
    char *tmppattern = pattern+idx;
    string name = _read_until(tmppattern, '>');
    string succ = _read_until_wildcard(tmppattern);

    char *strtemp = str;

    // advance to start of last occurance of the succ string in str
    while (strncmp(succ.c_str(), strtemp, succ.length()) != 0) strtemp++;
  
    unsigned valuelen = (unsigned)(strtemp-str);
  
    char *token = (char *)malloc(sizeof(char)*valuelen);
    strncpy(token,str,valuelen);
    string value(token);
    free(token);
    m[name] = value;
  }
  
  return m;
}

list<string> pattern::match_splats(string comp) {
  list<string> lst;
  char *pattern = (char *)_pattern.c_str();
  char *str = (char *)comp.c_str();
  int index_delta = 0;
  
  // for (size_t i = 0; i < _splat_indeces.size(); i++) {
//     unsigned idx = _splat_indeces[i]+index_delta;
//     char *tmppattern = pattern+idx+1;
//
//
//
//     char *strtemp = str;
//
//     // advance to start of last occurance of the succ string in str
//     while (strncmp((char *)succ.c_str(), strtemp, succ.length()) != 0) strtemp++;
//
//
//     str
//   }
  
  return lst;
}

//
// Private
//

void pattern::create(string ptrn) {
  _splat_indeces = _get_splat_indeces((char *)ptrn.c_str());
  _wildcard_indeces = _get_wildcard_indeces((char *)ptrn.c_str());
  _pattern = ptrn;
}

void pattern::_advance_to_wildcard(char *ptr, char c) {
  if (c == '\0') {
    while (*ptr != '*' && *ptr != '<' && *(ptr+1) != '\0') ptr++;
  } else {
    while (*ptr != c && *(ptr+1) != '\0') ptr++;
  }
}

unsigned pattern::_advance_to_succ(char *ptr) {
  char *start = ptr;
  if (ptr[0] == '<') {
    while (*ptr != '>') ptr++; ptr++;
  } else if (ptr[0] == '*') {
    ptr++;
  }
  return (unsigned)(ptr-start);
}

bool pattern::_ptrncmp(char *pattern, char *comp) {
  if (strlen(comp) == 0 && strlen(pattern) == 0) return true;
  
  if (_advance_to_succ(pattern) != 0) {
    char *succ = pattern;
    _advance_to_wildcard(pattern);
    unsigned succlen = (unsigned)(pattern-succ);

    if (strncmp(succ, comp, succlen) == 0)
      comp += succlen; // advance comp
    else
      return false;
  } else {
    if (pattern[0] != comp[0]) return false;
    return _ptrncmp(pattern+1, comp+1);
  }
}

string pattern::_read_until(char *ptr, char c) {
  char *savedptr = ptr;
  while (*(ptr++) != c);
  unsigned len = ptr-savedptr;
  char *token = (char *)malloc(sizeof(char)*len);
  strncpy(token,savedptr,len);
  string read_str(token);
  free(token);
  return read_str;
}

string pattern::_read_until_wildcard(char *ptr) {
  char *savedptr = ptr;
  while (*ptr != '<' && *ptr != '*') ptr++;
  unsigned len = ptr-savedptr;
  char *token = (char *)malloc(sizeof(char)*len);
  strncpy(token,savedptr,len);
  string read_str(token);
  free(token);
  return read_str;
}

list<unsigned> pattern::_get_splat_indeces(char *str, char *ptr) {
  if (!ptr) ptr = str;
  if (str == ptr) return list<unsigned>();
  
  while (*ptr != '*') ptr++; ptr++;
  
  list<unsigned> recursive = _get_splat_indeces(str, ptr+1);
  recursive.push_front((unsigned)(ptr-str));
  return recursive;
}

list<unsigned> pattern::_get_wildcard_indeces(char *str, char *ptr) {
  if (!ptr) ptr = str;
  if (str == ptr) return list<unsigned>();
  
  _advance_to_wildcard(ptr, '<');
  unsigned idx = (unsigned)(ptr-str);
  _advance_to_succ(ptr);
  
  list<unsigned> recursive = _get_wildcard_indeces(str, ptr);
  recursive.push_front(idx);
  return recursive;
}

list<string> pattern::_get_splats(char *pattern, char *comp) {
  
}

map<string, string> pattern::_get_wildcards(char *pattern, char *comp) {
  
}
