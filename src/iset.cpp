#include "iset.h"

// TODO
template<class InputIt>
iset(InputIt, InputIt) {
}

// TODO
iset(std::initializer_list<interval>) {
}

// TODO
std::pair<iterator, bool> insert(const interval&);
iterator insert(const_iterator, const interval&);
template<class InputIt>
void insert(InputIt, InputIt);
void insert(std::initializer_list<interval>);

// TODO
size_type erase(const interval&);

void swap(iset &s) noexcept {
    _set.swap(i._set);
}
void merge(iset &s) {
    _set.merge(s);
}
void merge(iset &&s) {
    // forward ? czy move?
    _set.merge(std::move(s._set));
}

// TODO
std::pair<iterator, iterator> equal_range(const interval&);
std::pair<const_iterator, const_iterator> equal_range(const interval&) const;

// TODO
iterator lower_bound(const interval&);
const_iterator lower_bound(const interval&) const;
iterator upper_bound(const interval&);
const_iterator upper_bound(const interval&) const;

// below, split, join
// could utilize C++14 find for this
// TODO
bool contains_interval(const interval&);
iterator find_container_interval(const interval&);
const_iterator find_container_interval(const interval&);

bool operator==(iset &a, iset &b) {
    return a._set == b._set;
}

template <>
void swap<iset>(iset &a, iset &b) noexcept {
    a.swap(b);
}
