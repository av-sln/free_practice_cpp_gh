#include <chrono>
#include <utility>
#include <iostream>
#include <map>
#include <format>
#include "cust_alloc.hpp"

int factorial(int n) {
   int result{1};
   for (int i{2}; i <= n; ++i) {
      result *= i;
   }
   return result;
}

int main() {
   using namespace std::chrono;
   constexpr std::size_t N = 10; // Максиальное количество элементов контейнера

   // Заполняем и выводим map со стандартным аллокатором
   {
      auto std_map = std::map<int, int, std::less<int>>{};
      auto pre = high_resolution_clock::now();
      for (int i = 0; i <= 9; ++i) {
         std_map[i] = factorial(i);
      }
      auto post = high_resolution_clock::now();
      auto lead_time = post - pre;
      // Вывод контейнера на печать
      for (const auto& pair : std_map) {
         std::cout << std::format("{} {}\n", pair.first, pair.second);
      }
      std::cout << "Lead time is "
         << duration_cast<microseconds>(lead_time).count()
         << std::endl;
   }

   // Заполняем и выводим map с кастомным аллокатором
   {
      std::cout << std::endl;
      using CustomAllocator = CustAllocator<std::pair<const int, int>, N>;
      std::map<int, int, std::less<int>, CustomAllocator> cust_map;
      auto pre = high_resolution_clock::now();
      for (int i = 0; i <= 9; ++i) {
         cust_map[i] = factorial(i);
      }
      auto post = high_resolution_clock::now();
      auto lead_time = post - pre;
      // Вывод контейнера на печать
      for (const auto& pair : cust_map) {
         std::cout << std::format("{} {}\n", pair.first, pair.second);
      }
      std::cout << "Lead time is " 
         << duration_cast<microseconds>(lead_time).count()
         << std::endl;
      
      // Попытка добавить 11 элемент в контейнер
      cust_map[10] = factorial(10);
   }
}