// cust_alloc.hpp -- 
#include <cstddef>
#include <new>
template<typename T, std::size_t size>
class CustAllocator {
public:
   using value_type = T;
private:
   void* m_block; // Указатель на начало блока памяти
   T* m_current;  // Указатель на свободный текущий блок памяти
   std::size_t m_capacity; // Максимальная ёмкость блока памяти
   std::size_t m_allocated;  // Всего размещено элементов
public:
   // Конструктор по умолчанию
   CustAllocator() noexcept
      : m_block{nullptr}, m_current{nullptr}, m_capacity{size}, m_allocated{} {
         m_block = (::operator new (m_capacity * sizeof(T),
         std::align_val_t{alignof(T)}));
         m_current = static_cast<T*>(m_block);
   }

   // Конструктор копирования
   CustAllocator(const CustAllocator& original) noexcept = delete;
   
   // Конструктор перемещения
   CustAllocator(CustAllocator&& original) noexcept = delete;

   // Деструктор
   ~CustAllocator() {
      // Уничтожаем аллоцированные объекты
      if (m_block) {
         for (std::size_t i{}; i < m_allocated; ++i) {
            destroy(m_current - m_allocated + i);
         }
      }
      // Освобождаем выделенную память
      ::operator delete (m_block, std::align_val_t{alignof(T)});
   }

   template<typename U, typename... Args>
   void construct(U* p, Args&&... args) {
      new (p) U(std::forward<Args>(args)...);
   }

   template<typename U>
   void destroy(U* p) {
      p->~U();
   }

   T* allocate(std::size_t n) {
      // проверяем наличие памяти в блоке
      if (m_allocated + n > m_capacity) {
         std::cerr << "Ошибка: Не достаточно памяти!\n";
         throw std::bad_alloc{};
      }
      auto ptr = m_current;
      m_current += n;
      m_allocated += n;
      return ptr;
   }

   void deallocate(T*, std::size_t) {}

   template<typename U>
   struct rebind {
      using other = CustAllocator<U, size>;
   };
};