/**
 *\file node.hpp
 *\author Gaia Saveri
 *\brief Class describing a node of the B+Tree.
 */

#ifndef __NODE_
#define __NODE_

#include<vector>

#include"linkedlist.hpp"

/**
 *\tparam k Type of the keys of the node.
 *\tparam v Type of the values of the (leaf) node.
 */
template<class k, class v>
struct node {
  //BPTree uses leftLeaf method
  template<class Tk, class Tv> friend class BPTree;

  using LinkedList = List<v>;

  /** Discriminate between internal and leaf nodes. */
  bool leaf;
  /** Array of keys of a node (sorted, duplicates not allowed) */
  std::vector<k> keys;
  /** Unique pointer to the next leaf (only for leaf nodes) */
  std::unique_ptr<node> next;
  union{ //only one of the following will be active for each node
    /** Unique pointers to children nodes (for internal nodes) */
    std::vector<std::unique_ptr<node>> children;
    /** Pointers to linked lists of values (for leaf nodes) */
    std::vector<LinkedList*> values;
  };
  /**
    * brief Default constructor.
    */
  node() = default;

  /**
   *\brief Custom constructor.
   *\param l Boolean value specifying if we are construicting a leaf node (true)
   * or an internal node (false).
   */
  node(bool l){
    leaf = l;
    //we created a leaf
    if(l) new (&this->values) std::vector<LinkedList*>;
    //we created an internal node
    else if (!l) new (&this->children) std::vector<std::unique_ptr<node>>;
  }
  /**
   *\brief Custom constructor.
   *\param key Key to insert in the node we are constructing.
   *\param value Value to insert in the node we are contructing.
   *
   */
  node(k key, v value){
    leaf = true;
    keys.push_back(key);
    new (&this->values) std::vector<LinkedList*>;
    LinkedList* ll = new LinkedList{};
    ll->insert(value, method::push_back);
    this->values.push_back(ll);
  }
  /**
   *\brief Custom constructor.
   *\param key Key to insert in the node we are constructing.
   *\param child1 Pointer to the node that has to become child of the current node.
   *\param child2 Pointer to the node that has to become child of the current node.
   */
  node(k key, node* child1, node* child2){
    leaf = false;
    keys.push_back(key);
    new (&this->children) std::vector<std::unique_ptr<node>>;
    this->children.resize(2);
    this->children.reserve(2);
    this->children.at(0).reset(child1);
    this->children.at(1).reset(child2);
  }
  /**
   *\brief Function to find the leftmost leaf starting from the current node.
   *\return node* Pointer to the leaf we are searching for.
   */
  node* leftLeaf();
  /**
   *\brief Default destructor.
   */
  ~node(){
    this->keys.clear();
  }
};

template<class k, class v>
node<k,v>* node<k,v>::leftLeaf(){
  if(this->leaf){
    return this;
  }
  //current is not a leaf --> it has pointers to children
  for(int i=0; i<this->children.size(); i++){
    if(this->children.at(i).get()){ //it is not nullptr
      return this->children.at(i).get()->leftLeaf();
    }
  }
  return nullptr;
}

#endif
