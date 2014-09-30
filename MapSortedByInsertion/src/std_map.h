/*
 * std_map.h
 *
 *  Created on: 30/09/2014
 *      Author: Scott Rasmussen (ZaitaNZ)
 *
 *  Description:
 *  This is an implementation of a customer compare function that
 *  will ensure the objects put in the map are stored by order
 *  of insertion
 */
#ifndef STD_MAP_H_
#define STD_MAP_H_

#include <map>
#include <vector>
#include <string>

struct cmp_by_insertion {
  std::vector<std::string> current_keys_;

  bool operator() (std::string const &a, std::string const &b) {
    unsigned a_index = current_keys_.size() + 10;
    unsigned b_index = current_keys_.size() + 10;

    for (unsigned i = 0; i < current_keys_.size(); ++i) {
      if (current_keys_[i] == a)
        a_index = i;
      if (current_keys_[i] == b)
        b_index = i;
    }

    if (a_index > current_keys_.size())
      current_keys_.push_back(a);
    if (b_index > current_keys_.size())
      current_keys_.push_back(b);

    return a_index <= b_index;
  }
};

template<typename _Key, typename _Tp>
class std_map : public std::map<_Key, _Tp, cmp_by_insertion> { };

#endif /* STD_MAP_H_ */
