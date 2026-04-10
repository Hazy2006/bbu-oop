#pragma once
#include "domain.h"

template <typename T>

class DynamicVector
{
private:
    T* elems;
    int size;
    int capacity;

    void _resize() {
        this->capacity *= 2;
        T* newBlock = new T[this->capacity];
        for (int i = 0; i < this->size; ++i)
            newBlock[i] = this->elems[i];
        delete[] this->elems;
        this->elems = newBlock;
    }

public:
    DynamicVector(int initial_cap = 10) {
        this->capacity = initial_cap;
        this->size = 0;
        this->elems = new T[capacity];
    }

    DynamicVector(const DynamicVector<T>& other) : capacity(other.capacity), size(other.size) {
        this->elems = new T[other.capacity];
        for (int i = 0; i < other.size; ++i)
            this->elems[i] = other.elems[i];
    }

    DynamicVector<T>& operator=(const DynamicVector<T>& other) {
        if (this == &other) return *this;
        delete[] this->elems;
        this->elems = new T[other.capacity];
        this->capacity = other.capacity;
        this->size = other.size;
        for (int i = 0; i < this->size; ++i)
            this->elems[i] = other.elems[i];
        return *this;
    }

    void add(T element) {
        if (this->size == this->capacity)
            this->_resize();
        this->elems[this->size] = element;
        this->size++;
    }

    void remove(int index) {
        if (index < 0 || index >= this->size) return;
        for (int i = index; i < this->size - 1; ++i)
            this->elems[i] = this->elems[i + 1];
        this->size--;
    }

    void update(T element, int index) {
        if (index < 0 || index >= this->size) return;
        this->elems[index] = element;
    }

    int search(T element) {
        for (int i = 0; i < this->size; ++i)
            if (elems[i] == element)
                return i;
        return -1;
    }

    T get(int index) const {
        return this->elems[index];
    }

    int length() const {
        return this->size;
    }

    ~DynamicVector() {
        delete[] this->elems;
    }
};
