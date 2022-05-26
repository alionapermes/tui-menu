#pragma once

#include <cstddef>
#include <cstdint>
#include <cmath>
#include <iterator>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>


template <typename Value = int>
class htab
{
public:
    struct htab_iterator;

private:
    struct item;

public:
    using value_type       = Value;
    using item_type        = item;
    using size_type        = size_t;
    using reference        = value_type&;
    using pointer          = value_type*;
    using const_reference  = const value_type&;
    using const_pointer    = const value_type*;
    using iterator         = htab_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;

private:
    enum class item_state : char
    {
        empty  = 0,
        used   = 1,
        unused = 2,
    };

    struct item
    {
    private:
        Value _value;
        item_state _state = item_state::empty;

    public:
        item() : _state(item_state::empty) {}

        item(
            value_type&& value,
            item_state state = item_state::empty
        )
            : _value(std::move(value))
            , _state(state)
        {}

        reference
        value()
        { return _value; }

        void
        set(value_type value)
        {
            _value = value;
            _state = item_state::used;
        }

        void
        set_state(item_state state)
        { _state = state; }

        item_state
        state() const
        { return _state; }
    };

public:
    struct htab_iterator
    {
        friend htab;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;
        using value_type        = Value;

    private:
        item_type* _ptr = nullptr;

    public:
        htab_iterator() = default;

        htab_iterator(item_type* ptr) : _ptr(ptr) {}

        iterator&
        operator++()
        {
            while ((++_ptr)->state() != item_state::used);
            return *this;
        }

        iterator
        operator++(int)
        {
            iterator old(_ptr);
            ++(*this);
            return old;
        }

        iterator&
        operator--()
        {
            while ((--_ptr)->state() != item_state::used);
            return *this;
        }

        iterator
        operator--(int)
        {
            iterator old(_ptr);
            --(*this);
            return old;
        }

        reference
        operator*()
        { return _ptr->value(); }

        pointer
        operator->()
        { return &_ptr->value(); }

        bool
        operator==(const iterator& other) const
        { return other._ptr == _ptr; }
    };

private:
    size_type _cap     = 0;
    size_type _size    = 0;
    item_type* _data   = nullptr;

public:
    htab() : _cap(1000) {}

    ~htab()
    { clear(); }

    void
    clear()
    {
        delete[] _data;
        _data = nullptr;
        _size = 0;
    }

    size_type
    size() const
    { return _size; }

    bool
    empty() const
    { return _size == 0; }

    template <typename T>
    iterator
    insert(T&& value)
    {
        if (_data == nullptr) {
            _data = new item_type[_cap + 1]();
            _data[_cap].set_state(item_state::used);
        }

        iterator iter;
        std::pair<iterator, bool> res;

#ifdef LINEAR
        res  = linear_insert(value);
        iter = res.first;
#elif defined QUADRATIC
        res  = quadratic_insert(value);
        iter = res.first;
#endif

        if (iter != end() && res.second)
            ++_size;
        return iter;
    }

    template <typename T>
    void
    erase(T&& value)
    { return erase(find(std::forward<value_type>(value))); }

    void
    erase(iterator pos)
    { pos._ptr->set_state(item_state::unused); }

    iterator
    find(value_type value)
    {
        size_type index;

#ifdef LINEAR
        index = linear_find(value);
#elif defined QUADRATIC
        index = quadratic_find(value);
#endif

        return (index < _cap ? iterator(_data + index) : end());
    }

    iterator
    begin()
    {
        if (empty())
            return end();

        iterator iter(_data);

        if (_data->state() == item_state::used)
            return iter;
        return ++iter;
    }

    iterator
    end()
    { return iterator(_data + _cap); }

    reverse_iterator
    rbegin()
    {
        if (empty())
            return rend();

        iterator iter(_data + _cap - 1);

        if (iter._ptr->state() == item_state::used)
            return reverse_iterator(iter);
        return reverse_iterator(--iter);
    }

    reverse_iterator
    rend()
    { return reverse_iterator(begin()); }

    size_type
    get_hash(reference value)
    {
#ifdef MULTIPLICATIVE
        return multiplicative_hashing(value);
#elif defined MODULE
        uint64_t key;

        if (value > 100'000) {
    #ifdef CHOOSING_NUMBER
            key = choosing_number(value);
    #elif defined CONVOLUTION
            key = convolution(value);
    #endif
        } else
            key = value;

        return module_hashing(key);
#endif
    }

#ifdef LINEAR
    std::pair<iterator, bool>
    linear_insert(reference value, size_type try_n = 0)
    {
        size_type index = get_hash(value) + try_n;

        if (index >= _cap)
            return std::make_pair(end(), false);

        if (_data[index].state() == item_state::empty) {
            _data[index].set(value);
            return std::make_pair(iterator(_data + index), true);
        }

        if (_data[index].value() == value) {
            if (_data[index].state() == item_state::unused)
                _data[index].set_state(item_state::used);
            return std::make_pair(iterator(_data + index), false);
        }

        return linear_insert(value, try_n + 1);
    }

    size_type
    linear_find(reference value, size_type try_n = 0)
    {
        size_type index = get_hash(value) + try_n;

        if (index + try_n == _cap)
            return _cap;

        if (_data[index].state() == item_state::used
            && _data[index].value() == value
        ) { return index; }

        return linear_find(value, try_n + 1);
    }
#elif defined QUADRATIC
    std::pair<iterator, bool>
    quadratic_insert(reference value, size_t try_n = 0)
    {
        static const int c1 = 3;
        static const int c2 = 8;

        size_type index =
            (get_hash(value) + (c1 * try_n) + (c2 * try_n * try_n)) % _cap;

        if (index >= _cap)
            return std::make_pair(end(), false);

        if (_data[index].state() == item_state::empty) {
            _data[index].set(value);
            return std::make_pair(iterator(_data + index), true);
        }

        if (_data[index].value() == value) {
            if (_data[index].state() == item_state::unused)
                _data[index].set_state(item_state::used);
            return std::make_pair(iterator(_data + index), false);
        }

        return quadratic_insert(value, try_n + 1);
    }

    size_type
    quadratic_find(reference value, size_type try_n = 0)
    {
        size_type index = get_hash(value) + try_n;

        if (index + try_n == _cap)
            return _cap;

        if (_data[index].state() == item_state::used
            && _data[index].value() == value
        ) { return index; }

        return quadratic_find(value, try_n + 1);
    }
#endif

private:
#ifdef CHOOSING_NUMBER
    uint64_t
    choosing_number(uint64_t num)
    {
        int n1 = 0, n2 = 0, n3 = 0;
        
        for (int counter = 0; num > 0; ++counter) {
            int tail = num % 10;

            if (counter < 1)
                n1 += tail;
            else if (counter < 6)
                n2 += tail;
            else
                n3 += tail;

            num /= 10;
        }

        return (((n3 % 10) * 100) + ((n2 % 10) * 10) + (n1 % 10));
    }
#endif

#ifdef CONVOLUTION
    uint64_t
    convolution(uint64_t num)
    {
        std::vector<int> parts;
        int tmp = 0;

        for (int counter = 0; num > 0; ++counter) {
            int tail = num % 10;

            if (counter == 3) {
                parts.push_back(tmp);
                tmp     = 0;
                counter = 0;
            }

            tmp += tail * std::pow(10, counter);
            num /= 10;
        }

        int xor_n = 1;
        for (auto& p : parts)
            tmp += p ^ xor_n++;

        return tmp % 1'000;
    }
#endif

#ifdef MODULE
    size_type
    module_hashing(std::uint64_t key)
    { return (key % _cap); }
#endif

#ifdef MULTIPLICATIVE
    inline float
    fract(float num)
    { return num - static_cast<int>(num); }

    uint64_t
    horner(std::string_view sw)
    {
        size_type digit = sw.length() - 1;
        uint64_t nat    = 0;

        for (auto ltr : sw)
            nat += static_cast<uint64_t>(ltr) * (10 * digit--);
        return nat;
    }

    uint64_t
    multiplicative_hashing(std::string key)
    {
        static const float A = (std::sqrt(5) - 1) / 2;
        return _cap * fract(horner(key) * A);
    }
#endif
};

