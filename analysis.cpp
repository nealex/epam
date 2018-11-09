#include <algorithm>

/**
 * @description Метод определения, является ли ip адресс уникальным в списке.
 */
bool isUniqHost(string ip, map<string, int> &uHost, int uniqCount)
{
    map<string, int>::iterator it = uHost.begin();
    if (uniqCount == 0)
    {
        return false;
    }
    it = uHost.find(ip);
    return (it != uHost.end());
}

/**
 * @description Получение подсети по ip адресу
 */
string getSubNetByIp(string ip)
{
    vector<string> sip = split(ip, '.');
    return sip[0] + '.' + sip[1] + '.' + sip[2];
}

/**
 * @description Метод для определения, является ли подсеть уникальной в списке или нет.
 */
// bool isUniqSubNet(string subnet, vector<subNetStruct> &sNet)
// {
//     vector<subNetStruct>::iterator it = sNet.begin();

//     it = find(sNet.begin(), sNet.end(), 30);
//     if(*it.count){
//         return true;
//     }else{
//         return false;
//     }
//     //return (it != sNet.end());
//     // it = sNet.find(subnet);
//     // else{
//     //     return
//     // }
//     // return (it != sNet.end());
// }