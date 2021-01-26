#include <iostream>
#include <string>
#include <locale>


// const char * ConvertToUTF8(const wchar_t * pStr) {
//     static char szBuf[1024];
//     WideCharToMultiByte(CP_UTF8, 0, pStr, -1, szBuf, sizeof(szBuf), NULL, NULL);
//     return szBuf;
// }


int main(int argc, char *argv[]) {

  using namespace std;
  string word = "azertyué";
  cout << word.size() << endl;
  cout << word << endl;
  string wildcard = "azertyu*";
  cout << wildcard.size() << endl;

  cout << "é" << endl;
  cout << "é" << endl;
}
