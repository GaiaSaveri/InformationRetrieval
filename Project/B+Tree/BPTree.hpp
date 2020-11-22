/**
 *\file BPTree.hpp
 *\author Gaia Saveri
 *\brief header containing the implementation of the B+Tree.
 */

#ifndef __BPTREE_
#define __BPTREE_

#include<iostream>
#include<fstream>
#include<vector>
#include<memory>
#include<algorithm> 
#include<queue>

#include"node.hpp"

template<class Tk, class Tv>
class BPTree {

public:
  using Node = node<Tk, Tv>;
  using LinkedList = List<Tv>;

private:
  /** unique pointer to the root node */
  std::unique_ptr<Node> root;
  /** branching factor of the B+Tree */
  int branchingFactor;
  /**
   *\brief find the parent of the current node
   *\param current
   *\param child
   *
   *Leaves nodes are ignored (cannot be parent of any other node by definition).
   *Also parents of the leaves are ignored, this is because this function is called
   *inside the insertInternal method.
   */
  Node* findParent(Node* child);
  /**
   *\brief auxiliary function to insert a key in an internal node
   *\param key key of the node to be inserted
   *\param parent node in which to insert the key
   *\param child node currently containing key
   */
  void insertInternal(Tk key, Node* parent, Node** child);
  /**
    *\brief function to insert a new key in a leaf node of the B+Tree.
    *\param key key to be inserted
    *\param value value associated to a key
    */
  void insertFirst(Tk key, Tv value);
  /**
   *\brief function to add a value in the linked list associated to a key already
   *present in the B+Tree.
   *\param leaf leaf containing the key
   *\param key key associated to the value we want to add
   *\param value value we want to add in the linked list associated to the key.
   */
  void addValue(Node* leaf, Tk key, Tv value);
public:

  /** default constructor */
  BPTree() {
    root = nullptr;
    branchingFactor = 4;
  }

  /** custom constructor */
  BPTree(int bf){
    branchingFactor = bf;
  }

  /**
   *\brief function to get the branching factor of the B+Tree
   *\return int branching factor
   */
  int getBF(){
    return branchingFactor;
  }
  /**
   *\brief write a file containing all the keys in the leaves (one per line),
   *and another one with the linked lists of values (one linked list for each line).
   *To i-th line of the file containing the keys corresponds the i-th line containing
   *the lists.
   */
   void writeOnFile();
  /**
   *\brief function to search a key (witnessed in a leaf node).
   *\param key key to be searched
   *\return Node* pointer to the leaf containing the key (nullptr if key not present)
   */
  Node* searchLeaf(Tk key);

  /**
   *\brief function to retrieve the linked list of values associated to a key.
   *
   */
  void searchValues(Tk key, LinkedList& values);

  /**
   *\brief function to insert a key an associated value in the B+Tree.
   *
   *The function first search for the key in the tree: if the key is already in the
   *tree, it adds the value to the linked list of value associated to the key.
   *If the key is not present, it adds the key-value pair in the B+Tree.
   */
  void insert(Tk key, Tv value);

  //MANCANO TUTTI I REMOVE!

  /**
   *\brief function to print the keys at the leaves' level.
   */
  void printLeaves();

  /*
   *\brief function to print the B+tree level-wise
   */
  void printLevels();

  /**
   *\brief function to clear the content of the tree.
   *
   *This function resets the root of the tree, so that the whole B+Tree is destroyed
   *without any memory leak.
   */
  void clear() noexcept {
     root.reset();
  }

  /** destructor for the B+Tree */
  ~BPTree() = default;

};

#include"BPTree_methods.hpp"

#endif
