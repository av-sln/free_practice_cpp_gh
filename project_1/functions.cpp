// functions.cpp
#include <iostream>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <cctype>

// Функция разбивает строку по разделителю
// Здесь возвращаемое значение std::vector<std::string>
auto split(const std::string &string, char delimiter)
{
   std::vector<std::string> delString{};
   if (string == "") return delString;
   std::string::size_type start{0};                     // Стартовая позиция
   std::string::size_type stop{string.find(delimiter)}; // Позиция останова
   while (stop != std::string::npos)
   {
      delString.emplace_back(string.substr(start, stop - start));
      start = stop + 1;
      stop = string.find(delimiter, start);
   }
   delString.emplace_back(string.substr(start));
   return delString;
}

// Проверяем исходный вектор на корректность ip-адресов
// если ip-адрес не корректен удаляем его из списка
auto invalidIP = [](const std::vector<std::string> &ipLines)
{
   // Проверяем количество октетов
   if (ipLines.size() != 4)
      return true;
   // Проверяем каждый октет на содержание только цифр
   for (const auto &octet : ipLines)
   {
      if (std::ranges::any_of(octet,
                              [](char ch)
                              { return !std::isdigit(ch); }))
         return true;
      // Проверка октетов на соответствие диапазону [0-255]
      if (std::stoi(octet) < 0 || std::stoi(octet) > 255)
         return true;
   }
   return false;
};

// Обратная лексикографическая сортировка
// Определяем именованную лямбду
auto compareIP = [](const std::vector<std::string> &ip1,
                    const std::vector<std::string> &ip2)
{
   for (int i{}; i < 4; ++i)
   {
      int octetIp1 = std::stoi(ip1[i]);
      int octetIp2 = std::stoi(ip2[i]);
      if (octetIp1 != octetIp2)
      {
         return octetIp1 > octetIp2;
      }
   }
   return false;
};

// Фильтрация по первому и второму байтам
// Первый байт = 46, второй байт = 70
auto filterOneTwo = [](const std::vector<std::string> &ip)
{
   return (ip[0] == "46" && ip[1] == "70");
};

// Фильтрация списка по любому байту, который равен 46
auto anyByteFilter = [](const std::vector<std::string> &ip)
{
   return (ip[0] == "46" || ip[1] == "46" || ip[2] == "46" || ip[3] == "46");
};