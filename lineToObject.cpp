
#include "host.h"
#include "request.h"
#include "mylibs.cpp"
#include "hostcheker.cpp"
#include "analysis.cpp"

/**
 * @description Преобразование строки лога в объект.
 */
request lineToObject(string line)
{
    request a, empty;
    a.inputHostIp = "bad";
    vector<string> tmp = split(line, ';');
    if (tmp.size() != 7)
    {
        cout << "Пропускаем строку так как она не подходит под формат: "
             << "'" + line + "'" << endl;
        return empty;
    }

    vector<string> inputIpPortVector = split(tmp.at(0), ':');
    if (inputIpPortVector.size() != 2)
    {
        cout << "Исходящий IP+Порт адрес (неправельный формат) '" << tmp.at(0) << "'" << endl;
        cout << "'" << line << "'" << endl;
        return empty;
    }

    if (!checkIp(inputIpPortVector.at(0)))
    {
        cout << "Исходящий IP адрес (неправельный формат) '" << tmp.at(0) << "'" << endl;
        cout << "'" + line + "'" << endl;
        return empty;
    }
    if (!checkPort(inputIpPortVector.at(1)))
    {
        cout << " Исходящий Порт (неправельный формат) '" << tmp.at(0) << "'" << endl;
        cout << "'" + line + "'" << endl;
        return empty;
    }

    if (!checkMac(tmp.at(1)))
    {
        cout << "Исходящий Mac не корректный (неправельный формат) '" << tmp.at(1) << "'" << endl;
        cout << "'" + line + "'" << endl;
        return empty;
    }

    vector<string> uotIpPortVector = split(tmp.at(2), ':');
    if (uotIpPortVector.size() != 2)
    {
        cout << "Входящий IP+Порт адрес (неправельный формат) '" << tmp.at(2) << "'" << endl;
        cout << "'" + line + "'" << endl;
        return empty;
    }

    if (!checkIp(uotIpPortVector.at(0)))
    {
        cout << "Входящий IP адрес (неправельный формат) '" << tmp.at(2) << "'" << endl;
        cout << "'" + line + "'" << endl;
        return empty;
    }
    if (!checkPort(uotIpPortVector.at(1)))
    {
        cout << "Входящий Порт (неправельный формат) '" << tmp.at(2) << "'" << endl;
        cout << "'" + line + "'" << endl;
        return empty;
    }

    if (!checkMac(tmp.at(3)))
    {
        cout << "Входящий Mac не корректный (неправельный формат) '" << tmp.at(3) << "'" << endl;
        cout << "'" << line << "'" << endl;
        return empty;
    }

    if (!checkTCPType(tmp.at(4)))
    {
        cout << "Неверный формат TCP соединения (true|false) (неправельный формат) '" << tmp.at(4) << "'" << endl;
        cout << "'" << line << "'" << endl;
        return empty;
    }

    if (!checkRequestSize(tmp.at(5)))
    {
        cout << "Количество отданных байт не является числом '" << tmp.at(5) << "'" << endl;
        cout << "'" << line << "'" << endl;
        return empty;
    }

    if (!is_float(tmp.at(6)))
    {
        cout << "Некорректное время выполнения запроса'" << tmp.at(6) << "'" << endl;
        cout << "'" << line << "'" << endl;
        return empty;
    }
    // Диагностическая заглушка
    // for (int j = 0; j < tmp.size(); j++)
    // {
    //     cout << j << " " << tmp.at(j) << endl;
    // }
    //cout << inputIpPortVector.at(0) << endl;
    a.inputHostIp = inputIpPortVector.at(0);
    a.inputHostPort = atoi(inputIpPortVector.at(1).c_str());
    a.inputHostMac = tmp.at(1);
    a.outHostIp = uotIpPortVector.at(0);
    a.outHostPort = atoi(uotIpPortVector.at(1).c_str());
    a.outHostMac = tmp.at(3);
    a.tcp_type = tmp.at(4) == "false";
    a.size = atoi(tmp.at(5).c_str());
    a.time = atof(tmp.at(6).c_str());
    // Странная бага с вычислением скорости запроса именно в этом месте...
    // a.averageSpeed = trunc(a.size / a.time) / 1000;
    // a.averageSpeed = a.size / a.time;
    a.outSubNet = getSubNetByIp(a.outHostIp);
    //  a.averageSpeed = round(a.size / a.time);
    // unsigned int test = trunc(t.size / t.time) / 1000;
    return a;
}
