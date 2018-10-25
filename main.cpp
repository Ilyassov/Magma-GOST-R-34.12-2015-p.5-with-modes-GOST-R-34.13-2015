#include "headers.h"
#include "vector.h"

int main (int argc, char* argv[]) {

  std::vector <unsigned long> text;   //Вектор для хранения текста по 8 байт
  text.push_back(0xfedcba9876543210); //Значение текста из примера
  //  std::cout << std::hex << text[0] << std::endl;

  size_t i = 0; //Итератор для текста
  size_t j = 1; //Итератор для ключа

  unsigned int a0, a1;       //Значения текста на текущей итерации
  a1 = text[i] >> 32;         //Значение первой переменной для первого шага
  a0 = (text[i] << 32) >> 32; //Значение второй переменной для первого шага
  //std::cout << std::hex << a1 << std::endl;
  //std::cout << std::hex << a0 << std::endl;


  std::vector <unsigned int> key; //Вектор ключей
  key.push_back(0);               //Для удобства индексация начинается с 1
  key.push_back(0xffeeddcc);      //Значение ключа из примера
  key.push_back(0xbbaa9988);
  key.push_back(0x77665544);
  key.push_back(0x33221100);
  key.push_back(0xf0f1f2f3);
  key.push_back(0xf4f5f6f7);
  key.push_back(0xf8f9fafb);
  key.push_back(0xfcfdfeff);
  keyForm(key);
/*  for (size_t j = 0; j <= key.size(); j++) {
    std::cout << std::hex << key[j] << std::endl;
  }*/
  /*  std::cout << std::hex << key[0] << std::endl;
  std::cout << std::hex << key[1] << std::endl;
  std::cout << std::hex << key[2] << std::endl;
  std::cout << std::hex << key[3] << std::endl;
  std::cout << std::hex << key[4] << std::endl;
  std::cout << std::hex << key[5] << std::endl;
  std::cout << std::hex << key[6] << std::endl;
  std::cout << std::hex << key[7] << std::endl;
  std::cout << std::hex << key[8] << std::endl;*/

//  G(key[j], a1, a0);

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

  return 0;
}
