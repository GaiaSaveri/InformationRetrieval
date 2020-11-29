#ifndef __NODE_
#define __NODE_

#include<vector>

#include"linkedlist.hpp"

template<class k, class v>
struct node {

  template<class Tk, class Tv> friend class BPTree;

  using LinkedList = List<v>;

  /** to discriminate between internal and leaf nodes */
  bool leaf;
  /** array of keys in a node (sorted, duplicates not allowed) */
  std::vector<k> keys;
  /** pointer to the next leaf (only for leaf nodes) */
  std::unique_ptr<node> next;
  union{ //only one of the following will be active for each node
    /** pointers to children nodes (for internal nodes) */
    std::vector<std::unique_ptr<node>> children;
    /** pointers to linked lists of values (for leaf nodes) */
    std::vector<LinkedList*> values;
  };

  /** default constructor */
  node() = default;

  /** custom constructor */
  node(bool l){
    leaf = l;
    //we created a leaf
    if(l) new (&this->values) std::vector<LinkedList*>;
    //we created an internal node
    else if (!l) new (&this->children) std::vector<std::unique_ptr<node>>;
  }
  /**
   *\brief custom constructor
   *\param l boolean indicating wheter the node is or isn't a leaf node
   *\param key key to insert in the node we are constructing
   *\param value value to insert in the node we are contructing
   *
   */
  node(bool l, k key, v value){
    leaf = l;
    keys.push_back(key);
    new (&this->values) std::vector<LinkedList*>;
    //LinkedList list{};
    LinkedList* ll = new LinkedList{};
    ll->insert(value, method::push_back);
    this->values.push_back(ll);
  }
  /**
   *\brief custom constructor
   *
   */
  node(bool l, k key, node* child1, node* child2){
    leaf = l;
    keys.push_back(key);
    new (&this->children) std::vector<std::unique_ptr<node>>;
    this->children.resize(2);
    this->children.reserve(2);
    this->children.at(0).reset(child1);
    this->children.at(1).reset(child2);
  }

  /**
   *\brief function to find the leftmost leaf starting from a given node
   *\param current node from which we start
   *\return node* pointer to the leaf we are searching for
   */
  node* leftLeaf();

  /** default destructor */
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
