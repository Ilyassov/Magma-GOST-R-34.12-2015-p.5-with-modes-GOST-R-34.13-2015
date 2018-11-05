#include "headers.h"

int main (int argc, char *argv[]) {
  try {
    std::vector <char> c_fl(14, 0);         //Количество каждого флага в вызове функции
//    check_args(argc, argv, c_fl);
    std::vector <unsigned long long> text;  //Вектор для хранения текста по 8 байт
    std::vector <unsigned long long> ctext; //Вектор для хранения зашифрованного текста по 8 байт
    std::vector <unsigned long long> etext; //Вектор для хранения расшифрованного текста по 8 байт
    std::vector <unsigned char> init_text;           //Вектор для хранения текста по 1 байту
    char *read_ptr = NULL;
    unsigned int pos;
    std::vector <unsigned int> key;         //Вектор ключей
    key.push_back(0);
    keyProcess(pos, key, argv);
    input_redirect(read_ptr, pos, argv);
    full_init_text(read_ptr, init_text);


    size_t j = 0;
    while (j != init_text.size()) {
      unsigned long temp = 0;
      for (size_t i = 0; i < 8; i++) {
        temp = (temp <<  8) + (unsigned int)init_text[j];
        j++;
      }
      text.push_back(temp);
    }
    for (size_t i = 0; i < text.size(); i++) {
      std::cout << std::hex << text[i] << std::endl;
    }

    //-i <input file> – входной файл. По умолчанию читать с stdin до EOF;
//    ctext.resize(text.size());
//    etext.resize(ctext.size());

//    keyForm(key);
//    std::vector <unsigned long long> IV;

    /////////////////////MAC//////////////////////////////////////
    /*std::vector <unsigned long long> R;
    std::vector <unsigned long long> C;
    C.push_back(0);
    R.push_back(0x2fa2cd99a1290a12);
    unsigned int text_size_1 = text.size()-1; //Завел чтобы не считать по нескольку раз
    unsigned int a0 = 0;
    unsigned int a1 = 0;        //Значения текста на текущей итерации
    unsigned long long K1, K2, Kl;
    R[0] = enc(key, a1, a0);
    K1 = R[0] << 1;
    if (((R[0] << 1) >> 1) != R[0]) {
      K1 = K1 ^ 0x000000000000001b;
    }
    K2 = K1 << 1;
    if (((K1 << 1) >> 1) != R[0]) {
      K2 = K1 ^ 0x000000000000001b;
    }
    Kl = K1;  //Так как длин совпадает с n, а вообще по другому
    std::cout << "R\t\t" << std::hex << R[0] << std::endl;
    std::cout << "K1\t\t" << std::hex << K1 << std::endl;
    std::cout << "K2\t\t" << std::hex << K2 << std::endl;
    for (size_t i = 0; i < text_size_1; i++) {  //Пример работы orb
      unsigned int a0, a1;        //Значения текста на текущей итерации
      std::cout << "Text\t\t" << std::hex << text[i] << std::endl;
      std::cout << "Input\t\t" << std::hex << (text[i] ^ C[i]) << std::endl;
      a1 = (text[i] ^ C[i]) >> 32;         //Значение первой переменной для первого шага
      a0 = ((text[i] ^ C[i]) << 32) >> 32; //Значение второй переменной для первого шага
      ctext[i] = enc(key, a1, a0);
      C.push_back(ctext[i]);
      std::cout << "Output\t\t" << std::hex << ctext[i] << std::endl;
    }
    unsigned long long MAC_inp = text[text_size_1] ^ C[text_size_1] ^ Kl; // Входные переменные для кодирования в МАС
    std::cout << "Text\t\t" << std::hex << text[text_size_1] << std::endl;
    std::cout << "Input\t\t" << std::hex << MAC_inp << std::endl;
    a1 = MAC_inp >> 32;
    a0 = (MAC_inp << 32) >> 32;
    unsigned int MAC = (unsigned int)(enc(key, a1, a0) >> 32);
    std::cout << "MAC\t\t" << std::hex << MAC << std::endl;*/

    /////////////////////CFB//////////////////////////////////////
    /*std::vector <unsigned long long> R;
    R.push_back(IV[0]);
    R.push_back(IV[1]);
    unsigned long long temp = 0;
    for (size_t i = 0; i < text.size(); i++) {  //Пример работы orb
      unsigned int a0, a1;        //Значения текста на текущей итерации
      a1 = (R[i]) >> 32;         //Значение первой переменной для первого шага
      a0 = ((R[i]) << 32) >> 32; //Значение второй переменной для первого шага
      temp = enc(key, a1, a0);
      ctext[i] = text[i] ^ temp;
      etext[i] = ctext[i] ^ temp;
      R.push_back(ctext[i]);
      std::cout << "Text\t\t" << std::hex << text[i] << "\nCText\t\t" << ctext[i] << "\n\t\t\t" << (etext[i] == text[i]) << std::endl;
    }*/

    /////////////////////CBC//////////////////////////////////////
    /*  std::vector <unsigned long long> R;
    R.push_back(IV[0]);
    R.push_back(IV[1]);
    R.push_back(IV[2]);
    //std::cout << std::hex << a1 << std::endl;
    //std::cout << std::hex << a0 << std::endl;
    for (size_t i = 0; i < text.size(); i++) {  //Пример работы orb
      unsigned int a0, a1;        //Значения текста на текущей итерации
      a1 = (text[i] ^ R[i]) >> 32;         //Значение первой переменной для первого шага
      a0 = ((text[i] ^ R[i]) << 32) >> 32; //Значение второй переменной для первого шага
      ctext[i] = enc(key, a1, a0);
      a1 = (ctext[i]) >> 32;         //Значение первой переменной для первого шага
      a0 = ((ctext[i]) << 32) >> 32; //Значение второй переменной для первого шага
      etext[i] = (dec(key, a1, a0)) ^ R[i];
      R.push_back(ctext[i]);
      std::cout << "Text\t\t" << std::hex << text[i] << "\nCText\t\t" << ctext[i] << "\n\t\t\t" << (etext[i] == text[i]) << std::endl;
    }*/

    /////////////////////OFB//////////////////////////////////////
    /*  std::vector <unsigned long long> R;
    R.push_back(IV[0]);
    R.push_back(IV[1]);
    //std::cout << std::hex << a1 << std::endl;
    //std::cout << std::hex << a0 << std::endl;
    for (size_t i = 0; i < text.size(); i++) {  //Пример работы orb
      unsigned int a0, a1;        //Значения текста на текущей итерации
      a1 = R[i] >> 32;         //Значение первой переменной для первого шага
      a0 = (R[i] << 32) >> 32; //Значение второй переменной для первого шага
      for (size_t j = 1; j < 32; j++) {
        G(key[j], a1, a0);
      }
      ctext[i] = (( ((unsigned long long)g(key[32], a0)) ^ a1) << 32) + a0;
      R.push_back(ctext[i]);
      ctext[i] = text[i] ^ ctext[i];
      etext[i] = (( ((unsigned long long)g(key[32], a0)) ^ a1) << 32) + a0;
      etext[i] = ctext[i] ^ etext[i];
      std::cout << "Text  " << std::hex << text[i] << " CText " << ctext[i] << " " << (text[i] == etext[i]) <<std::endl;
    }*/

    ////////////////////CTR///////////////////////////////////////
    /*std::vector <unsigned long long> IV;   //Ключ для режима ctr
    std::vector <unsigned long long> CTR;
    IV.push_back(0x1234567800000000);
    CTR.push_back(0x1234567800000000);
    for (size_t i = 0; i < text.size()-1; i++) {
      CTR.push_back(CTR[i]+1);
    }
    //  std::cout << std::hex << IV[0] << std::endl;

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
    }*/

    //////////////////////////////////ECB///////////////////////////////////////////////
    /*  for (size_t i = 0; i < text.size(); i++) {  //Пример работы ecb
        unsigned int a0, a1;        //Значения текста на текущей итерации
        a1 = text[i] >> 32;         //Значение первой переменной для первого шага
        a0 = (text[i] << 32) >> 32; //Значение второй переменной для первого шага
        //std::cout << std::hex << a1 << std::endl;
        //std::cout << std::hex << a0 << std::endl;
        for (size_t j = 1; j < 32; j++) {
          G(key[j], a1, a0);
      }
      ctext[i] = (( ((unsigned long long)g(key[32], a0)) ^ a1) << 32) + a0;
      std::cout << std::hex << text[i] << " " << ctext[i] << std::endl;
    }*/

    ////////Считывание ключа с бинарного файла/////////
    /*  std::vector<int> Key;
      std::vector<std::vector<int>> Keys;
      Keys.resize(33);
      keyDeploy(argv[1], Key, Keys);*/
    ///////////////////////////////////////////////////

    /////Считывание с файла до конца файла или переноса, или \r
    /*  char ch;
      std::ifstream File(argv[1], std::ios::binary);
      do {
        File.read((char*)&ch, sizeof(ch));
      } while ((ch != '\n') && (ch != EOF) && (int(ch) != '\r'));
      File.close();*/
    //////////////////////////////////////////////////////////////
  }
  catch (const char * S) {
    std::cerr << S << help;
  }
  catch (const std::string S) {
    std::cerr << S;
  }
  catch(...) {
    std::cerr << "Unknown error!\n";
  }
  return 0;
}

//FILE * ptrFile = freopen(read_ptr, "rb", stdin);
/*char ch;
while ((ch = fgetc(ptrFile))) {
init_text.push_back(ch);
std::cout << std::hex << int(ch);
}
fclose(ptrFile);*/
