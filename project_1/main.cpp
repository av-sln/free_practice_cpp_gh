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
   std::string::size_type stop{string.find(delimiter)};
   while (stop != std::string::npos) {
      delString.emplace_back(string.substr(start, stop - start));
      start = stop + 1;
      stop = string.find(delimiter, start);
   }
   delString.emplace_back(string.substr(start));
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
      ipPool.emplace_back(split(line, '.'));
      while (std::cin.get() != '\n') {
         continue;
      }
   }

   // Проверяем исходный вектор на корректность ip-адресов
   // если ip-адрес не корректен удаляем его из списка
   auto IPCheck = [](std::vector<std::string>& ipLines) {
      for (const auto& octet : ipLines) {
            if (!(std::stoi(octet) >= 0 && std::stoi(octet) <= 255)) {
               return true;
            }
      }
      return false;      
   };
   std::erase_if(ipPool, IPCheck);
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
   DisplayIP(ipPool);

   // Фильтрация первому байту
   std::vector<std::vector<std::string>> temp{};
   std::ranges::copy_if(ipPool, std::back_inserter(temp), 
      [](const std::vector<std::string>& ip){return ip[0] == "1";});
   // Отображаем выбранный список
   DisplayIP(temp);

   // Фильтрация по первому и второму байтам
   // Первый байт = 46, второй байт = 70
   auto filterOneTwo = [](const std::vector<std::string>& ip) {
      return (ip[0] == "46" && ip[1] == "70");
   };
   temp.clear();
   std::ranges::copy_if(ipPool, std::back_inserter(temp), filterOneTwo);
   // Отображаем отфильтрованный список
   DisplayIP(temp);

   // Фильтрация списка по любому байту, который равен 46
   auto anyByteFilter = [](const std::vector<std::string>& ip) {
      return (ip[0] == "46" || ip[1] == "46" 
            || ip[2] == "46" || ip[3] == "46");
   };
   temp.clear();
   std::string anyByte{"46"};
   std::ranges::copy_if(ipPool, std::back_inserter(temp), anyByteFilter);
   // Отображаем отфильтрованный список
   DisplayIP(temp);
}