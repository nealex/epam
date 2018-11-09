#include <iostream>
#include <cstring>
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <typeinfo>
#include <math.h>
#include <algorithm>
#include <set>
#include <ctime> 
#include "subnet.h"
#include "lineToObject.cpp"

using namespace std;

/**
 * @description функция чтения из файла и последующего анализа строк
 */ 
bool reader(char *FULL_FILE_PATH)
{

    unsigned int start_time =  clock(); // начальное время
    char buf[BUFSIZ];   // Буфер для считывания текущей строки
    fpos_t pos; // Указатель на позицию в файле.
    FILE *fp; // Указатель на файл для чтения
    long unsigned int max_size_ip = 0;

    map<string, int> UniqHost;            // Список уникальных хостов
    map<string, unsigned int> topSubNet; // Список подсетей и количество запросов в них.

    vector<request> TopHost;    // Список топ10 хостов по активности
    int UniqHostCount = 0;      // Количество уникальных хостов

    unsigned int averageSpeed = 0.0; // Средняя скорость
    unsigned int averageSize = 0; // Средний объем переданных данных
    map<string, map<string, map<string, vector<bool> > > > proxy;       // Дерево с запросами к хостам

    unsigned int countUdp = 0;      // Количество Udp пакетов
    unsigned int SumSpeedUdp = 0.0; // Сумма скоростей udp соединений

    unsigned int countTcp = 0;      // Количество Tcp пакетов
    unsigned int SumSpeedTcp = 0.0; // Сумма скоростей Tcp соединений

    if (NULL == (fp = fopen(FULL_FILE_PATH, "r")))
    {
        fprintf(stderr, "Cannot find file: %s\n", FULL_FILE_PATH);
        exit(1);
    }
    // Читает с начала файла, но можно и с самого конца(Режим для аля сокета.)
    if (0 != fseek(fp, 0, 0 /*SEEK_END*/))
    {
        fprintf(stderr, "Cannot read file: %s\n", FULL_FILE_PATH);
        exit(EXIT_FAILURE);
    }

    if (-1 == fgetpos(fp, &pos))
    {
        fprintf(stderr, "Cannot read file: %s\n", FULL_FILE_PATH);
        exit(EXIT_FAILURE);
    }

    for (unsigned int __iterator = 0;;)
    {
        //Диагностическая заглушка
        // if (__iterator >= 10)
        // {
        //     break;
        // }
        fgets(buf, BUFSIZ, fp);
        if (ferror(fp))
        {
            fprintf(stderr, "Error reading file: %s\n", FULL_FILE_PATH);
            exit(EXIT_FAILURE);
        }

        // Вывожу результаты пока здесь, но можно предусмотреть вариант вывода в отдельном потоке, процессе и т.д.
        if (feof(fp))
        {            
            cout << "1. Уникальных узлов : " << UniqHostCount << endl;
            cout << "2. Средняя скорость сети " << averageSize / __iterator << " байт/c" << endl;
            cout << "3. Средняя скорость TCP соединения: " << (SumSpeedTcp / countTcp) << " байт/c"
                 << " Средняя скорость UDP соединения: " << (SumSpeedUdp / countUdp) << " байт/c" << endl;
            cout << "4. 10 узлов сети с самой высокой средней скоростью передачи данных: " << endl;
            for (int k = 0; k < TopHost.size(); k++)
            {
                string ip = TopHost.at(k).outHostIp;
                for (int d = TopHost.at(k).outHostIp.length(); d < 15; d++)
                {
                    ip += " ";
                }

                std::cout << "   " << ip << "\t" << TopHost.at(k).averageSpeed << " байт/c" << '\n';
            }
            cout << "5. 10 самых активных подсетей /24 (A.B.C.xxx): " << endl;

            vector<subNet> tmpVectorSubnet;
            for (std::map<string, unsigned int>::iterator it = topSubNet.begin(); it != topSubNet.end(); ++it)
            {
                subNet t;
                t.value = it->first;
                t.count = it->second;

                tmpVectorSubnet.push_back(t);
            }
            std::sort(tmpVectorSubnet.begin(), tmpVectorSubnet.end(), maxSubNet);
            int c = 0;
            for (std::vector<subNet>::iterator it = tmpVectorSubnet.begin(); it != tmpVectorSubnet.end(); ++it, c++)
            {
                if (c >= 10)
                {
                    break;
                }

                string s = (*it).value + ".XXX";
                for (int d = s.length(); d < 15; d++)
                {
                    s += " ";
                }

                cout << "   " << s << "\t" << (*it).count << endl;
                     
            }
            cout << "6. Список прокси серверов в сети:" << endl;
            // IP 1
             short int isProxy=0;
            for (map<string, map<string, map<string, vector<bool> > > >::iterator f_it = proxy.begin(); f_it != proxy.end(); ++f_it, isProxy= 0)
            {
                //cout<< "# "<<(*f_it).first<<endl;
                // Protocol
                for (map<string, map<string, vector<bool> > >::iterator s_it = ((*f_it).second).begin(); s_it != ((*f_it).second).end(); ++s_it)
                {
                    //cout<< "\t: "<<(*s_it).first<<endl;
                    // Если хостов по протаколу было меньше 2, то пропускаем этот протокол 
                    if(((*s_it).second).size()<2){
                            break;
                        }
                    // IP 2          
                    for(map<string, vector<bool> >::iterator t_it = ((*s_it).second).begin(); t_it  != ((*s_it).second).end();  ++t_it )
                    {
                        //cout<< "\t\t* "<<(*t_it).first<<" r: "<<std::set<bool>( ((*t_it).second).begin(), ((*t_it).second).end() ).size()<<endl;
                        // Если запросов Входящих/Исходящих с конкретным хостом по конкретному протокоду было меньше 2 , то пропускаем этот хост
                        if(((*t_it).second).size()<2){
                            break;
                        }
                        // Если уникальных записей в сете 2, значит узел передал и принимал данные
                        if(std::set<bool>( ((*t_it).second).begin(), ((*t_it).second).end() ).size()==2){
                            isProxy++;
                        }

                        if(isProxy>=2){
                            cout <<"Прокси сервер: "<<  (*f_it).first<<endl;
                            break;
                        }
                    }
                   if(isProxy){
                       break;
                   }
                }
            }

            cout<<endl<<"Затрачено времени:"<<   ((clock() - start_time)/1000.0)<<endl;
            exit(EXIT_FAILURE);
            fsetpos(fp, &pos);
            clearerr(fp);
            sleep(1);
            continue;
        }
        fgetpos(fp, &pos);
        // Вывод строки для теста...
        // printf("%s", buf);
        request t = lineToObject(buf);
        if (t.inputHostIp == "bad")
        {
            fflush(stdout);
            continue;
        }
        t.i = __iterator;

        // Задание #1
        if (!isUniqHost(t.inputHostIp, UniqHost, UniqHostCount))
        {
            UniqHost[t.inputHostIp] = UniqHostCount;
            UniqHostCount++;
        }
        if (!isUniqHost(t.outHostIp, UniqHost, UniqHostCount))
        {
            UniqHost[t.outHostIp] = UniqHostCount;
            UniqHostCount++;
        }

        // Задание #2
        averageSize += t.size / t.time;
        t.averageSpeed = t.size / t.time;

        // Задание #3
        if (t.tcp_type)
        {
            countUdp++;
            SumSpeedUdp += t.averageSpeed;
        }
        else
        {
            countTcp++;
            SumSpeedTcp += t.averageSpeed;
        }

        // Задание #4
        std::sort(TopHost.begin(), TopHost.end(), MaxRequest);
        if (TopHost.size() < 10)
        {
            TopHost.push_back(t);
        }
        else
        {
            for (int k = TopHost.size() - 1; k != 0; k--)
            {
                if (TopHost.at(k).averageSpeed < t.averageSpeed)
                {
                    bool uniq = true;
                    for (int p = 0; p < TopHost.size(); p++)
                    {
                        if (TopHost.at(p).inputHostIp == t.inputHostIp &&
                            TopHost.at(p).inputHostPort == t.inputHostPort &&
                            TopHost.at(p).inputHostMac == t.inputHostMac &&
                            TopHost.at(p).outHostIp == t.outHostIp &&
                            TopHost.at(p).outHostPort == t.outHostPort &&
                            TopHost.at(p).outHostMac == t.outHostMac &&
                            TopHost.at(p).size == t.size &&
                            TopHost.at(p).time == t.time)
                        {
                            uniq = false;
                        }
                    }
                    if (uniq)
                    {
                        TopHost[k] = t;
                        break;
                    }
                }
            }
            //Не знаю почему, но с итератором работает медленее... 
            // PS Оставил на всякий случай.
            // int c=0;
            // for(vector<request>::iterator it =TopHost.begin() ; it<TopHost.end(); ++it, c++ ){
            //             if ((*it).inputHostIp == t.inputHostIp &&
            //                 (*it).inputHostPort == t.inputHostPort &&
            //                 (*it).inputHostMac == t.inputHostMac &&
            //                 (*it).outHostIp == t.outHostIp &&
            //                 (*it).outHostPort == t.outHostPort &&
            //                 (*it).outHostMac == t.outHostMac &&
            //                 (*it).size == t.size &&
            //                 (*it).time == t.time)
            //             {
            //                 TopHost[c]=t;
            //             }
            // }

        }
        // Задание #5
        topSubNet[t.outSubNet]++;

        // Задание #6
        string tcp_type = t.tcp_type ? "udp" : "tcp";
        proxy[t.outHostIp][tcp_type][t.inputHostIp].push_back(false);// 0 - получение 1 - отправка
        proxy[t.inputHostIp][tcp_type][t.outHostIp].push_back(true);// 0 - получение 1 - отправка
        __iterator++;
    }
}
