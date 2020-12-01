/**
 *\file BPTree.hpp
 *\author Gaia Saveri
 *\brief Class implementating a B+Tree.
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

/**
 *\tparam Tk Type of the keys in the tree.
 *\tparam Tv Tyoe of the values in the tree.
 */
template<class Tk, class Tv>
class BPTree {

public:
  using Node = node<Tk, Tv>;
  using LinkedList = List<Tv>;

private:
  /** Unique pointer to the root node. */
  std::unique_ptr<Node> root;
  /** Branching factor of the B+Tree. */
  int branchingFactor;
  /**
   *\brief Find the parent of the current node.
   *\param child Node of we want to find the parent of.
   */
  Node* findParent(Node* child);
  /**
   *\brief Auxiliary function to insert a key in an internal node
   *\param key Key of the node to be inserted.
   *\param parent Node in which to insert the key.
   *\param child Node currently containing key.
   */
  void insertInternal(Tk key, Node* parent, Node** child);
  /**
    *\brief Function to insert a new key in a leaf node of the B+Tree.
    *\param key Key to be inserted.
    *\param value Value associated to a key.
    */
  void insertFirst(Tk key, Tv value);
  /**
   *\brief Function to add a value in the linked list associated to a key already
   *present in the B+Tree.
   *\param leaf Leaf containing the key.
   *\param key Key associated to the value we want to add.
   *\param value Value we want to add in the linked list associated to the key.
   */
  void addValue(Node* leaf, Tk key, Tv value);

public:
  /**
   *\brief Default constructor.
   */
  BPTree() {
    root = nullptr;
    branchingFactor = 4;
  }
  /**
   *\brief Custom constructor.
   *\param bf Branching factor of the B+Tree.
   */
  BPTree(int bf){
    branchingFactor = bf;
  }
  /**
   *\brief Function to get the branching factor of the B+Tree.
   *\return int Branching factor.
   */
  int getBF(){
    return branchingFactor;
  }
  /**
   *\brief Function used to write a file containing all the keys in the leaves (one per line),
   *and another one with the linked lists of values (one linked list for each line).
   *
   *To the i-th line of the file containing the keys corresponds the i-th line of the file containing
   *the lists.
   */
   void writeOnFile();
  /**
   *\brief Function to search a key (witnessed in a leaf node).
   *\param key Key to be searched.
   *\return Node* Pointer to the leaf containing the key (nullptr if key not present).
   */
  Node* searchLeaf(Tk key);
  /**
   *\brief Function to retrieve the linked list of values associated to a key.
   *
   */
  void searchValues(Tk key, LinkedList& values);
  /**
   *\brief Function to insert a key an associated value in the B+Tree.
   *
   *The function first search for the key in the tree: if the key is already in the
   *tree, it adds the value to the linked list of values associated to the key.
   *If the key is not present, it adds the key-value pair in the B+Tree.
   */
  void insert(Tk key, Tv value);

#ifdef DEBUG
  /**
   *\brief Function to print the keys and associated values at the leaves' level.
   */
  void printLeaves();
  /*
   *\brief Function to print the B+tree level-wise.
   */
  void printLevels();

#endif
  /**
   *\brief Destructor for the B+Tree.
   */
  ~BPTree(){
    root.reset();
  };

};

#include"BPTree_methods.hpp"

#endif
