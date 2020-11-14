#ifndef __NODE_
#define __NODE_

#include<vector>
//#include<set>
#include<functional> //std::less

template<class k, class v, class c=std::less<k> >
struct node {

  template<class Tk, class Tv, class Tc> friend class BPTree;

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
    /** pointers to values (for leaf nodes) */
    //PROBAIBLMENTE DA CAMBIARE CON UN POINTER AD UNA LINKED LIST!
    std::vector<v> values;

    p() {}

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
    new (&this->ptrs.values) std::vector<v>;
    this->ptrs.values.push_back(value);
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

#include"node_methods.hpp"

#endif
