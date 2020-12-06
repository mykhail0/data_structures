#include <set>
#include <algorithm>
#include <iostream>
#include <utility>

struct insert_comparator {
    bool operator()(const std::pair<int, int> &a, const std::pair<int, int> &b) const {
        // for lower_bound, so val is b
        // need to find lowest interval s. t. its second + 1 is >= x.first
        return a.second + 1 < b.first;
    }
};

struct erase_comparator {
    bool operator()(const std::pair<int, int> &a, const std::pair<int, int> &b) const {
        // for upper_bound, so val is a
        // need to find lowest interval s. t. its second is > x.first 
        return a.first <= b.second;
    }
};

/*
This is an interval set, i. e. a set of integers,
where large intervals can be stored as single elements.
Intervals stored in the set are disjoint.
*/

class iset {
  public:
    // MEMBER TYPES
    using interval = std::pair<int, int>;
    using size_type = std::set<interval>::size_type;
    using iterator = std::set<interval>::iterator;
    using const_iterator = std::set<interval>::const_iterator;
    using reverse_iterator = std::set<interval>::reverse_iterator;
    using const_reverse_iterator = std::set<interval>::const_reverse_iterator;
    // end MEMBER TYPES

  private:
    insert_comparator ins_cmp;
    erase_comparator er_cmp;

    int cardinality;
    std::set<interval> _set;

  public:
    // MEMBER FUNCTIONS
    iset() : ins_cmp{}, er_cmp{}, cardinality(0), _set{}{}
    iset(const iset&) = default;
    iset(iset&&) = default;

    iset &operator=(const iset&) = default;
    iset &operator=(iset&&) noexcept = default;
    //iset &operator=(std::initializer_list<interval> init) {_set = init};
    ~iset() = default;
    // end MEMBER FUNCTIONS

    // ITERATORS
    iterator begin() noexcept {return _set.begin();}
    const_iterator begin() const noexcept {return _set.begin();}
    const_iterator cbegin() const noexcept {return _set.cbegin();}

    iterator end() noexcept {return _set.end();}
    const_iterator end() const noexcept {return _set.end();}
    const_iterator cend() const noexcept {return _set.cend();}

    reverse_iterator rbegin() noexcept {return _set.rbegin();}
    const_reverse_iterator rbegin() const noexcept {return _set.rbegin();}
    const_reverse_iterator crbegin() const noexcept {return _set.crbegin();}

    reverse_iterator rend() noexcept {return _set.rend();}
    const_reverse_iterator rend() const noexcept {return _set.rend();}
    const_reverse_iterator crend() const noexcept {return _set.crend();}
    // end ITERATORS


    // CAPACITY
    bool empty() const noexcept {return _set.empty();}
    size_type size() const noexcept {return _set.size();}
    size_type max_size() const noexcept {return _set.max_size();}
    // end CAPACITY


    // MODIFIERS
    void clear() noexcept {_set.clear();}

    std::pair<iterator, bool> insert(const interval&);

    void erase(const interval&);

    // idk does not compile
    //iterator erase(const_iterator);
    iterator erase(iterator);
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
    const_iterator find(const interval &i) const {return _set.find(i);}

    // only in c++20
    //bool contains(const interval &i) const {return _set.contains(i);}

    std::pair<iterator, iterator> equal_range(const interval&);
    std::pair<const_iterator, const_iterator> equal_range(const interval&) const;

    iterator lower_bound(const interval&);
    const_iterator lower_bound(const interval&) const;
    iterator upper_bound(const interval&);
    const_iterator upper_bound(const interval&) const;
    // end LOOK UP

    // OPERATORS
    friend bool operator==(iset&, iset&);

    int get_cardinality() const {return cardinality;}
    void print() const;
};

bool operator==(iset&, iset&);

void iset::print() const {
    for (auto &p : _set)
        std::cout << "[" << p.first << ", " << p.second << "]" << std::endl;
}

std::pair<iset::iterator, bool> iset::insert(const iset::interval &i) {
    auto it = std::lower_bound(_set.begin(), _set.end(), i, ins_cmp);

    if (it == _set.end() || i.second < it->first) {
        cardinality += i.second - i.first + 1;
        return _set.insert(i);
    }

    auto interval = std::make_pair(std::min(i.first, it->first),
                                   std::max(i.second, it->second));

    while (it != _set.end() && it->second <= interval.second) {
        cardinality -= it->second - it->first + 1;
        it = _set.erase(it);
    }

    if (it == _set.end()) {
        cardinality += interval.second - interval.first + 1;
        return std::make_pair(_set.insert(it, interval), true);
    }

    if (it->first <= interval.second + 1) {
        interval.second = it->second;
        cardinality -= it->second - it->first + 1;
        it = _set.erase(it);
    }

    cardinality += interval.second - interval.first + 1;
    return std::make_pair(_set.insert(it, interval), true);
}

void iset::erase(const iset::interval &i) {
    auto it = std::upper_bound(_set.begin(), _set.end(), i, er_cmp);

    if (it == _set.end() || i.second < it->first)
        return;

    auto intervalL = *it;
    bool L = false;

    if (it->first < i.first) {
        intervalL.second = i.first - 1;
        L = true;
    }

    while (it != _set.end() && it->second <= i.second) {
        cardinality -= it->second - it->first + 1;
        it = _set.erase(it);
    }

    if (it != _set.end() && it->first <= i.second) {
        auto intervalR = *it;

        cardinality -= it->second - it->first + 1;
        it = _set.erase(it);

        intervalR.first = i.second + 1;
        cardinality += intervalR.second - intervalR.first + 1;
        _set.insert(it, intervalR);
    }

    if (L) {
        cardinality += intervalL.second - intervalL.first + 1;
        _set.insert(intervalL);
    }
}

iset::iterator iset::erase(iset::iterator pos) {
    cardinality -= pos->second - pos->first + 1;
    return _set.erase(pos);
}

void iset::swap(iset &s) noexcept {
    _set.swap(s._set);
}

void iset::merge(iset &s) {
    _set.merge(s._set);
}

void iset::merge(iset &&s) {
    // forward ? or move?
    _set.merge(std::move(s._set));
}

std::pair<iset::iterator, iset::iterator> iset::equal_range(const interval &i) {
    return _set.equal_range(i);
}

std::pair<iset::const_iterator, iset::const_iterator> iset::equal_range(const interval &i) const {
    return _set.equal_range(i);
}

iset::iterator iset::lower_bound(const iset::interval &i) {
    return _set.lower_bound(i);
}

iset::const_iterator iset::lower_bound(const iset::interval &i) const {
    return _set.lower_bound(i);
}

iset::iterator iset::upper_bound(const iset::interval &i) {
    return _set.upper_bound(i);
}

iset::const_iterator iset::upper_bound(const iset::interval &i) const {
    return _set.upper_bound(i);
}

bool operator==(iset &a, iset &b) {
    return a._set == b._set;
}

using namespace std;

void mal() {
    int n, updates;
    cin >> n >> updates;
    iset s;

    for (int i = 0; i < updates; ++i) {
        pair<int, int> rg;
        char c;
        cin >> rg.first >> rg.second >> c;

        if (c == 'B')
            s.insert(rg);
        else
            s.erase(rg);
        //s.print();

        cout << s.get_cardinality() << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr);
    mal();
    return 0;
}
