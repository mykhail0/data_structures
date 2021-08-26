/*
First line is i.
If instead of line there is a point,
we don't know its relation to the end of the other line.
1)  ---
    --

2)   .
    ---

or    .
    ---

3)     --
    --

4)  --
    --

or  -
    --

5)  .
      ---

6)  --
       --
*/

std::pair<iset::iterator, bool> iset::insert(const iset::interval_t &i) {
    if (_set.empty())
        return _set.insert(i);

    auto it = _set.lower_bound(i);

    if (it == _set.end()) {
        --it;
        if (i.first == it->first) {
            // 1)
            _set.erase(it);
            return _set.insert(i);
        }
        if (i.first <= it->second) {
            // 2)
            bool inserted = i.second > it->second;
            if (inserted) {
                auto tmp = it->first;
                it = _set.erase(it);
                it = _set.insert(it, {tmp, i.second});
            }
            return {it, inserted};
        }
        assert(i.first > it->second);
        return {_set.insert(_set.end(), i), true};
    }

    if (i.first == it->first)
        // 4)
        return {it, false};

    // 5)
    auto a = i.first, b = i.second;
    if (it != _set.begin()) {
        // Exists a previous interval.
        --it;
        if (it->second >= a) {
            a = it->first;
            it = _set.erase(it);
        } else {
            ++it;
        }
    }

    while (it != _set.end() && it->first <= b) {
        b = std::max(b, it->second);
        it = _set.erase(it);
    }

    // 6)
    return _set.insert({a, b});
}
