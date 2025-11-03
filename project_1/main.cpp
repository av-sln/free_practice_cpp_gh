// main.cpp
// Файл исходного кода фильтрации ip-адресов

#include <iostream>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>

// Функция разбивает строку по разделителю
std::vector<std::string> split(const std::string& string, char delimiter) {
   std::vector<std::string> delString;
   std::string::size_type start{0};
   std::string::size_type stop{string.find_first_of(delimiter)};
   while (stop != std::string::npos) {
      delString.push_back(string.substr(start, stop - start));
      start = stop + 1;
      stop = string.find_first_of(delimiter, start);
   }
   delString.push_back(string.substr(start));
   return delString;
}

// Функция отображает список IP-адресов
// Один адрес в одной строке
void DisplayIP(std::vector<std::vector<std::string>>& ipPoolRef) {
   for (const auto& addressLine : ipPoolRef) {
      for (const auto& address : addressLine) {
         std::cout << address << '.';
      }
      std::cout << "\b \n";
   }
}

int main(int argc, char const *argv[]) {
   std::vector<std::vector<std::string>> ipPool{};
   for (std::string line; std::cin >> line;) {
      ipPool.push_back(split(line, '.'));
      while (std::cin.get() != '\n') {
         continue;
      }
   }

   // Выводим на печать не сортированный список ip-адресов
   DisplayIP(ipPool);

   // Обратная лексикографическая сортировка
   // Определяем именованную лямбду
   auto compareIP = [](const std::vector<std::string>& ip1,
      const std::vector<std::string>& ip2) {
         for (int i{}; i < 4; ++i) {
            int octetIp1 = std::stoi(ip1[i]);
            int octetIp2 = std::stoi(ip2[i]);
            if (octetIp1 != octetIp2) {
               return octetIp1 > octetIp2;
            }
         }
         return false;
   };
   // С помощью алгоритма сортировки сортируем ip-адреса
   std::ranges::sort(ipPool, compareIP);
   // Отображаем отсортированный список ip-адресов
   std::cout << "----------Displaying the list of IP-addresses----------\n";
   DisplayIP(ipPool);

   // Фильтрация первому байту
   auto filterOne = [](const std::vector<std::string>& ip) {
      return ip[0] == "1";
   };
   std::vector<std::vector<std::string>> temp{};
   std::copy_if(ipPool.cbegin(), ipPool.cend(), std::back_inserter(temp),
      filterOne);
   // Отображаем выбранный список
   std::cout << "----------Displaying first byte filter list----------\n";
   DisplayIP(temp);

   // Фильтрация по первому и второму байтам
   // Первый байт = 46, второй байт = 70
   auto filterOneTwo = [](const std::vector<std::string>& ip) {
      return (ip[0] == "46" && ip[1] == "70");
   };
   temp.clear();
   std::copy_if(ipPool.cbegin(), ipPool.cend(), std::back_inserter(temp),
      filterOneTwo);
   // Отображаем отфильтрованный список
   std::cout << "----------Display one and two bytes filter----------\n";
   DisplayIP(temp);

   // Фильтрация списка по любому байту, который равен 46
   temp.clear();
   std::string anyByte{"46"};
   std::copy_if(ipPool.cbegin(), ipPool.cend(), std::back_inserter(temp),
      [anyByte](const std::vector<std::string>& ip)
      {return (ip[0] == anyByte or ip[1] == anyByte 
         or ip[2] == anyByte or ip[3] == anyByte);});
   // Отображаем отфильтрованный список
   std::cout << "----------Displaying any byte filtering----------\n";
   DisplayIP(temp);
}   