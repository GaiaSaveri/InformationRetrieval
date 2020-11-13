/**
 *\file node.h
 *\author Gaia Saveri
 *\brief header containing the implementation of the class node
 */

#ifndef __NODE_
#define __NODE_

#include<memory> //unique_ptr
#include<vector> //std::vector
#include<set> //std::set

/**
 *\tparam Tk is the type of the keys in the nodes of the Btree
 *\tparam Tv is the type of the values associated to keys in the leaf nodes
 *\tparam Tc type of the comparison operator
 */
template<class Tk, class Tv, class Tc>
struct node {

  //Btree has to be friend .....
  tamplate<class Tk, class Tv, class Tc> friend class BPTree;

  /** array of ordered keys contained in the node. */
  std::set<Tk> keys;

  /** default constructor */
  node() = default;

  /** costum constructor */
  //to do --> when doing insert in Btree

  /** default destructor */
  virtual ~node() {}

  /** copy and move semantics */
  //to do!

  /**
   *\brief method to count the number of keys in the node
   *\return int number of keys in the current node
   */
  int keyNumber() {
    return keys.size();
  }

  virtual bool isOverflow() = 0;
  virtual bool isUnderflow() = 0;

  virtual void insertValue(Tk& key, Tv& value);
  virtual void deleteValue(Tk& key);

  virtual void merge(node& sibling);
  virtual node& split();
};

template<class Tk, class Tv, class Tc>
struct internalNode : public node {
  /** array of pointers to children nodes */
  std::vector<node*> children;

  /** constructors */
  //to do!

  bool isOverflow() override {
    return children.size() > branchingFactor;
  }

  boll isUnderflow() override {
    return children.size() < (branchingFactor + 1)/2;
  }

  //void insertValue(Tk& key, Tv& value) override;

  void merge(node& sibling) override {
    internalNode node = (internalNode) sibling;

  }


};

template<class Tk, class Tv, class Tc>
struct leafNode : public node {
};


#include"node_methods.hpp"

#endif
