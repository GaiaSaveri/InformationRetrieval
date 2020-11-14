/**
 *\file BPTree.hpp
 *\author Gaia Saveri
 *\brief header containing the implementation of the B+Tree.
 */

#ifndef __BPTREE_
#define __BPTREE_

#include<iostream>
#include<vector>
#include<memory>
#include<algorithm> //std::upper_bound
#include<queue>

#include"node.hpp"

template<class Tk, class Tv, class Tc=std::less<Tk> >
class BPTree {

public:
  using Node = node<Tk, Tv, Tc>;

private:
  /** unique pointer to the root node */
  std::unique_ptr<Node> root;
  /** branching factor of the B+Tree */
  int branchingFactor; 
  /**
   *\brief auxiliary function to insert a key in an internal node
   *\param key key of the node to be inserted
   *\param parent node in which to insert the key
   *\param child node currently containing key
   */
   void insertInternal(Tk key, Node** parent, Node** child);
public:
  /** comparison operator */
  Tc comp;

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
   *\brief function to search a key (witnessed in a leaf node)
   *\param key key to be searched
   *\return Node* pointer to the leaf containing the key (nullptr if key not present)
   */
  Node* searchLeaf(Tk key);

  /**
   *\brief function to insert a new key in a leaf node of the B+Tree.
   *\param key key to be inserted
   *\param value value associated to a key
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
   void clear() noexcept
   {
     root.reset();
   }

   /** destructor for the B+Tree */
   ~BPTree() = default;

};

#include"BPTree_methods.hpp"

#endif
