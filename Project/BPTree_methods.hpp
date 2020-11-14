#include<iostream>
#include<vector>
#include<set>
#include<memory>
#include<algorithm>
#include<queue>

//--------------------------------- Search method -----------------------------------//
template<class Tk, class Tv, class Tc>
typename BPTree<Tk,Tv,Tc>::Node* BPTree<Tk,Tv,Tc>::searchLeaf(Tk key){
  if(!root.get()){ //BPTree is empty
    std::cout<<"empty tree"<<std::endl;
    return nullptr;
  } else {
    Node* current = root.get();
    while(current->leaf == false){
      int idx = std::upper_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();
      current = current->ptrs.children.at(idx);
    }

    int idx = std::lower_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();

    if(idx == current->keys.size() || current->keys.at(idx)!=key){
      std::cout<<"not found"<<std::endl;
      return nullptr;
    }
    //now current is a leaf node, at index idx we have the key we are searching for
    std::cout<<"found"<<std::endl;
    return current;
  }
}

//----------------------------- Insertion methods -----------------------------------//
template<class Tk, class Tv, class Tc>
void BPTree<Tk,Tv,Tc>::insert(Tk key, Tv value){
  if(!root.get()) { //the tree is empty
    root.reset(new Node(true, key, value));
    //root is leaf now --> QUESTA PARTE POTREBBE CAMBIARE SE METTO LINKED LIST
  } else { //tree is non empty
    //starting from the root
    Node* current = root.get();
    Node* parent = nullptr;
    //search for possible position for the input key
    while(!current->leaf){ //not a leaf-->children active
      parent = current; //go a level down the tree
      int idx = std::upper_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();
      current = current->ptrs.children.at(idx);
    }
    //now current node is a leaf node
    if(current->keys.size()<branchingFactor) { //leaf is not full
      //need to find the position where to insert the key
      int i = std::upper_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();
      //insert at the end (to increase the size)
      current->keys.push_back(key);
      current->ptrs.values.push_back(value);
      //need to insert at position i
      if(i!=current->keys.size() - 1) {
        for(int j = current->keys.size()-1; j>i; j--) {
          //shifting
          current->keys.at(j) = current->keys.at(j-1);
          current->ptrs.values.at(j) = current->ptrs.values.at(j-1);
        }
        current->keys.at(i) = key;
        current->ptrs.values.at(i) = value;
      }
    } else { //if the leaf is full
      std::vector<Tk> fakeNode(current->keys);
      std::vector<Tv> fakeNodeValues(current->ptrs.values);
      //find "theorical" place for the new key
      int i = std::upper_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();
      //create space
      fakeNode.push_back(key);
      fakeNodeValues.push_back(value);

      //insert in correct position
      if(i!=fakeNode.size()-1) {
        for(int j=fakeNode.size()-1; j>i; j--) {
          fakeNode.at(j) = fakeNode.at(j-1);
          fakeNodeValues.at(j) = fakeNodeValues.at(j-1);
        }
        fakeNode.at(i) = key;
        fakeNodeValues.at(i) = value;
      }

      //need to create a new leaf since we have to split the previous one
      Node* newLeaf = new Node(true);

      //swapping pointers
      Node* tmp = current->next;
      current->next = newLeaf;
      newLeaf->next = tmp;

      //split the two leaves
      current->keys.resize((branchingFactor)/2 + 1);
      current->ptrs.values.reserve((branchingFactor)/2 + 1);

      for(int i=0; i<=(branchingFactor)/2; i++){
        current->keys.at(i) = fakeNode.at(i);
        current->ptrs.values.push_back(fakeNodeValues.at(i));
      }

      for(int i=(branchingFactor)/2+1; i<fakeNode.size(); i++) {
        newLeaf->keys.push_back(fakeNode.at(i));
        newLeaf->ptrs.values.push_back(fakeNodeValues.at(i));
      }

      //if we splitted the root, we need to create a new root
      if(current == root.get()) {
        root.release();
        Node* newRoot = new Node(false); //not a leaf
        newRoot->keys.push_back(newLeaf->keys.at(0));
        //new (&newRoot->ptrs.children) std::vector<Node*>;
        newRoot->ptrs.children.push_back(current);
        newRoot->ptrs.children.push_back(newLeaf);
        root.reset(newRoot);
      } else { //we didn't split the root
        //insert new key in the parent
        insertInternal(newLeaf->keys.at(0), &parent, &newLeaf);
      }
    }
  }
}

template<class Tk, class Tv, class Tc>
void BPTree<Tk,Tv,Tc>::insertInternal(Tk key, typename BPTree<Tk,Tv,Tc>::Node** parent, typename BPTree<Tk,Tv,Tc>::Node** child) {
  if((*parent)->keys.size()<branchingFactor-1){ //parent is not full
    int i = std::upper_bound((*parent)->keys.begin(), (*parent)->keys.end(), key) - (*parent)->keys.begin();
    //make space
    (*parent)->keys.push_back(key);
    (*parent)->ptrs.children.push_back(*child);

    if(i!=(*parent)->keys.size()-1) {
      //shifting keys
      for(int j=(*parent)->keys.size()-1; j>i; j--) {
        (*parent)->keys.at(j) = (*parent)->keys.at(j-1);
      }
      //shifiting pointers
      for(int j=(*parent)->ptrs.children.size()-1; j>(i+1); j--) {
        (*parent)->ptrs.children.at(j) = (*parent)->ptrs.children.at(j-1);
      }
      //insert in correct position
      (*parent)->keys.at(i) = key;
      (*parent)->ptrs.children.at(i+1) = *child;
    }
    } else { //we need to split also this node
      std::vector<Tk> fakeNode((*parent)->keys);
      std::vector<Node*> fakeNodeChildren((*parent)->ptrs.children);
      //find position for the key
      int i = std::upper_bound((*parent)->keys.begin(), (*parent)->keys.end(), key) - (*parent)->keys.begin();
      fakeNode.push_back(key);
      fakeNodeChildren.push_back(*child);

      if(i!=fakeNode.size()-1){
        for(int j=fakeNode.size()-1; j>i; j--){
          fakeNode.at(j) = fakeNode.at(j-1);
        }
        for(int j=fakeNodeChildren.size()-1; j>(i+1); j--){
          fakeNodeChildren.at(j) = fakeNodeChildren.at(j-1);
        }
        //insert in right position
        fakeNode.at(i) = key;
        fakeNodeChildren.at(i+2) = *child;
      }
      //actual splitting of the node
      Tk partitionKey = fakeNode.at(fakeNode.size()/2);
      int partitionIdx = fakeNode.size()/2;

      (*parent)->keys.resize(partitionIdx);
      (*parent)->ptrs.children.resize(partitionIdx + 1);
      (*parent)->ptrs.children.reserve(partitionIdx + 1);
      for(int i=0; i<partitionIdx; i++){
        (*parent)->keys.at(i) = fakeNode.at(i);
      }
      for(int i=0; i<partitionIdx + 1; i++){
        (*parent)->ptrs.children.at(i) = fakeNodeChildren.at(i);
      }

      Node* newNode = new Node(false);
      for(int i=partitionIdx; i<fakeNode.size(); i++){
        newNode->keys.push_back(fakeNode.at(i));
      }
      for(int i=partitionIdx; i<fakeNodeChildren.size(); i++){
        newNode->ptrs.children.push_back(fakeNodeChildren.at(i));
      }
      if((*parent) == root.get()){ //we splitted the root
        root.release();
        Node* newRoot = new Node(false, partitionKey, *parent, newNode);
        root.reset(newRoot);
      } else { //we splitted an internal node
        //recursive call
        insertInternal(newNode->keys.at(0), newNode->findParent(root.get(), *parent), &newNode);
      }
    }
}

//------------------------------ Print methods ------------------------------------//
template<class Tk, class Tv, class Tc>
void BPTree<Tk,Tv,Tc>::printLeaves(){
  //pointer to the leftmost leaf starting from the root
  Node* ll = root.get()->leftLeaf();

  if(!ll){ //the tree is empty
    std::cout<<"empty B+tree"<<std::endl;
  }
  while(ll){
    for(int i=0; i<ll->keys.size(); i++){
      std::cout<<"["<<ll->keys.at(i)<<","<<ll->ptrs.values.at(i)<<"] ";

    }
    ll = ll->next;
    if(ll) std::cout<<"--> ";
  }
  std::cout<<std::endl;
}

template<class Tk, class Tv, class Tc>
void BPTree<Tk,Tv,Tc>::printLevels(){
  if(root.get()==nullptr){
    std::cout<<"Empty B+Tree"<<std::endl;
  }

  std::queue<Node*> q;
  q.emplace(root.get());

  while(!q.empty()){
    int size = q.size();
    for(int i=0; i<size; i++){
      Node* u = q.front();
      q.pop();

      for(Tk val : u->keys){
        std::cout<<val<<" ";
      }
      std::cout<<"||";

      if(u->leaf!=true){
        for(Node* v: u->ptrs.children){
          q.emplace(v);
        }
      }
    }
    std::cout<<std::endl;
  }
}
