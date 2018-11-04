#include "headers.h"

void keyForm(std::vector <unsigned int> &Key) { //Развертывание ключа
  Key.resize(32);
  for (size_t i = 1; i <= 8; i++) {
    Key[i+8]  = Key[i];
    Key[i+16] = Key[i];
    Key[i+24] = Key[9 - i];
  }
}

unsigned int t(const unsigned int& a) {
  unsigned int subVal[8];     //Массив для примениния нелинейного биективного преобразования
  unsigned int highligh = 15; //Помогает выделять 4 бита
  for (size_t i = 0; i < 8; i++) {      //Разбиение текста по 4 бита
    subVal[i] = (a & highligh) >> i * 4;
    highligh = highligh << 4;
  }
  for (size_t i = 0; i < 8; i++) { //нелинейное биективное преобразование
    unsigned int tmp = subVal[i];
    subVal[i] = pi[i][tmp];
  }
  unsigned int ans = 0;
  for (size_t i = 0; i < 8; i++) { //Формирование текста по 4 битам
    ans += (subVal[i] << (4 * i));
  }
  return ans;
}

unsigned int g(const unsigned int& key, const unsigned int& a0) {
  unsigned int Sum = t(key + a0);
  return ((Sum << 11) + ((Sum & (2047 << 21)) >> 21));  //Делаем циклический сдвиг влево на 11
}

void G(const unsigned int& key, unsigned int& a1, unsigned int& a0) {
  unsigned long A1 = a1;  //Сохраняем старое значение а1
  a1 = a0;
  a0 = g(key, a0) ^ A1;
}

unsigned long long Gl(const unsigned int& key, unsigned int& a1, unsigned int& a0) {
  return ((( ((unsigned long long)g(key, a0)) ^ a1) << 32) + a0);
}

unsigned long long enc(const std::vector <unsigned int>& key, unsigned int& a1, unsigned int& a0) {
  for (size_t j = 1; j < 32; j++) {
    G(key[j], a1, a0);
  }
  return Gl(key[32], a1, a0);
}

unsigned long long dec(const std::vector <unsigned int>& key, unsigned int& a1, unsigned int& a0) {
  for (size_t j = 32; j > 1; j--) {
    G(key[j], a1, a0);
  }
  return Gl(key[1], a1, a0);
}

bool there_is(const std::string flag, unsigned char &pos) {
  for (size_t i = 0; i < flags.size(); i++) {
    if (flags[i] == flag) {
      pos = i;
      return true;
    }
  }
  return false;
}

void encount_flags_fill(int argc, char *argv[], std::vector <char> &encount_flags) {
  unsigned char pos = 0;
  for (int i = 1; i < argc; i++) {
    if (there_is(argv[i], pos)) {
      encount_flags[pos] += flag_val[pos];
    }
  }
}

void check_args(int argc, char *argv[], std::vector <char> &encount_flags) {
  encount_flags_fill(argc, argv, encount_flags);
/*  for (size_t i = 0; i < 14; i++) {
    std::cout << std::dec << char(encount_flags[i] + '0') << std::endl;
  }*/
  
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

*/
