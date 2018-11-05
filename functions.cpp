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


///////////////////////Проверка параметров командной строки////////////////////

bool there_is(const std::string flag, unsigned char &pos) {
  for (size_t i = 0; i < flags.size(); i++) {
    if (flags[i] == flag) {
      pos = i;
      return true;
    }
  }
  return false;
}

void c_fl_fill(int argc, char *argv[], std::vector <char> &c_fl) {
  unsigned char pos = 0;
  for (int i = 1; i < argc; i++) {
    if (there_is(argv[i], pos)) {
      c_fl[pos] += flag_val[pos];
    }
  }
}

void check_number_flags(int argc, char *argv[], const std::vector <char> &c_fl) {
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

void check_file_flag_pos(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    if ((argv[i] == flags[10] || argv[i] == flags[11] || argv[i] == flags[12] || argv[i] == flags[13]) && (i == argc-1)) {
      throw "File path must be after file flag!\n";
    }
  }
}

void is_there_k(int argc, char *argv[]) {
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

void check_enc_dec(int argc, char *argv[], const std::vector <char> &c_fl) {
  if ((c_fl[8] + c_fl[9]) == 0) {
    throw "Choose mode : encrypt or decrypt!\n";
  } else if ((c_fl[7] + c_fl[9]) > 1) {
    throw "Mac mode has no encryption!\n";
  } else if (c_fl[2] + c_fl[7] + c_fl[11] > 1) {
    throw "IV not needed in ecb or mac modes!\n";
  }
}

void check_args(int argc, char *argv[], std::vector <char> &c_fl) {
  c_fl_fill(argc, argv, c_fl);
  check_number_flags(argc, argv, c_fl);
  check_file_flag_pos(argc, argv);
  is_there_k(argc, argv);
  check_enc_dec(argc, argv, c_fl);
}
///////////////////////////////////////////////////////////////////////////////

unsigned int search(char* argv[], const std::string &flag) {
  for (size_t i = 1; i < 14; i++) {
    if (argv[i] == flag) {
      return i+1;
    }
  }
  return 0;
}

void keySizeCheck(const char* file) {
  std::ifstream in(file, std::ifstream::binary);
  if (in) {
    in.seekg (0, in.end);
    unsigned int length = in.tellg();
    in.seekg (0, in.beg);
    in.close();
    if (length != 32) {
      throw "Wrong key size!\n";
    }
  } else {
    throw "File not opened!\n";
  }
}

void keyRead(std::vector <unsigned int> &key, const char* file) {
  keySizeCheck(file);
  unsigned int buff;
  std::ifstream in(file, std::ios::binary);
  for (size_t i = 0; i < 8; i++) {
    in.read((char*)&buff, sizeof(buff)); //Перенос байтов из файла в buff
    key.push_back(buff);
  }
  in.close();
}

void keyProcess(unsigned int &pos, std::vector <unsigned int> &key, char* argv[]) {
  if ((pos = search(argv, "-k"))) {
    keyRead(key, argv[pos]);
  } else {
    throw "No key flag found!\n";
  }
}

void full_init_text(const char *read_ptr, std::vector <unsigned char> &init_text) { //Выполняет побайтовое бинарное считывание
  unsigned char ch;
  std::ifstream in(read_ptr, std::ios::binary);
  in.seekg (0, in.end);
  unsigned int length = in.tellg();
  in.seekg (0, in.beg);
  for (size_t i = 0; i < length; i++) {
    in.read((char*)&ch, sizeof(ch));
    init_text.push_back(ch);
  }
  in.close();
}

void input_redirect(char *(&read_ptr), unsigned int &pos, char *argv[]) { //Если нужно перенаправляет поток входа на файл
  if ((pos = search(argv, "-i"))) {
    read_ptr = argv[pos];
  }
}


void full_text(const std::vector <unsigned char> &init_text, std::vector <unsigned long long> &text) { //Заполняет текст по байтам
  size_t j = 0;                                                                                        //находящимся в init_text
  while (j < init_text.size()) {
    unsigned long long temp = 0;
    for (size_t i = 0; i < 8; i++) {
      temp = (temp <<  8) + (unsigned int)init_text[j];
      j++;
    }
    text.push_back(temp);
  }
}

void zero_supplement(char *argv[], std::vector <unsigned char> &init_text) {
  while ((init_text.size() % 8) != 0) {
    init_text.push_back(0);
  }
}

void one_supplement(char *argv[], std::vector <unsigned char> &init_text) {
  init_text.push_back((1 << 7));
  zero_supplement(argv, init_text);
}

void smart_supplement(char *argv[], std::vector <unsigned char> &init_text) {
  if ((init_text.size() % 8) != 0) {
    init_text.push_back((1 << 7));
  }
  zero_supplement(argv, init_text);
}

int text_supplement(char *argv[], std::vector <unsigned char> &init_text, const std::vector <char> c_fl) {
  if ((c_fl[3] + c_fl[4] + c_fl[6]) == 1) { //режимы CTR, OFB и CFB
    zero_supplement(argv, init_text);
  } else if ((c_fl[2] + c_fl[5]) == 1) { //режимы ECB и CBC
    int one_pos = init_text.size() % 8;
    zero_supplement(argv, init_text);
    return one_pos;
  } else { //режим MAC
    smart_supplement(argv, init_text);
  }
  return -1;
}

void suplement(int one_pos, std::vector <unsigned long long> &text) {
  if (one_pos != -1) {
    if (one_pos) {
      text[text.size()-1] = text[text.size()-1] + (((unsigned long long)(1 << 7)) << (56 - 8 * one_pos)); //Ставим 1 сразу после последнего считанного байта
    } else if (one_pos == 0) {
      text.push_back(0x8000000000000000);
    }
  }
}

void ecb(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key) {
  unsigned int a0, a1;        //Значения текста на текущей итерации
  if (c_fl[8]) {  // encrypt
    for (size_t i = 0; i < text.size(); i++) {
      a1 = text[i] >> 32;         //Значение первой переменной
      a0 = (text[i] << 32) >> 32; //Значение второй переменной
      for (size_t j = 1; j < 32; j++) { //Применение первых 31 шагов
        G(key[j], a1, a0);
      }
      ctext[i] = (( ((unsigned long long)g(key[32], a0)) ^ a1) << 32) + a0;
      std::cout << std::hex << text[i] << " " << ctext[i] << std::endl;
    }
  } else if (c_fl[9]) { //decrypt
    for (size_t i = 0; i < text.size(); i++) {
      ctext[i] = text[i];
      a1 = (ctext[i]) >> 32;
      a0 = ((ctext[i]) << 32) >> 32;
      etext[i] = dec(key, a1, a0);
    }
  }
}

unsigned int file_check(std::ifstream &in) {
  in.seekg(0, in.end);
  unsigned int length = in.tellg();
  in.seekg(0, in.beg);
  return length;
}

size_t choise(char c) {
  if (c) {
    return 4;
  } else {
    return 8;
  }
}

void IVread(const std::vector <char> &c_fl, char *(&read_ptr), std::vector <unsigned long long> &IV) {
  unsigned char ch;
  std::ifstream in(read_ptr, std::ios::binary);
  unsigned int length = file_check(in);
  if ((c_fl[3] && (length != 4)) || ((length % 8) != 0))
    throw "Wrong CTR size!\n";
  size_t j = 0;
  size_t k = choise(c_fl[3]);
  while (j < length) {
    unsigned long long temp;
    for (size_t i = 0; i < k; i++) {
      in.read((char*)&(ch), sizeof(ch));
      temp = (temp << 8) + (unsigned int)ch;
      j++;
    }
    IV.push_back(temp);
  }
  if (k == 4) {
    IV[0] = (IV[0] << 32);
  }
  in.close();
}

void ctr_enc(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> CTR) {
  for (size_t i = 0; i < text.size(); i++) {  //Пример работы ctr
    unsigned int a0, a1;       //Значения текста на текущей итерации
    a1 = CTR[i] >> 32;         //Значение первой переменной для первого шага
    a0 = (CTR[i] << 32) >> 32; //Значение второй переменной для первого шага
    for (size_t j = 1; j < 32; j++) {
      G(key[j], a1, a0);
    }
    ctext[i] = (( ((unsigned long long)g(key[32], a0)) ^ a1) << 32) + a0;
    ctext[i] = text[i] ^ ((ctext[i] >> (64-s)) << (64-s));
    etext[i] = (( ((unsigned long long)g(key[32], a0)) ^ a1) << 32) + a0;
    etext[i] = ctext[i] ^ ((etext[i] >> (64-s)) << (64-s));
    std::cout << "Text  " << std::hex << text[i] << " CText " << ctext[i] << " " << (text[i] == etext[i]) << std::endl;
  }
}

void ctr_dec(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> CTR) {
  for (size_t i = 0; i < text.size(); i++) {  //Пример работы ctr
    unsigned int a0, a1;       //Значения текста на текущей итерации
    a1 = CTR[i] >> 32;         //Значение первой переменной для первого шага
    a0 = (CTR[i] << 32) >> 32; //Значение второй переменной для первого шага
    for (size_t j = 1; j < 32; j++) {
      G(key[j], a1, a0);
    }
    etext[i] = (( ((unsigned long long)g(key[32], a0)) ^ a1) << 32) + a0;
    etext[i] = ctext[i] ^ ((etext[i] >> (64-s)) << (64-s));
  }
}

void ctr(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key, char *argv[]) {
  std::vector <unsigned long long> IV;   //Ключ для режима ctr
  unsigned int pos;
  if ((pos = search(argv, "-v"))) {
    IVread(c_fl, argv[pos], IV);
  } else {
    IV.push_back(0);
  }
  std::vector <unsigned long long> CTR;
  CTR.push_back(IV[0]);
  for (size_t i = 0; i < text.size()-1; i++) {
    CTR.push_back(CTR[i]+1);
  }
  if (c_fl[8]) {
    ctr_enc(c_fl, text, ctext, etext, key, CTR);
  } else if (c_fl[9]) {
    ctr_dec(c_fl, text, ctext, etext, key, CTR);
  } else {
    throw "CTR Error!\n";
  }
}

void ofb_enc(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> R) {
  for (size_t i = 0; i < text.size(); i++) {
    unsigned int a0, a1;        //Значения текста на текущей итерации
    a1 = R[i] >> 32;         //Значение первой переменной для первого шага
    a0 = (R[i] << 32) >> 32; //Значение второй переменной для первого шага
    for (size_t j = 1; j < 32; j++) {
      G(key[j], a1, a0);
    }
    ctext[i] = (( ((unsigned long long)g(key[32], a0)) ^ a1) << 32) + a0;
    R.push_back(ctext[i]);
    ctext[i] = text[i] ^ ctext[i];
  }
}

void ofb_dec(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> R) {
  for (size_t i = 0; i < text.size(); i++) {
    unsigned int a0, a1;        //Значения текста на текущей итерации
    a1 = R[i] >> 32;         //Значение первой переменной для первого шага
    a0 = (R[i] << 32) >> 32; //Значение второй переменной для первого шага
    for (size_t j = 1; j < 32; j++) {
      G(key[j], a1, a0);
    }
    ctext[i] = text[i];
    R.push_back(ctext[i]);
    etext[i] = (( ((unsigned long long)g(key[32], a0)) ^ a1) << 32) + a0;
    etext[i] = ctext[i] ^ etext[i];
  }
}


void ofb(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key, char *argv[]) {
  std::vector <unsigned long long> IV;
  unsigned int pos;
  if ((pos = search(argv, "-v"))) {
    IVread(c_fl, argv[pos], IV);
  } else {
    IV.push_back(0);
  }
  std::vector <unsigned long long> R;
  for (size_t j = 0; j < IV.size(); j++) {
    R.push_back(IV[j]);
  }
  if (c_fl[8]) {
    ofb_enc(c_fl, text, ctext, etext, key, R);
  } else if (c_fl[9]) {
    ofb_dec(c_fl, text, ctext, etext, key, R);
  } else {
    throw "OFB Error!\n";
  }
}

void cbc_enc(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> R) {
  for (size_t i = 0; i < text.size(); i++) {
    unsigned int a0, a1;        //Значения текста на текущей итерации
    a1 = (text[i] ^ R[i]) >> 32;         //Значение первой переменной для первого шага
    a0 = ((text[i] ^ R[i]) << 32) >> 32; //Значение второй переменной для первого шага
    ctext[i] = enc(key, a1, a0);
    R.push_back(ctext[i]);
  }
}

void cbc_dec(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> R) {
  for (size_t i = 0; i < text.size(); i++) {
    unsigned int a0, a1;
    ctext[i] = text[i];
    a1 = (ctext[i]) >> 32;         //Значение первой переменной для первого шага
    a0 = ((ctext[i]) << 32) >> 32; //Значение второй переменной для первого шага
    etext[i] = (dec(key, a1, a0)) ^ R[i];
    R.push_back(ctext[i]);
  }

}

void cbc(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key, char *argv[]) {
  std::vector <unsigned long long> IV;
  unsigned int pos;
  if ((pos = search(argv, "-v"))) {
    IVread(c_fl, argv[pos], IV);
  } else {
    IV.push_back(0);
  }
  std::vector <unsigned long long> R;
  for (size_t j = 0; j < IV.size(); j++) {
    R.push_back(IV[j]);
  }
  if (c_fl[8]) {
    cbc_enc(c_fl, text, ctext, etext, key, R);
  } else if (c_fl[9]) {
    cbc_dec(c_fl, text, ctext, etext, key, R);
  } else {
    throw "CBC Error!\n";
  }
}

void cfb_dec(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> R) {
              unsigned long long temp = 0;
  for (size_t i = 0; i < text.size(); i++) {  //Пример работы orb
    unsigned int a0, a1;        //Значения текста на текущей итерации
    a1 = (R[i]) >> 32;         //Значение первой переменной для первого шага
    a0 = ((R[i]) << 32) >> 32; //Значение второй переменной для первого шага
    temp = enc(key, a1, a0);
    ctext[i] = text[i];
    etext[i] = ctext[i] ^ temp;
    R.push_back(ctext[i]);
  }
}

void cfb_enc(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> R) {
  unsigned long long temp = 0;
  for (size_t i = 0; i < text.size(); i++) {  //Пример работы orb
    unsigned int a0, a1;        //Значения текста на текущей итерации
    a1 = (R[i]) >> 32;         //Значение первой переменной для первого шага
    a0 = ((R[i]) << 32) >> 32; //Значение второй переменной для первого шага
    temp = enc(key, a1, a0);
    ctext[i] = text[i] ^ temp;
    R.push_back(ctext[i]);
  }

}

void cfb(const std::vector <char> &c_fl,
        const std::vector <unsigned long long> &text,
        std::vector <unsigned long long> &ctext,
        std::vector <unsigned long long> &etext,
        const std::vector <unsigned int> &key, char *argv[]) {
  std::vector <unsigned long long> IV;
  unsigned int pos;
  if ((pos = search(argv, "-v"))) {
    IVread(c_fl, argv[pos], IV);
  } else {
    IV.push_back(0);
  }
  std::vector <unsigned long long> R;
  for (size_t j = 0; j < IV.size(); j++) {
    R.push_back(IV[j]);
  }
  if (c_fl[8]) {
    cbc_enc(c_fl, text, ctext, etext, key, R);
  } else if (c_fl[9]) {
    cbc_dec(c_fl, text, ctext, etext, key, R);
  } else {
    throw "CFB Error!\n";
  }
}

void K_func(const std::vector <unsigned long long> &R, unsigned long long &K1,
                          unsigned long long &K2, unsigned long long &Kl, int mack) {
  K1 = R[0] << 1;
  if (((R[0] << 1) >> 1) != R[0]) {
    K1 = K1 ^ 0x000000000000001b;
  }
  K2 = K1 << 1;
  if (((K1 << 1) >> 1) != R[0]) {
    K2 = K1 ^ 0x000000000000001b;
  }
  if (mack) {
    Kl = K2;  //Если длниа совпадает с n
  } else {
    Kl = K1;
  }
}

void mac(const std::vector <char> &c_fl,
        const std::vector <unsigned long long> &text,
        std::vector <unsigned long long> &ctext,
        std::vector <unsigned long long> &etext,
        const std::vector <unsigned int> &key, char *argv[], int mack) {
  std::vector <unsigned long long> R;
  std::vector <unsigned long long> C;
  C.push_back(0);
  unsigned int text_size_1 = text.size()-1; //Завел чтобы не считать по нескольку раз
  unsigned int a0 = 0, a1 = 0;
  unsigned long long K1, K2, Kl;
  R.push_back(enc(key, a1, a0));
  K_func(R, K1, K2, Kl, mack);
  for (size_t i = 0; i < text_size_1; i++) {  //Пример работы orb
    unsigned int a0, a1;        //Значения текста на текущей итерации
    a1 = (text[i] ^ C[i]) >> 32;         //Значение первой переменной для первого шага
    a0 = ((text[i] ^ C[i]) << 32) >> 32; //Значение второй переменной для первого шага
    ctext[i] = enc(key, a1, a0);
    C.push_back(ctext[i]);
  }
  unsigned long long MAC_inp = text[text_size_1] ^ C[text_size_1] ^ Kl; // Входные переменные для кодирования в МАС
  a1 = MAC_inp >> 32;
  a0 = (MAC_inp << 32) >> 32;
  unsigned int MAC = (unsigned int)(enc(key, a1, a0) >> 32);
  etext.push_back((unsigned long long)MAC);
}

void action(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key, char *argv[], int mack) {
  if (c_fl[2]) {
    ecb(c_fl, text, ctext, etext, key);
  } else if (c_fl[3]) {
    ctr(c_fl, text, ctext, etext, key, argv);
  } else if (c_fl[4]) {
    ofb(c_fl, text, ctext, etext, key, argv);
  } else if (c_fl[5]) {
    cbc(c_fl, text, ctext, etext, key, argv);
  } else if (c_fl[6]) {
    cfb(c_fl, text, ctext, etext, key, argv);
  } else if (c_fl[7]) {
    mac(c_fl, text, ctext, etext, key, argv, mack);
  } else {
    throw "Wrong mode!\n";
  }

}
