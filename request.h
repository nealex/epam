#include <iostream>
#include <string>
using namespace std;

/**
 * @description Структура описывающая запрос
 */
struct request
{
    unsigned int i;            // Порядковый номер запроса с начала работы программы
    string inputHostIp;        // Ip адресс уазла получателя
    int inputHostPort;         // Порт узла получателя
    string inputHostMac;       // Mac адресс узла получателя
    string outHostIp;          // Ip адресс уазла отправителя
    int outHostPort;           // Порт узла отправителя
    string outHostMac;         // Mac адресс узла отправителя
    string outSubNet;          // Подсеть уззла отправителя
    bool tcp_type;             // Тип TCP/UPD соединение
    unsigned int size;         // Размер переданных данных
    double time;               // Время запроса
    unsigned int averageSpeed; // Сорость передачи данных
};
