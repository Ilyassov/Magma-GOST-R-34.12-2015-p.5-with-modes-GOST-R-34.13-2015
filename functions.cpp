#include "headers.h"

void keyForm(std::vector <unsigned int> &Key) { //Развертывание ключа
  Key.resize(32);
  for (size_t i = 1; i <= 8; i++) {
    Key[i+8]  = Key[i];
    Key[i+16] = Key[i];
    Key[i+24] = Key[9 - i];
  }
}

unsigned int g(const unsigned int& key, const unsigned int& a0) {
  unsigned int Sum = _int_32(a0) + _int_32(key);
}

void G(const unsigned int& key, unsigned int& a1, unsigned int& a0) {
  unsigned long A1 = a1;  //Сохраняем старое значение а1
  a1 = a0;
  a0 = g(key, a0) ^ A1;
}





/*void keyRead(std::string fileName, std::vector<int>& Key) {
  char ch;
  std::ifstream File(fileName, std::ios::binary);
  for (int j = 0; j < 32; j++) {
    File.read((char*)&ch, sizeof(ch));
    for (unsigned int i = 0; i < 8; i++) {
      Key.push_back((ch & (128 >> i)) != 0);
    }
  }
  File.close();
}
*/


/*
void keyDeploy(const std::string fileName, std::vector<int> &Key, std::vector<std::vector<int> >& Keys) {
  keyRead(fileName, Key);
  keysForm(Key, Keys);
}

int _int_32(const std::vector<std::vector<int>> numb, int ind) {
  int ans = 0;
  for (int i = 0; i < 32; i++) {
    ans = (ans << 1) + numb[ind][i];
  }
  return ans;
}
*/
