#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

const std::vector <std::vector<int>> pi = { {12, 4, 6, 2, 10, 5, 11, 9, 14, 8, 13, 7, 0, 3, 15, 1},
                                            {6, 8, 2, 3, 9, 10, 5, 12, 1, 14, 4, 7, 11, 13, 0, 15},
                                            {11, 3, 5, 8, 2, 15, 10, 13, 14, 1, 7, 4, 12, 9, 6, 0},
                                            {12, 8, 2, 1, 13, 4, 15, 6, 7, 0, 10, 5, 3, 14, 9, 11},
                                            {7, 15, 5, 10, 8, 1, 6, 13, 0, 9, 3, 14, 11, 4, 2, 12},
                                            {5, 13, 15, 6, 9, 2, 12, 10, 11, 7, 8, 1, 4, 3, 14, 0},
                                            {8, 14, 2, 5, 6, 9, 1, 12, 15, 4, 11, 0, 13, 10, 3, 7},
                                            {1, 7, 14, 13, 0, 5, 8, 3, 4, 15, 10, 6, 9, 12, 11, 2} };

const std::vector <std::string> flags = {"-h", "--help", "--ecb", "--ctr", "--ofb", "--cbc", "--cfb", "--mac",
                                    "-e", "-d", "-k", "-v", "-i", "-o"};
const std::vector <char> flag_val = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2};
const std::string help = "magma [-h|--help]\nmagma [--ecb|--ctr|--ofb|--cbc|--cfb] {-e|-d} -k <key file> [options]\nmagma --mac -k <key file> [options]\n";

const unsigned int s = 64;
const unsigned int n = 64;
const unsigned int z = 2;
const unsigned int m = z * n;

void keyForm(std::vector <unsigned int> &Key);

unsigned int t(const unsigned int& a);
unsigned int g(const unsigned int& key, const unsigned int& a0);
void G(const unsigned int& key, unsigned int& a1, unsigned int& a0);
unsigned long long Gl(const unsigned int& key, unsigned int& a1, unsigned int& a0);
unsigned long long enc(const std::vector <unsigned int>& key, unsigned int& a1, unsigned int& a0);
unsigned long long dec(const std::vector <unsigned int>& key, unsigned int& a1, unsigned int& a0);


bool there_is(const std::string flag, unsigned char &pos);  //Проверяет на наличие флага и изменяет pos
                                                            //на его позицию в этом случае
void c_fl_fill(int argc, char *argv[], std::vector <char> &c_fl); //Заполняет c_fl количеством появления флагов
void check_number_flags(int argc, char *argv[], const std::vector <char> &c_fl);  //Проверяет аргументы командной строки
                                                                                        //по их количеству
void check_file_flag_pos(int argc, char *argv[]);//Проверяет позицию файлового флага
                                                              //Если в конце, то ошибка
void check_enc_dec(int argc, char *argv[], const std::vector <char> &c_fl);
void check_args(int argc, char *argv[], std::vector <char> &c_fl);  //Проверяет входные аргументы командной строки

unsigned int search(char* argv[], const std::string &flag, int argc);
void keySizeCheck(const char* file);
void keyRead(std::vector <unsigned int> &key, const char* file);  //Считывание ключа
void keyProcess(unsigned int &pos, std::vector <unsigned int> &key, char* argv[], int argc);

void full_init_text(const char *read_ptr, std::vector <unsigned char> &init_text); //Выполняет побайтовое бинарное считывание
void input_redirect(char *(&read_ptr), unsigned int &pos, char *argv[], int argc);//Если нужно перенаправляет поток входа на файл
unsigned long long swap(unsigned long long temp); //Меняет байты местами в правильный порядок
void full_text(const std::vector <unsigned char> &init_text, std::vector <unsigned long long> &text); //Заполняет текст по байтам
                                                                                                    //находящимся в init_text
void zero_supplement(char *argv[], std::vector <unsigned char> &init_text); //Дополнение нулями
void one_supplement(char *argv[], std::vector <unsigned char> &init_text);  //Дополнение единицей и нулями
void smart_supplement(char *argv[], std::vector <unsigned char> &init_text);//Дополнение в зависимости от размера
void text_supplement(char *argv[], std::vector <unsigned char> &init_text, const std::vector <char> c_fl);//Вызов дополнения в зависимости от режима
//void suplement(int one_pos, std::vector <unsigned long long> &text);

void ofb_enc(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> R);
void ofb_dec(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> R);
void ofb(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key, char *argv[], int argc);

void make_ans_text(std::vector <unsigned char> &ans_text,
                std::vector <unsigned long long> &ctext,
                std::vector <unsigned long long> &etext,
                const std::vector <char> &c_fl);

unsigned int file_check(std::ifstream &in);
size_t choise(char c);
void IVread(const std::vector <char> &c_fl, char *(&read_ptr), std::vector <unsigned long long> &IV);

void ctr_enc(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> CTR);
void ctr_dec(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> CTR);
void ctr(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key, char *argv[], int argc);

void ecb(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key);

void cbc_enc(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> R);

void cbc_dec(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> R);

void cbc(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key, char *argv[], int argc);

void cfb_dec(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> R);

void cfb_enc(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key,
            std::vector <unsigned long long> R);

void cfb(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key, char *argv[], int argc);

void mac(const std::vector <char> &c_fl,
        const std::vector <unsigned long long> &text,
        std::vector <unsigned long long> &ctext,
        std::vector <unsigned long long> &etext,
        const std::vector <unsigned int> &key, char *argv[], int mack);

void K_func(const std::vector <unsigned long long> &R, unsigned long long &K1,
                          unsigned long long &K2, unsigned long long &Kl);

void action(const std::vector <char> &c_fl,
            const std::vector <unsigned long long> &text,
            std::vector <unsigned long long> &ctext,
            std::vector <unsigned long long> &etext,
            const std::vector <unsigned int> &key, char *argv[], int mack, int argc);

            void print_ans(const std::vector <unsigned char> &ans_text, char *argv[], int argc);
