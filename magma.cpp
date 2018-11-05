#include "headers.h"

int main (int argc, char *argv[]) {
  try {
    std::vector <char> c_fl(14, 0);         //Количество каждого флага в вызове функции
    check_args(argc, argv, c_fl);
    std::vector <unsigned long long> text;  //Вектор для хранения текста по 8 байт
    std::vector <unsigned long long> ctext; //Вектор для хранения зашифрованного текста по 8 байт
    std::vector <unsigned long long> etext; //Вектор для хранения расшифрованного текста по 8 байт
    std::vector <unsigned char> init_text;           //Вектор для хранения текста по 1 байту
    std::vector <unsigned char> ans_text;
    char *read_ptr = NULL;
    unsigned int pos;
    std::vector <unsigned int> key;         //Вектор ключей
    key.push_back(0);
    keyProcess(pos, key, argv, argc);
    input_redirect(read_ptr, pos, argv, argc);
    full_init_text(read_ptr, init_text);
    unsigned int mack = init_text.size() % 8;
    text_supplement(argv, init_text, c_fl);
    full_text(init_text, text);
    ctext.resize(text.size());
    etext.resize(ctext.size());
    action(c_fl, text, ctext, etext, key, argv, mack, argc);
    make_ans_text(ans_text, ctext, etext, c_fl);
    print_ans(ans_text, argv, argc);
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
