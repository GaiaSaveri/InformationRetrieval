#ifndef __NODE_
#define __NODE_

#include<vector>
#include<functional> //std::less

#include"linkedlist.hpp"

template<class k, class v>
struct node {

  template<class Tk, class Tv> friend class BPTree;

  using LinkedList = List<v>;

  /** to discriminate between internal and leaf nodes */
  bool leaf;
  /** array of keys in a node (sorted, duplicates not allowed) */
  //std::set<k, c> keys;
  std::vector<k> keys;
  /** pointer to the next leaf (only for leaf nodes) */
  node* next;
  union p { //only one of the following will be active for each node
    /** pointers to children nodes (for internal nodes) */
    std::vector<node*> children;
    /** pointers to linked lists of values (for leaf nodes) */
    std::vector<LinkedList> values;
    /** constructor for the union */
    p() {}
    /** destructor for the union */
    ~p() {
      children.clear();
      values.clear();
    }
  } ptrs;
  /** default constructor */
  node() = default;

  /** custom constructor */
  node(bool l){
    leaf = l;
    //we created a leaf
    if(l) new (&this->ptrs.values) std::vector<v>;
    //we created an internal node
    else if (!l) new (&this->ptrs.children) std::vector<node*>;
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
    new (&this->ptrs.values) std::vector<LinkedList>;
    LinkedList list{};
    list.insert(value, method::push_back);
    this->ptrs.values.push_back(list);
    next = nullptr;
  }

  /**
   *\brief custom constructor
   *
   */
  node(bool l, k key, node* child1, node* child2){
    leaf = l;
    keys.push_back(key);
    new (&this->ptrs.children) std::vector<node*>;
    this->ptrs.children.push_back(child1);
    this->ptrs.children.push_back(child2);
  }

  /**
   *\brief find the parent of the current node
   *\param current
   *\param child
   *
   *Leaves nodes are ignored (cannot be parent of any other node by definition).
   *Also parents of the leaves are ignored, this is because this function is called
   *inside the insertInternal method.
   */
  node** findParent(node* current, node* child);

  /**
   *\brief function to find the leftmost leaf starting from a given node
   *\param current node from which we start
   *\return node* pointer to the leaf we are searching for
   */
  node* leftLeaf();

  /** default destructor */
  ~node() = default;
};

//#include"node_methods.hpp"

template<class k, class v>
node<k,v>** node<k,v>::findParent(node<k,v>* current, node<k,v>* child){
  //ignore leaves and leaves' parents
  if(current->leaf || current->ptrs.children.at(0)->leaf){
    return nullptr;
  }
  static node* parent; //need to allocate this on the heap
  for(int i=0; i<current->ptrs.children.size(); i++){
    if(current->ptrs.children.at(i) == child){
      parent = current;
    } else {
      node* tmp = current->ptrs.children.at(i);
      findParent(tmp, child);
    }
  }
  return &parent;
}

template<class k, class v>
node<k,v>* node<k,v>::leftLeaf(){
  if(this->leaf){
    return this;
  }
  //current is not a leaf --> it has pointers to children
  for(int i=0; i<this->ptrs.children.size(); i++){
    if(this->ptrs.children.at(i)){ //it is not nullptr
      return this->ptrs.children.at(i)->leftLeaf();
    }
  }
  return nullptr;
}

#endif
