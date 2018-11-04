#include "headers.h"

void keyForm(std::vector <unsigned int> &Key) { //Развертывание ключа
  Key.resize(32);
  for (size_t i = 1; i <= 8; i++) {
    Key[i+8]  = Key[i];
    Key[i+16] = Key[i];
    Key[i+24] = Key[9 - i];
  }
}

/////////////////////////////Шифрование////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////


/////////////////////////Провка параметров командной строки////////////////////

bool there_is(const std::string flag, unsigned char &pos) {
  for (size_t i = 0; i < flags.size(); i++) {
    if (flags[i] == flag) {
      pos = i;
      return true;
    }
  }
  return false;
}

void c_fl_fill(const int argc, const char *argv[], std::vector <char> &c_fl) {
  unsigned char pos = 0;
  for (int i = 1; i < argc; i++) {
    if (there_is(argv[i], pos)) {
      c_fl[pos] += flag_val[pos];
    }
  }
}

void check_number_flags(const int argc, const char *argv[], const std::vector <char> &c_fl) {
  const unsigned char modes_sum = c_fl[2] + c_fl[3] + c_fl[4] + c_fl[5] + c_fl[6] + c_fl[7]; // Сумма количества встречающийхся режимов,
  const unsigned char help_sum = c_fl[0] + c_fl[1];                                          // флагов на помощь,
  unsigned char count_flags_sum = 0;                                                   // всех флагов
  for (size_t i = 0; i < 14; i++) {
    count_flags_sum += c_fl[i];
  }
  if (count_flags_sum != argc-1) {
    throw "Wrong number of arguments!\n";
  }
  if (help_sum > 1) {
    throw "Use -h or --help! Not both!\n";
  } else if (((modes_sum + help_sum) == 0) || ((modes_sum + help_sum) > 1)) {
    throw "Wrong mode!\n";
  } else if ((c_fl[8] + c_fl[9]) > 1) {
    throw "You can not encrypt and decrypt at the same time!\n";
  }
}

void check_file_flag_pos(const int argc, const char *argv[]) {
  for (int i = 1; i < argc; i++) {
    if ((argv[i] == flags[10] || argv[i] == flags[11] || argv[i] == flags[12] || argv[i] == flags[13]) && (i == argc-1)) {
      throw "File path must be after file flag!\n";
    }
  }
}

void is_there_k(const int argc, const char *argv[]) {
  bool k_flag = true;
  for (int i = 1; i < argc; i++) {
    if (argv[i] == flags[10]) {
      k_flag = false;
    }
  }
  if (k_flag) {
    throw "No key found!\n";
  }
}

void check_enc_dec(const int argc, const char *argv[], const std::vector <char> &c_fl) {
  if ((c_fl[8] + c_fl[9]) == 0) {
    throw "Choose mode : encrypt or decrypt!\n";
  } else if ((c_fl[7] + c_fl[9]) > 1) {
    throw "Mac mode has no encryption!\n";
  }
}

void check_args(const int argc, const char *argv[], std::vector <char> &c_fl) {
  c_fl_fill(argc, argv, c_fl);
  check_number_flags(argc, argv, c_fl);
  check_file_flag_pos(argc, argv);
  is_there_k(argc, argv);
  check_enc_dec(argc, argv, c_fl);
}

///////////////////////////////////////////////////////////////////////////////

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
