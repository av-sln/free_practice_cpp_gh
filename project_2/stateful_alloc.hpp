// stateful_alloc.hpp -- заголовочный файл stateful-аллокатора

#include <cstddef>
#include <new>
#include <memory>
#include <iostream>

template<typename T, std::size_t capacity>
class StatefulAllocator {
public:
   using value_type = T;
   using propagate_on_contaoner_copy_assignment = std::true_type;
   using propagate_on_container_move_assignment = std::true_type;
   using propagate_on_contaoner_swap = std::true_type;
private:
   struct Memory {
      void* m_pool;
      T* m_current;  
      std::size_t m_capacity;
      std::size_t m_allocated;

      // Конструктор по умолчанию
      explicit Memory(std::size_t cap) noexcept
         : m_pool{nullptr}, m_current{nullptr}, m_capacity{cap},
         m_allocated{} {
            m_pool = ::operator new (m_capacity * sizeof(T),
               std::align_val_t{alignof(T)});
            m_current = static_cast<T*>(m_pool);
      } 

      // деструктор
      ~Memory() {
         ::operator delete (m_pool,
            std::align_val_t{alignof(T)});
      }

      // Копирующий конструктор запрещён
      Memory(const Memory&) = delete;

      // Конструктор копирующего присваивания запрещён
      Memory& operator=(const Memory&) = delete;

      // Перемещающий конструктор запрещён
      Memory(Memory&&) = delete;

      // Конструктор перемещающего присваивания
      Memory& operator=(Memory&&) = delete;
   };

   std::shared_ptr<Memory> memory;
public:
   // Конструктор по умолчанию
   StatefulAllocator() noexcept
      : memory{std::make_shared<Memory>(capacity)} {}

   // Копирующий конструктор
   StatefulAllocator(const StatefulAllocator&) noexcept = default;

   // Конструктор копирующего присваивания
   StatefulAllocator& operator=(const StatefulAllocator&) noexcept = default;

   // Перемещающий конструктор
   StatefulAllocator(StatefulAllocator&&) noexcept = default;

   // Конструктор пермещающего присавивания
   StatefulAllocator& operator=(StatefulAllocator&&) = default;

   // Функция construct
   template<typename U, typename... Args>
   void construct(U* p, Args&&... args) {
      new (p) U(std::forward<Args>(args)...);
   }

   // Функция destroy
   template<typename U>
   void destroy(U* p) {
      p->~U();
   }

   // Функция alocate
   T* allocate(std::size_t n) {
      if (!memory or memory->m_allocated + n > memory->m_capacity) {
         std::cerr << "Ошибка: недостаточно памяти!\n";
         throw std::bad_alloc{};
      }
      auto ptr = memory->m_current;
      memory->m_current += n;
      memory->m_allocated += n;
      return ptr;
   }

   // Функция deallocate
   void deallocate(T*, std::size_t) noexcept {}

   // Метафункция rebind
   template<typename U>
   struct rebind {
      using other = StatefulAllocator<U, capacity>;
   };

   // Перегруженный operator=
   template<typename U>
   bool operator=(const StatefulAllocator<U, capacity>& other) const noexcept {
      return memory == other.memory;
   }

   // Перегруженный operator!=
   template<typename U>
   bool operator!=(const StatefulAllocator<U, capacity>& other) const noexcept {
      return (*this == other);
   }
};