#include <iostream>
#include <string>
#include <vector>
#include <sstream>

/**
 * @description Проверка ip на корректность
 */
bool checkIp(string ip)
{
    vector<string> ipVector = split(ip, '.');

    if (ipVector.size() != 4)
    {
        return false;
    }

    for (int k = 0; k < 4; k++)
    {
        if (!is_number(ipVector.at(k)))
        {
            return false;
        }
        int node = atoi(ipVector.at(k).c_str());
        if (node > 255 || node < 0)
        {
            return false;
        }
    }

    return true;
}

/**
 * @description Проверка Порта на корректность
 */
bool checkPort(string port)
{
    if (!is_number(port))
    {
        return false;
    }
    int intPort = atoi(port.c_str());
    if (intPort <= 1 || intPort > 65535)
    {
        return false;
    }
    return true;
}

/**
 * @description Проверка Mac адреса не корректность
 * TODO: MAC проверяется только по общей длинне +/-
 */

bool checkMac(string mac)
{

    int lengthMac = strlen(mac.c_str());

    if (lengthMac > 17 || lengthMac < 6)
    {
        return false;
    }

    return true;
}

/**
 * @description Проверка Типа TCP соединения на корректность
 */

bool checkTCPType(string tcp){
    return (tcp!="false" || tcp !="true");
}


/**
 * @description Проверка количества переданнных байт
 * TODO: проверка на переполнение разрядов не провожу!!!!
 */

bool checkRequestSize(string size){
   return is_number(size);
}

/**
 * @description Проверка времени запроса
 * TODO: проверка на переполнение разрядов не провожу!!!!
 */
bool checkTime(string time){
    return is_float(time);
}
