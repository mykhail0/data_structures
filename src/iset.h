#ifndef INTERVAL_SET
#define INTERVAL_SET

#include <set>

/*
This is an interval set, i. e. a set of integers,
where large intervals can be stored as single elements.
Intervals stored in the set are disjoint.
*/

class iset {
  protected:
    std::set<interval> _set;
  public:
    // MEMBER FUNCTIONS
    // CONSTRUCTORS
    iset() = default;
    iset(const iset&) = default;
    iset(iset&&) = default;

    template<class InputIt>
    iset(InputIt, InputIt);
    iset(std::initializer_list<interval>);
    // end CONSTRUCTORS

    iset &operator=(const iset&) = default;
    iset &operator=(iset&&) noexcept = default;
    iset &operator=(std::initializer_list<interval>);
    ~iset() = default;
    // end MEMBER FUNCTIONS

    // MEMBER TYPES
    using interval = std::pair<int, int>;
    using size_type = std::set<interval>::size_type;
    using iterator = std::set<interval>::iterator;
    using const_iterator = std::set<interval>::const_iterator;
    using reverse_iterator = std::set<interval>::reverse_iterator;
    using const_reverse_iterator = std::set<interval>::const_reverse_iterator;
    // end MEMBER TYPES

    // ITERATORS
    iterator begin() noexcept {return _set.begin();}
    const_iterator begin() const noexcept {return _set.begin();}
    const_iterator cbegin() const noexcept {return _set.cbegin();}

    iterator end() noexcept {return _set.end();}
    const_iterator end() const noexcept {return _set.end();}
    const_iterator cend() const noexcept {return _set.cend();}

    reverse_iterator rbegin() noexcept {return _set.rbegin();}
    const_reverse_iterator rbegin() const noexcept {return _set.rbegin();}
    const_iterator crbegin() const noexcept {return _set.crbegin();}

    reverse_iterator rend() noexcept {return _set.rend();}
    const_reverse_iterator rend() const noexcept {return _set:rend();}
    const_iterator crend() const noexcept {return _set.crend();}
    // end ITERATORS


    // CAPACITY
    bool empty() const noexcept {return _set.empty();}
    size_type size() const noexcept {return _set.size();}
    size_type max_size() const noexcept {return _set.max_size();}
    // end CAPACITY


    // MODIFIERS
    void clear() noexcept {_set.clear();}

    std::pair<iterator, bool> insert(const interval&);
    iterator insert(const_iterator, const interval&);
    template<class InputIt>
    void insert(InputIt, InputIt);
    void insert(std::initializer_list<interval>);

    size_type erase(const interval&);

    iterator erase(const_iterator pos) {return _set.erase(pos);}
    iterator erase(iterator pos) {return _set.erase(pos);}
    iterator erase(const_iterator first, const_iterator last) {
        return _set.erase(first, last);
    }

    void swap(iset&) noexcept;
    void merge(iset&);
    void merge(iset&&);
    // end MODIFIERS


    // LOOK UP
    size_type count(const interval &i) const {return _set.count(i);}

    iterator find(const interval &i) {return _set.find(i);}
    const_iterator find(const interval &i) const {return _set.find(i);;

    bool contains(const interval &i) const {return _set.contains(i);}

    std::pair<iterator, iterator> equal_range(const interval&);
    std::pair<const_iterator, const_iterator> equal_range(const interval&) const;

    iterator lower_bound(const interval&);
    const_iterator lower_bound(const interval&) const;
    iterator upper_bound(const interval&);
    const_iterator upper_bound(const interval&) const;
    // end LOOK UP

    // OPERATORS
    friend bool operator==(iset&, iset&);

    // below, split, join
    // could utilize C++14 find for this
    bool contains_interval(const interval&);
    iterator find_container_interval(const interval&);
    const_iterator find_container_interval(const interval&);
};

bool operator==(iset&, iset&);

template <>
void swap<iset>(iset&, iset&) noexcept;

#endif /* INTERVAL_SET */
