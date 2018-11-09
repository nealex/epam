/**
 * @description Метод разбиения строки на части (Строка в Вектор)
 */ 
std::vector<std::string> split(std::string line, char delimotor)
{
    vector<string> tmp;
    string tmp_word;
    int iter = 0;
    while (iter <= strlen(line.c_str()))
    {
        char ch = line[iter];
        if (ch == delimotor || ch == '\0')
        {
            tmp.push_back(tmp_word);
            tmp_word = "";
        }
        else
        {
            tmp_word += ch;
        }
        iter++;
    }
    return tmp;
}
/**
 * @description Функция для проверки, является ли строка числом.
 */ 
bool is_number(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

/**
 * @description Функция для проверки является ли строка вещественным числом
 */ 
bool is_float(const std::string s)
{
    int iter = 0;
    while (iter <= strlen(s.c_str()))
    {
        if ((char)s[iter] != '.' && !std::isdigit(s[iter]) && (char)s[iter] != '\0' && (char)s[iter] != '\n')
        {

            cout << s << "'" << (char)s[iter] << "' " << iter;
            exit(EXIT_FAILURE);
            return false;
        }
        iter++;
    }
    return true;
}

bool maxSubNet(subNet i, subNet j) { 
    return (i.count > j.count); 
}

bool MaxRequest(request i, request j) {
    return (i.averageSpeed > j.averageSpeed);
}

