#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

#include "desx/desx.h"

using namespace std;

void WaitKey(){
  cin.get();
  cout << "\n[!] Press 'enter' key...";
  cin.get();
};

void CLS(){
  system("clear");
};

void MENU(){
  cout << "   [Menu]\n\n";

  cout << "[1] Шифрование\n";
  cout << "[2] Расшифрование\n";
  cout << "[3] Записать ключ\n";
  cout << "[0] Выход\n\n";
};

int main() {
  string   input_path, 
           enc_output_path, 
           dec_output_path;
  ifstream fin;
  ofstream fout;
  string   msg = "";
  string   key, 
           key1, 
           key2;
  DESX     encrypter;
  int      cmd;

  CLS();
  cout << CDESX::LOGO() << endl;

  cout << "    [Инициализация ключей]\n\n";
  cout << "[*] Введите ключ: "; cin >> key;
  fin.open("private.keys");
  if(fin.is_open()){
    fin >> key1;
    fin >> key2;
  }
  else{
    cerr << "[!] Файл с доп. ключами не найден!" << endl;
    WaitKey(); CLS();
    return -1;
  }

  fin.close();

  cout << "\n[+] Ключи инициализированны" << endl;
  WaitKey();

  for( ; ; ){
    CLS();
    cout << CDESX::LOGO() << endl;
    MENU();
    cout << "[>] "; cin >> cmd;
    switch(cmd){
      case 0: CLS(); return 0;

      case 1: {
        CLS();
        cout << CDESX::LOGO() << endl;

        msg.clear();

        encrypter.setKEY (key);
        encrypter.setBinaryKEY1(key1);
        encrypter.setBinaryKEY2(key2);

        cout <<   "[*] Путь до файла с откр. текстом: "; cin >> input_path;
        cout <<   "[*] Путь до файла с шифр-текстом : "; cin >> enc_output_path;

        fin.open(input_path);
        fout.open(enc_output_path);
        
        if(fin.is_open() && fout.is_open()){
          char ch;
          size_t cnt_chars = 0;
          for( ; fin.get(ch) ; ){
            msg += ch;
            ++cnt_chars;
            if(cnt_chars % encrypter.countPlainTextSymbols() == 0){
              encrypter.setMSG(msg);
              if(encrypter.encrypt())
                fout << encrypter.getBinaryMSG() << "\n";
              else
                cout << "[!] Encrypt failed..." << endl;

              msg.clear();
            }
          }
          if( msg != "" ){
            encrypter.setMSG(msg);
            if(encrypter.encrypt())
              fout << encrypter.getBinaryMSG();
            else
              cout << "[!] Ошибка шифрования..." << endl;

            msg.clear();
          }
        }
        else{
          cout << "[!] Ошибка открытия файла \"" << input_path << "\"...";
        }
        fin.close();
        fout.close();

        cout << "\n[+] Файл зашифрован" << endl;
        break;
      }

      case 2:{
        CLS();
        cout << CDESX::LOGO() << endl;

        msg.clear();
        
        encrypter.setKEY (key);
        encrypter.setBinaryKEY1(key1);
        encrypter.setBinaryKEY2(key2);

        cout <<   "[*] Путь до файла с шифр-текстом: "; cin >> enc_output_path;
        cout <<   "[*] Путь до файла с расшифровкой: "; cin >> dec_output_path;

        fin.open(enc_output_path);
        fout.open(dec_output_path);

        if(fin.is_open() && fout.is_open()){
          char ch;
          for( ; !fin.eof(); ){
            fin >> msg; 
            encrypter.setBinaryMSG(msg);
            if(encrypter.decrypt()){
              fout << encrypter.getMSG().c_str();
            }
            else
              cout << "[!] Ошибка расшифрования..." << endl;
            msg.clear();
          }
        }
        else{
          cout << "[!] Ошибка открытия файла \"" << enc_output_path << "\"...";
        }
        fin.close();
        fout.close();
        cout <<   "\n[+] Файл расшифрован" << endl;
        break;
      }

      case 3: {
        CLS();
        cout << CDESX::LOGO() << endl;
        string file = "all_keys.keys";
        fout.open(file);

        fout << key  << "\n";
        fout << key1 << "\n";
        fout << key2;

        fout.close();

        cout <<   "\n[+] Ключи записаны в файл \"" << file << "\"" << endl;
        
        break;
      }

      default: cout << "[!] Действие не определено..." << endl; break;
    }

    WaitKey();
  }
  
};