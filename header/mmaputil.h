#ifndef MMAP_UTIL_H
#define MMAP_UTIL_H

#include <map>
#include <iostream>
#include <vector>

template <typename KEY, class T, class Compare = std::less<KEY> >
std::ostream &operator<<(std::ostream &os, const std::multimap<KEY, T, Compare> &mm);

template <typename KEY, class T, class Compare = std::less<KEY>>
class multimap_util {
private:
    std::multimap<KEY, T, Compare> *map_;

    typedef typename std::multimap<KEY, T, Compare>::iterator MultiMapIterator;
    typedef typename std::multimap<KEY, T, Compare>::const_iterator CMultiMapIterator;

public:
    multimap_util(std::multimap<KEY, T, Compare> &map) : map_(&map) {}

    auto replace_key(const KEY &currentKey, const KEY &newKey) -> multimap_util<KEY, T, Compare>;

    auto swap(const KEY &lhs, const KEY &rhs) -> multimap_util<KEY, T, Compare>;

    auto replace_value(const T &a, const T &b) -> multimap_util<KEY, T, Compare>;

    auto erase(const T &a) -> multimap_util<KEY, T, Compare>;
};

template <typename KEY, class T, class Compare>
auto multimap_util<KEY, T, Compare>::replace_key(const KEY &currentKey, const KEY &newKey) -> multimap_util<KEY, T, Compare> {
    std::pair<MultiMapIterator, MultiMapIterator> result = map_->equal_range(currentKey);
    std::vector<T> Values;

    for (MultiMapIterator it = result.first; it != result.second; ++it) {
        Values.push_back(it->second);
    }

    map_->erase(result.first, result.second);
    //currentKey might not exist in memory anymore, do not use it ()
    //The example is quite jarring but will make one

    for (typename std::vector<KEY>::const_iterator cit = Values.begin(); cit != Values.end(); ++cit) {
        map_->insert(std::make_pair(newKey, *cit));
    }

    return *this;
}

template <typename KEY, class T, class Compare>
auto multimap_util<KEY, T, Compare>::swap(const KEY &lhs, const KEY &rhs) -> multimap_util<KEY, T, Compare> {
    if (map_->find(lhs) == map_->end() || map_->find(rhs) == map_->end()) {
        return *this;
    }

    std::pair<MultiMapIterator, MultiMapIterator> result = map_->equal_range(lhs);

    std::vector<T> ValuesOfLhs;
    for (CMultiMapIterator cit = result.first; cit != result.second; ++cit) {
        ValuesOfLhs.push_back(cit->second);
    }

    //make sure we do not delete lhs in case its location is inside map_ 
    //TODO
    map_->erase(result.first, result.second);
    replace_key(rhs, lhs);

    for (typename std::vector<KEY>::const_iterator cit = ValuesOfLhs.begin(); cit != ValuesOfLhs.end(); ++cit) {
        map_->insert(std::make_pair(rhs, *cit));
    }

    return *this;
}

template <typename KEY, class T, class Compare>
auto multimap_util<KEY, T, Compare>::replace_value(const T &a, const T &b) -> multimap_util<KEY, T, Compare>{
    for (MultiMapIterator it = map_->begin(); it != map_->end(); ++it) {
        if (it->second == a) {
            it->second = b;
        }
    }

    return *this;
}

template <typename KEY, class T, class Compare>
auto multimap_util<KEY, T, Compare>::erase(const T &a) -> multimap_util<KEY, T, Compare>{
    //To do make sure if a is part of the multi map we copy its values otherwise is possible we delete a itself
    MultiMapIterator it = map_->begin();
    while (it != map_->end()) {
        if (it->second == a) {
            map_->erase(it++);
        } else {
            ++it;
        }
    }
    return *this;
}

//print multimap as KEY Value pairs
template <typename KEY, class T, class Compare>
std::ostream &operator<<(std::ostream &os, const std::multimap<KEY, T, Compare> &mm) {
    for (typename std::multimap<KEY, T, Compare>::const_iterator cit = mm.begin(); cit != mm.end(); ++cit) {
        os << "(k: " << cit->first << ", v: " << cit->second << ")  ";
    }

    return os;
}

#endif //MMAP_UTIL_H