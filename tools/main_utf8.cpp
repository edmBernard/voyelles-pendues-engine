#include <iostream>
#include <string>

#include "utf8cpp/utf8.h"

int main(int argc, char *argv[]) {

  using namespace std;
  string word = "azertyué";
  cout << "utf8 distance :" << utf8::distance(word.begin(), word.end()) << endl;

  for (auto it_i = word.begin(); it_i != word.end(); utf8::next(it_i, word.end())) {
    cout << *it_i << endl;
  }

  cout << word << endl;
  string wildcard = "azertyu*";
  cout << wildcard.size() << endl;
}
