/*
 * custom_map.h
 *
 *  Created on: 30/09/2014
 *      Author: Scott Rasmussen (ZaitaNZ)
 *
 *  Description:
 *
 */

#ifndef CUSTOM_MAP_H_
#define CUSTOM_MAP_H_

#include <vector>
#include <map>
#include <set>
#include <iostream>

using namespace std;

template<typename K, typename V>
class custom_map {
public:
  /**
   * This is the iterator class. It allows us seemless interaction with the C++
   * language features (e.g for(auto iter: Map) etc)
   */
  class Iterator {
  public:
    /**
     * Constructor
     *
     * @param order_of_insertion reference to our vector containing the order of insertion
     * @param map Reference to the actual map containing the data
     */
    Iterator(custom_map<K, V>* parent) : parent_(parent) {
      order_iter_ = parent_->order_of_insertion_.begin();
    }

    void end() { order_iter_ = parent_->order_of_insertion_.end(); }

    /**
     * Handle the escape operator
     */
    std::pair<const K, V>* operator*() {
      return static_cast<std::pair<const K, V>*>(&(*parent_->map_.find( (*order_iter_))));
    }
    std::pair<const K, V>* operator->() {
      return static_cast<std::pair<const K, V>*>(&(*parent_->map_.find( (*order_iter_))));
    }

    /**
     * Handle the increment operator when incrementing and expecting an
     * Iterator back
     */
    Iterator& operator++() {
      (*this)++;
      return (*this);
    }

    /**
     * Handle the standard increment operator
     */
    void operator++(int) {
      if (order_iter_ != parent_->order_of_insertion_.end())
        order_iter_++;
    }

    /**
     * Handle the comparison operators
     */
    bool operator==(Iterator iter) { return this->order_iter_ == iter.order_iter_; }
    bool operator!=(Iterator iter) { return this->order_iter_ != iter.order_iter_; }

  private:
    // Members
    custom_map* parent_;

    typename std::vector<K>::iterator order_iter_;
  };

  /**
   * Implement the [] operator for getting or retreiving values
   */
  V& operator[](const K&& key) {
    if (map_.find(key) != map_.end())
      return map_[key];

    order_of_insertion_.push_back(key);
    return map_[key];
  }

  /**
   * Size
   */
  unsigned size() {
    return order_of_insertion_.size();
  }

  /**
   * Standard iterators
   */
  Iterator begin() {
    return Iterator(this);
  }
  Iterator end() {
    Iterator newIter(this);
    newIter.end();
    return newIter;
  }

private:
  // members
  std::map<K, V>    map_;
  std::vector<K>    order_of_insertion_;
};

#endif /* CUSTOM_MAP_H_ */
