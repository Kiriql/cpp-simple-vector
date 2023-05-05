#pragma once

#include "array_ptr.h"

#include <cassert>
#include <initializer_list>
#include <stdexcept>

// класс-обёртка для работы функции Reserve
class ReserveProxyObj {
public:
    ReserveProxyObj(size_t capacity)
        : capacity_(capacity)
    {
    }

    size_t GetCapacity() {
        return capacity_;
    }

private:
    size_t capacity_;
};

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size)
        : SimpleVector(size, Type())
    {
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value)
        : items_(size), size_(size), capacity_(size)
    {
        std::fill(items_.Get(), items_.Get() + size, value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init)
        : items_(init.size()), size_(init.size()), capacity_(init.size())
    {
        std::copy(init.begin(), init.end(), items_.Get());
    }

    // конструктор копирования
    SimpleVector(const SimpleVector& other)
        : items_(other.capacity_), size_(other.size_)
    {
        std::copy(other.begin(), other.end(), items_.Get());
    }

    // конструктор перемещения
    SimpleVector(SimpleVector&& other)
    {
        swap(other);
    }

    // Конструктор с вызовом функции Reserve
    explicit SimpleVector(ReserveProxyObj obj)
    {
        Reserve(obj.GetCapacity());
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (&items_ != &rhs.items_) {
            SimpleVector copy{rhs};
            swap(copy);
        }
        return *this;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_) throw std::out_of_range("Out of range");
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_) throw std::out_of_range("Out of range");
        return items_[index];
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return items_.Get() + size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return items_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return items_.Get() + size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return begin();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return end();
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    // void Fill(Iterator first, Iterator last) {
    //     assert(first < last);

    //     std::generate(first, last, std::move(Type()));
    // }

    void Resize(size_t new_size) {                                                             
        if (new_size == size_) return;
        
        if (new_size < size_) {
            size_ = new_size;
            return;
        }
        
        if (new_size > capacity_) {
            Reserve(std::max(new_size, 2 * capacity_));
        }
        
        for (auto it = &items_[size_]; it != &items_[new_size]; ++it) {
            *(it) = Type{};
        }
        
        size_ = new_size;
    } 

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
     void PushBack(const Type& item) {
        if (size_ < capacity_) {
            items_[size_] = item;
        } else {
            auto new_capacity = Reserve(std::max(size_t(1), 2 * capacity_)); //защита, если capacity_=0
            ArrayPtr<Type> arr_ptr(new_capacity);
            std::copy(&items_[0], &items_[size_], &arr_ptr[0]);
            arr_ptr[size_] = item;
            items_.swap(arr_ptr);
            capacity_ = new_capacity;
        }
        ++size_;
    }
    
    void PushBack(Type&& item) {
        if (size_ < capacity_) {
            items_[size_] = std::move(item);
        } else {
            Reserve(std::max(size_t(1), 2 * capacity_)); 
            items_[size_] = std::move(item);
            // auto new_capacity = std::max(size_t(1), 2 * capacity_); //защита, если capacity_=0
            // Reserve(new_capacity);
            // std::move(&items_[0], &items_[size_], &new_capacity[0]);
            // new_capacity[size_] = std::move(item);
            // items_.swap(new_capacity);
            // capacity_ = new_capacity;
        }
        ++size_;
    }


     void insertItemAtPos(size_t pos, const Type& value, ArrayPtr<Type>& arr_ptr) {
        arr_ptr[pos] = value;
    }
    void insertItemAtPos(size_t pos, Type&& value, ArrayPtr<Type>& arr_ptr) {
        arr_ptr[pos] = std::move(value);
    }

    Iterator Insert(ConstIterator pos, Type&& value) { 
        assert(pos >= cbegin() && pos <= cend()); 
        
        auto no_const_pos = const_cast<Iterator>(pos); 
        auto pos_element = std::distance(begin(), no_const_pos); 
        
        if (size_ < capacity_) { 
            std::move_backward(no_const_pos, end(), &items_[(size_ + 1)]); 
            insertItemAtPos(pos_element, std::move(value), items_);
        } else { 
            auto new_capacity = std::max(size_t(1), 2 * capacity_); //защита, если capacity_=0 
            ArrayPtr<Type> arr_ptr(new_capacity); 
            std::move(&items_[0], &items_[pos_element], &arr_ptr[0]); 
            std::move_backward(no_const_pos, end(), &arr_ptr[(size_ + 1)]); 
            insertItemAtPos(pos_element, std::move(value), arr_ptr);
            items_.swap(arr_ptr); 
            capacity_ = new_capacity; 
        } 
        
        ++size_; 
        return Iterator{&items_[pos_element]}; 
    }
    
    Iterator Insert(ConstIterator pos, const Type& value) { 
        assert(pos >= cbegin() && pos <= cend()); 
        
        auto no_const_pos = const_cast<Iterator>(pos); 
        auto pos_element = std::distance(begin(), no_const_pos); 
        
        if (size_ < capacity_) { 
            std::move_backward(no_const_pos, end(), &items_[(size_ + 1)]); 
            insertItemAtPos(pos_element, value, items_);
        } else { 
            auto new_capacity = std::max(size_t(1), 2 * capacity_); //защита, если capacity_=0 
            ArrayPtr<Type> arr_ptr(new_capacity); 
            std::move(&items_[0], &items_[pos_element], &arr_ptr[0]); 
            std::move_backward(no_const_pos, end(), &arr_ptr[(size_ + 1)]); 
            insertItemAtPos(pos_element, value, arr_ptr);
            items_.swap(arr_ptr); 
            capacity_ = new_capacity; 
        } 
        
        ++size_; 
        return Iterator{&items_[pos_element]}; 
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        assert(!IsEmpty());
        --size_;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        assert(pos != this->end());

        size_t count = pos - items_.Get();
        std::move(items_.Get() + count + 1, items_.Get() + size_, items_.Get() + count);
        --size_;

        return &items_[count];
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        items_.swap(other.items_);
    }

    void Reserve(size_t new_capacity) {
    if(new_capacity > capacity_){
        ArrayPtr<Type> arr_ptr(new_capacity);
        std::move(&items_[0], &items_[size_], &arr_ptr[0]);
        items_.swap(arr_ptr);
        capacity_ = new_capacity;
        }
    }

private:
    ArrayPtr<Type> items_;
    size_t size_ = 0;
    size_t capacity_ = 0;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) return false;
    return std::equal(lhs.begin(),lhs.end(),rhs.begin());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs <= rhs);
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
}