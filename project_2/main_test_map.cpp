#include <utility>
#include <iostream>
#include <map>
#include <format>
#include "stateful_alloc.hpp"

// Вычисление факториала
int factorial(int n) {
   int result{1};
   for (int i{2}; i <= n; ++i) {
      result *= i;
   }
   return result;
}

int main() {
      
   // Заполняем и выводим map со стандартным аллокатором
   {
      auto std_map = std::map<int, int, std::less<int>>{};
      for (int i = 0; i < 10; ++i) {
         std_map[i] = factorial(i);
      }
      // Вывод контейнера на печать
      for (const auto& pair : std_map) {
         std::cout << std::format("{} {}\n", pair.first, pair.second);
      }
   }

   // Заполняем и выводим map с кастомным аллокатором
   {
      std::cout << std::endl;
      using CustomAllocator = StatefulAllocator<std::pair<const int, int>, 10>;
      std::map<int, int, std::less<int>, CustomAllocator> cust_map;
      for (int i = 0; i <= 9; ++i) {
         cust_map[i] = factorial(i);
      }
      // Вывод контейнера на печать
      for (const auto& pair : cust_map) {
         std::cout << std::format("{} {}\n", pair.first, pair.second);
      }
   }

   // Тестируем копирование (оба контейнера используют один пул памяти)
   {
      std::cout << std::endl;
      using CustomAllocator = StatefulAllocator<std::pair<const int, int>, 10>;
      std::map<int, int, std::less<int>, CustomAllocator> cust_map;
      for (int i{}; i <= 4; ++i) {
         cust_map[i] = factorial(i);
      }
      // Вывод контейнера на экран
      std::cout << "Оригинал:\n";
      for (const auto& pair : cust_map) {
         std::cout << std::format("{} {}\n", pair.first, pair.second);
      }
      // Создаём копию в том же пуле памяти
      auto copy_map(cust_map);
      // Вывод контейнера на экран
      std::cout << "Копия:\n";
      for (const auto& pair : copy_map) {
         std::cout << std::format("{} {}\n", pair.first, pair.second);
      }
   }

   // Тестируем перемещение
   {
      std::cout << std::endl;
      using Allocator = StatefulAllocator<std::pair<const int, int>, 12>;
      std::map<int, int, std::less<int>, Allocator> cust_map;
      for (int i{}; i < 12; ++i) {
         cust_map[i] = factorial(i);
      }
      // Вывод на экран
      std::cout << "оригинал:\n";
      for (const auto& pair : cust_map) {
         std::cout << std::format("{} {}\n", pair.first, pair.second);
      }

      std::map<int, int, std::less<int>, Allocator> move_map;
      move_map = std::move(cust_map);
      // Вывод на экран
      std::cout << "Копия:\n";
      for (const auto& pair : move_map) {
         std::cout << std::format("{} {}\n", pair.first, pair.second);
      }
      std::cout << "Ёмкость оригинала: " << cust_map.size() << std::endl;
   }

   // тестирование операции swap
   {
      std::cout << std::endl;
      using Allocator = StatefulAllocator<std::pair<const int, int>, 10>;
      std::map<int, int, std::less<int>, Allocator> map_1;
      for (int i{}; i < 5; ++i) {
         map_1[i] = factorial(i);
      }
      std::cout << "map_1:\n";
      for (const auto& pair : map_1) {
         std::cout << std::format("{} {}\n", pair.first, pair.second);
      }
      std::map<int, int, std::less<int>, Allocator> map_2;
      for (int i{5}; i < 10; ++i) {
         map_2[i] = factorial(i);
      }
      std::cout << "map_2:\n";
      for (const auto& pair : map_2) {
         std::cout << std::format("{} {}\n", pair.first, pair.second);
      }
      // swap
      map_1.swap(map_2);
      std::cout << "После swap:\n";
      std::cout << "map_1:\n";
      for (const auto& pair : map_1) {
         std::cout << std::format("{} {}\n", pair.first, pair.second);
      }
      std::cout << "map_2:\n";
      for (const auto& pair : map_2) {
         std::cout << std::format("{} {}\n", pair.first, pair.second);
      }
   }
}
