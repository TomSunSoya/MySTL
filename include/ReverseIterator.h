#ifndef MYSTL_REVERSEITERATOR_H
#define MYSTL_REVERSEITERATOR_H

#include <iterator>

namespace MySTL {

    template<typename Iterator>
    class ReverseIterator {
    public:
        using iterator_type = Iterator;
        using difference_type = typename std::iterator_traits<Iterator>::difference_type;
        using value_type = typename std::iterator_traits<Iterator>::value_type;
        using pointer = typename std::iterator_traits<Iterator>::pointer;
        using reference = typename std::iterator_traits<Iterator>::reference;
        using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;

        ReverseIterator();

        explicit ReverseIterator(Iterator it);

        Iterator base() const;

        reference operator*() const;

        pointer operator->() const;

        ReverseIterator &operator++();

        ReverseIterator operator++(int);

        ReverseIterator &operator--();

        ReverseIterator operator--(int);

        ReverseIterator operator+(difference_type n) const;

        ReverseIterator operator-(difference_type n) const;

        ReverseIterator &operator+=(difference_type n);

        ReverseIterator &operator-=(difference_type n);

        reference operator[](difference_type n) const;

        bool operator==(const ReverseIterator &other) const;

        bool operator!=(const ReverseIterator &other) const;

        bool operator<(const ReverseIterator &other) const;

        bool operator>(const ReverseIterator &other) const;

        bool operator<=(const ReverseIterator &other) const;

        bool operator>=(const ReverseIterator &other) const;


    private:
        Iterator current;
    };

    template<typename Iterator>
    ReverseIterator<Iterator>::ReverseIterator() : current() {

    }

    template<typename Iterator>
    ReverseIterator<Iterator>::ReverseIterator(Iterator it) : current(it) {

    }

    template<typename Iterator>
    Iterator ReverseIterator<Iterator>::base() const {
        return current;
    }

    template<typename Iterator>
    ReverseIterator<Iterator>::reference ReverseIterator<Iterator>::operator*() const {
        auto temp = current;
        return *--temp;
    }

    template<typename Iterator>
    ReverseIterator<Iterator>::pointer ReverseIterator<Iterator>::operator->() const {
        return &(operator*());
    }

    template<typename Iterator>
    ReverseIterator<Iterator> &ReverseIterator<Iterator>::operator++() {
        --current;
        return *this;
    }

    template<typename Iterator>
    ReverseIterator<Iterator> ReverseIterator<Iterator>::operator++(int) {
        auto it = *this;
        ++current;
        return it;
    }

    template<typename Iterator>
    ReverseIterator<Iterator> &ReverseIterator<Iterator>::operator--() {
        ++current;
        return *this;
    }

    template<typename Iterator>
    ReverseIterator<Iterator> ReverseIterator<Iterator>::operator--(int) {
        auto it = *this;
        --current;
        return it;
    }

    template<typename Iterator>
    ReverseIterator<Iterator> ReverseIterator<Iterator>::operator+(ReverseIterator::difference_type n) const {
        return ReverseIterator(current - n);
    }

    template<typename Iterator>
    ReverseIterator<Iterator> ReverseIterator<Iterator>::operator-(ReverseIterator::difference_type n) const {
        return ReverseIterator(current + n);
    }

    template<typename Iterator>
    ReverseIterator<Iterator> &ReverseIterator<Iterator>::operator+=(ReverseIterator::difference_type n) {
        current -= n;
        return *this;
    }

    template<typename Iterator>
    ReverseIterator<Iterator> &ReverseIterator<Iterator>::operator-=(ReverseIterator::difference_type n) {
        current += n;
        return *this;
    }

    template<typename Iterator>
    ReverseIterator<Iterator>::reference
    ReverseIterator<Iterator>::operator[](ReverseIterator::difference_type n) const {
        return *(*this + n);
    }

    template<typename Iterator>
    bool ReverseIterator<Iterator>::operator==(const ReverseIterator &other) const {
        return current == other.current;
    }

    template<typename Iterator>
    bool ReverseIterator<Iterator>::operator!=(const ReverseIterator &other) const {
        return !(*this == other);
    }

    template<typename Iterator>
    bool ReverseIterator<Iterator>::operator<(const ReverseIterator &other) const {
        return current > other;
    }

    template<typename Iterator>
    bool ReverseIterator<Iterator>::operator>(const ReverseIterator &other) const {
        return current < other;
    }

    template<typename Iterator>
    bool ReverseIterator<Iterator>::operator<=(const ReverseIterator &other) const {
        return current >= other;
    }

    template<typename Iterator>
    bool ReverseIterator<Iterator>::operator>=(const ReverseIterator &other) const {
        return current <= other;
    }

}


#endif //MYSTL_REVERSEITERATOR_H
