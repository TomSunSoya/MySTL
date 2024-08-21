#ifndef MYSTL_DEQUE_H
#define MYSTL_DEQUE_H

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <stdexcept>

#include "ReverseIterator.h"

namespace MySTL {

    template<typename T>
    class Deque final {
        static constexpr int ELEMENTS_PER_BLOCK = 16;

        struct Block {
            T *elements;
            Block *prev;
            Block *next;

            Block()
                    : elements(new T[ELEMENTS_PER_BLOCK]),
                      prev(nullptr),
                      next(nullptr) {}

            ~Block() { delete[] elements; }
        };

    public:
        class Iterator {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T *;
            using reference = T &;

            explicit Iterator(Block **blockMap = nullptr, size_t blockIndex = 0,
                              size_t index = 0)
                    : blockMap(blockMap), blockIndex(blockIndex), index(index) {}

            reference operator*() const {
                return blockMap[blockIndex]->elements[index];
            }

            pointer operator->() { return &(operator*()); }

            Iterator &operator++() {
                ++index;
                if (index >= ELEMENTS_PER_BLOCK) {
                    ++blockIndex;
                    index = 0;
                }
                return *this;
            }

            Iterator operator++(int) {
                auto temp = *this;
                ++(*this);
                return temp;
            }

            Iterator &operator--() {
                if (index == 0) {
                    --blockIndex;
                    index = ELEMENTS_PER_BLOCK - 1;
                } else
                    --index;
                return index;
            }

            Iterator operator--(int) {
                auto temp = *this;
                --(*this);
                return temp;
            }

            Iterator operator+(difference_type n) const {
                auto temp = *this;
                difference_type offset = n + index;
                temp.blockIndex += offset / ELEMENTS_PER_BLOCK;
                temp.index = offset % ELEMENTS_PER_BLOCK;
                return temp;
            }

            Iterator operator-(difference_type n) const {
                auto temp = *this;
                difference_type offset =
                        (index - n + ELEMENTS_PER_BLOCK) % ELEMENTS_PER_BLOCK;
                temp.blockIndex -= offset / ELEMENTS_PER_BLOCK;
                temp.index = offset % ELEMENTS_PER_BLOCK;
                return temp;
            }

            difference_type operator-(const Iterator &other) const {
                return (blockIndex - other.blockIndex) * ELEMENTS_PER_BLOCK +
                       index - other.index;
            }

            bool operator==(const Iterator &other) const {
                return blockMap == other.blockMap &&
                       blockIndex == other.blockIndex && index == other.index;
            }

            bool operator!=(const Iterator &other) const {
                return !(*this == other);
            }

            reference operator[](difference_type n) const { return *(*this + n); }

            Iterator &operator+=(difference_type n) {
                *this = *this + n;
                return *this;
            }

        private:
            Block **blockMap;
            size_t blockIndex;
            size_t index;
        };

        Deque();

        ~Deque();

        Deque(const Deque<T> &other);

        Deque(Deque<T> &&other) noexcept;

        Deque<T> &operator=(const Deque<T> &other);

        Deque<T> &operator=(Deque<T> &&other) noexcept;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t size() const;

        [[nodiscard]] size_t max_size() const;

        void clear();

        T &operator[](size_t index);

        const T &operator[](size_t index) const;

        T &at(size_t index);

        const T &at(size_t index) const;

        T &front();

        const T &front() const;

        T &back();

        const T &back() const;

        void push_back(const T &value);

        void push_front(const T &value);

        void pop_back();

        void pop_front();

        template<typename... Args>
        void emplace_back(Args &&...args);

        template<typename... Args>
        void emplace_front(Args &&...args);

        void insert(size_t index, const T &value);

        void erase(size_t index);

        using iterator = Iterator;
        using const_iterator = const Iterator;
        using reverse_iterator = ReverseIterator<iterator>;
        using const_reverse_iterator = ReverseIterator<const_iterator>;

        iterator begin();

        iterator end();

        const_iterator cbegin();

        const_iterator cend();

        reverse_iterator rbegin();

        reverse_iterator rend();

        const_reverse_iterator crbegin();

        const_reverse_iterator crend();

    private:
        Block **map;
        size_t mapSize;
        size_t first;
        size_t last;
        size_t len;
        size_t cap;
        size_t firstIndex{};
        size_t lastIndex{};

        Iterator start{};
        Iterator finish{};

        void expandMap();
    };

    template<typename T>
    Deque<T>::Deque()
            : map(new Block *[1]),
              mapSize(1),
              first(0),
              last(0),
              len(0),
              cap(ELEMENTS_PER_BLOCK) {
        map[0] = new Block();
        ++mapSize;
        start = Iterator(&(map[0]->elements[ELEMENTS_PER_BLOCK / 2]));
        finish = Iterator(&(map[0]->elements[ELEMENTS_PER_BLOCK / 2]));
        first = ELEMENTS_PER_BLOCK / 2;
        last = ELEMENTS_PER_BLOCK / 2;
    }

    template<typename T>
    Deque<T>::~Deque() {
        for (size_t i = 0; i < mapSize; ++i) {
            delete[] map[i];
        }
        delete[] map;
    }

    template<typename T>
    void Deque<T>::expandMap() {
        auto newMapSize = mapSize * 2;
        auto newMap = new Block *[newMapSize];
        std::copy(map, map + mapSize, newMap);
        delete[] map;
        map = newMap;
        mapSize = newMapSize;
        start = Iterator(&(map[first]->elements[firstIndex]));
        finish = Iterator(&(map[last]->elements[lastIndex]));
    }

    template<typename T>
    Deque<T>::iterator Deque<T>::begin() {
        return Deque::start;
    }

    template<typename T>
    Deque<T>::iterator Deque<T>::end() {
        return Deque::finish + 1;
    }

    template<typename T>
    Deque<T>::const_iterator Deque<T>::cbegin() {
        return Deque::start;
    }

    template<typename T>
    Deque<T>::const_iterator Deque<T>::cend() {
        return Deque::finish + 1;
    }

    template<typename T>
    Deque<T>::reverse_iterator Deque<T>::rbegin() {
        return Deque::finish;
    }

    template<typename T>
    Deque<T>::reverse_iterator Deque<T>::rend() {
        return Deque::start - 1;
    }

    template<typename T>
    Deque<T>::const_reverse_iterator Deque<T>::crbegin() {
        return Deque::finish;
    }

    template<typename T>
    Deque<T>::const_reverse_iterator Deque<T>::crend() {
        return Deque::start - 1;
    }

    template<typename T>
    void Deque<T>::erase(size_t index) {
        if (index >= size()) throw std::out_of_range("Index is out of bounds.");

        size_t blockIndex = index / ELEMENTS_PER_BLOCK;
        size_t inBlockIndex = index % ELEMENTS_PER_BLOCK;

        std::copy(map[blockIndex]->elements + inBlockIndex + 1,
                  map[blockIndex]->elements + ELEMENTS_PER_BLOCK,
                  map[blockIndex]->elements + inBlockIndex);
        for (size_t i = blockIndex; i < last; ++i) {
            map[i]->elements[ELEMENTS_PER_BLOCK - 1] = map[i + 1]->elements[0];
            std::copy(map[i + 1]->elements + 1,
                      map[i + 1]->elements + ELEMENTS_PER_BLOCK,
                      map[i + 1]->elements);
        }

        --lastIndex;
        if (lastIndex < 0 && last) {
            lastIndex = ELEMENTS_PER_BLOCK - 1;
            delete[] map[last];
            map[last--] = nullptr;
        }
        --finish;
        --len;
    }

    template<typename T>
    void Deque<T>::insert(size_t index, const T &value) {
        if (index > size()) throw std::out_of_range("Index is out of bounds.");

        auto blockIndex = index / ELEMENTS_PER_BLOCK;
        auto inBlockIndex = index % ELEMENTS_PER_BLOCK;

        if (lastIndex == ELEMENTS_PER_BLOCK - 1) {
            if (last + 1 == mapSize) {
                expandMap();
            }
            map[++last] = new Block();
        }

        for (size_t i = last; i > blockIndex; --i) {
            map[i]->elements[0] = map[i - 1]->elements[ELEMENTS_PER_BLOCK - 1];
            std::copy_backward(map[i - 1]->elements,
                               map[i - 1]->elements + ELEMENTS_PER_BLOCK - 1,
                               map[i - 1]->elements + ELEMENTS_PER_BLOCK);
        }
        std::copy_backward(map[blockIndex]->elements + inBlockIndex,
                           map[blockIndex]->elements + lastIndex,
                           map[blockIndex]->elements + lastIndex + 1);
        map[blockIndex]->elements[inBlockIndex] = value;
        ++lastIndex;
        ++finish;
        ++len;
    }

    template<typename T>
    template<typename... Args>
    void Deque<T>::emplace_front(Args &&...args) {
        if (firstIndex > 0) {
            --firstIndex;
            std::copy_backward(map[first]->elements + firstIndex,
                               map[first]->elements + ELEMENTS_PER_BLOCK - 1,
                               map[first]->elements + ELEMENTS_PER_BLOCK);
        } else {
            if (first == 0) {
                auto newMapSize = mapSize * 2;
                auto newMap = new Block *[newMapSize];
                std::copy(map, map + mapSize, newMap + 1);
                delete[] map;
                mapSize = newMapSize;
                first = 1;
            }
            --first;
            map[first] = new Block();
            firstIndex = 0;
        }
        map[first]->elements[firstIndex] = T(std::forward<Args>(args)...);
        start = Iterator(&(map[first]->elements[firstIndex]));
        ++len;
    }

    template<typename T>
    template<typename... Args>
    void Deque<T>::emplace_back(Args &&...args) {
        if (lastIndex == ELEMENTS_PER_BLOCK - 1) {
            if (last + 1 >= mapSize) {
                auto newMapSize = mapSize * 2;
                auto newMap = new Block *[newMapSize];
                std::copy(map, map + mapSize, newMap);
                delete[] map;
                map = newMap;
                mapSize = newMapSize;
            }
            ++last;
            lastIndex = 0;
        } else
            ++lastIndex;
        map[last]->elements[lastIndex] = T(std::forward<Args>(args)...);
        finish = Iterator(&(map[last]->elements[lastIndex]));
        ++len;
    }

    template<typename T>
    void Deque<T>::pop_front() {
        if (len == 0) return;
        if (firstIndex == ELEMENTS_PER_BLOCK - 1) {
            if (first == last) {
                // initialize
                firstIndex = ELEMENTS_PER_BLOCK / 2;
                lastIndex = ELEMENTS_PER_BLOCK / 2;
            } else {
                delete map[first];
                ++first;
                firstIndex = 0;
            }
        } else {
            ++firstIndex;
        }
        --len;
    }

    template<typename T>
    void Deque<T>::pop_back() {
        if (len == 0) return;
        if (lastIndex == 0) {
            if (first == last) {
                // initialize
                firstIndex = ELEMENTS_PER_BLOCK / 2;
                lastIndex = ELEMENTS_PER_BLOCK / 2;
            } else {
                delete map[last];
                --last;
                lastIndex = ELEMENTS_PER_BLOCK - 1;
            }
        } else {
            --lastIndex;
        }
        --len;
    }

    template<typename T>
    void Deque<T>::push_front(const T &value) {
        if (firstIndex > 0) {
            --firstIndex;
            std::copy_backward(map[first]->elements + firstIndex,
                               map[first]->elements + ELEMENTS_PER_BLOCK - 1,
                               map[first]->elements + ELEMENTS_PER_BLOCK);
        } else {
            if (first == 0) {
                auto newMapSize = mapSize * 2;
                auto newMap = new Block *[newMapSize];
                std::copy(map, map + mapSize, newMap + 1);
                delete[] map;
                mapSize = newMapSize;
                first = 1;
            }
            --first;
            map[first] = new Block();
            firstIndex = 0;
        }
        map[first]->elements[firstIndex] = value;
        start = Iterator(&(map[first]->elements[firstIndex]));
        ++len;
    }

    template<typename T>
    void Deque<T>::push_back(const T &value) {
        if (lastIndex == ELEMENTS_PER_BLOCK - 1) {
            if (last == mapSize - 1) {
                auto newMapSize = mapSize * 2;
                auto newMap = new Block *[newMapSize];
                std::copy(map, map + mapSize, newMap);
                delete[] map;
                map = newMap;
                mapSize = newMapSize;
                map[last] = new Block();
            } else {
                ++last;
            }
            lastIndex = 0;
        } else
            ++lastIndex;
        map[last]->elements[lastIndex] = value;
        finish = Iterator(&(map[last]->elements[lastIndex]));
        ++len;
    }

    template<typename T>
    const T &Deque<T>::back() const {
        return map[last]->elements[lastIndex];
    }

    template<typename T>
    T &Deque<T>::back() {
        return map[last]->elements[lastIndex];
    }

    template<typename T>
    const T &Deque<T>::front() const {
        return map[first]->elements[firstIndex];
    }

    template<typename T>
    T &Deque<T>::front() {
        return map[first]->elements[firstIndex];
    }

    template<typename T>
    const T &Deque<T>::at(size_t index) const {
        return map[first + index / ELEMENTS_PER_BLOCK]
                ->elements[index % ELEMENTS_PER_BLOCK];
    }

    template<typename T>
    T &Deque<T>::at(size_t index) {
        return map[first + index / ELEMENTS_PER_BLOCK]
                ->elements[index % ELEMENTS_PER_BLOCK];
    }

    template<typename T>
    const T &Deque<T>::operator[](size_t index) const {
        return at(index);
    }

    template<typename T>
    T &Deque<T>::operator[](size_t index) {
        return at(index);
    }

    template<typename T>
    void Deque<T>::clear() {
        for (size_t i = 0; i < mapSize; ++i) {
            delete map[i];
        }
        delete[] map;
        map = new Block *[1];
        map[0] = new Block();
        mapSize = 1;
        first = 0;
        last = 0;
        len = 0;
        cap = ELEMENTS_PER_BLOCK;
        start = Iterator(&(map[0]->elements[ELEMENTS_PER_BLOCK / 2]));
        finish = Iterator(&(map[0]->elements[ELEMENTS_PER_BLOCK / 2]));
        firstIndex = ELEMENTS_PER_BLOCK / 2;
        lastIndex = ELEMENTS_PER_BLOCK / 2;
    }

    template<typename T>
    size_t Deque<T>::max_size() const {
        return mapSize * ELEMENTS_PER_BLOCK;
    }

    template<typename T>
    size_t Deque<T>::size() const {
        return len;
    }

    template<typename T>
    bool Deque<T>::empty() const {
        return len == 0;
    }

    template<typename T>
    Deque<T> &Deque<T>::operator=(Deque<T> &&other) noexcept {
        if (this != &other) {
            for (size_t i = 0; i < mapSize; ++i) delete[] map[i];
            delete[] map;

            map = other.map;
            mapSize = other.mapSize;
            first = other.first;
            last = other.last;
            firstIndex = other.firstIndex;
            lastIndex = other.lastIndex;
            start = other.start;
            finish = other.finish;

            other.map = nullptr;
            other.mapSize = 0;
            other.first = 0;
            other.last = 0;
            other.firstIndex = 0;
            other.lastIndex = 0;
            other.start = Iterator();
            other.finish = Iterator();
        }
        return *this;
    }

    template<typename T>
    Deque<T> &Deque<T>::operator=(const Deque<T> &other) {
        if (this != &other) {
            for (size_t i = 0; i < mapSize; ++i) delete[] map[i];
            delete[] map;

            mapSize = other.mapSize;
            map = new Block *[mapSize];
            for (size_t i = 0; i < mapSize; ++i) {
                map[i] = new Block();
                std::copy(other.map[i]->elements,
                          other.map[i]->elements + ELEMENTS_PER_BLOCK,
                          map[i]->elements);
            }

            first = other.first;
            last = other.last;
            firstIndex = other.firstIndex;
            lastIndex = other.lastIndex;
            len = other.len;
            start = Iterator(&(map[first]->elements[firstIndex]));
            finish = Iterator(&(map[last]->elements[lastIndex]));
        }
        return *this;
    }

    template<typename T>
    Deque<T>::Deque(Deque<T> &&other) noexcept {
        map = other.map;
        mapSize = other.mapSize;
        first = other.first;
        last = other.last;
        firstIndex = other.firstIndex;
        lastIndex = other.lastIndex;
        start = other.start;
        finish = other.finish;

        other.map = nullptr;
        other.mapSize = 0;
        other.first = 0;
        other.last = 0;
        other.firstIndex = 0;
        other.lastIndex = 0;
        other.start = Iterator();
        other.finish = Iterator();
    }

    template<typename T>
    Deque<T>::Deque(const Deque<T> &other) {
        mapSize = other.mapSize;
        map = new Block *[mapSize];
        for (size_t i = 0; i < mapSize; ++i) {
            map[i] = new Block();
            std::copy(other.map[i]->elements,
                      other.map[i]->elements + ELEMENTS_PER_BLOCK,
                      map[i]->elements);
        }

        first = other.first;
        last = other.last;
        firstIndex = other.firstIndex;
        lastIndex = other.lastIndex;
        len = other.len;
        start = Iterator(&(map[first]->elements[firstIndex]));
        finish = Iterator(&(map[last]->elements[lastIndex]));
    }
}  // namespace MySTL

#endif  // MYSTL_DEQUE_H
