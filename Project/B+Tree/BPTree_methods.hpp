#include<iostream>
#include<fstream>
#include<vector>
#include<set>
#include<memory>
#include<algorithm>
#include<queue>

//--------------------------------- SEARCH METHODS -----------------------------------//
//---------------------------------- Search Leaf -------------------------------------//
template<class Tk, class Tv>
typename BPTree<Tk,Tv>::Node* BPTree<Tk,Tv>::searchLeaf(Tk key){
  if(!root.get()){ //BPTree is empty
    return nullptr;
  } else {
    Node* current = root.get();
    while(!current->leaf){
      int idx = std::upper_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();
      current = current->children.at(idx).get();
    }

    int idx = std::lower_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();

    if(idx == current->keys.size() || current->keys.at(idx)!=key){
      return nullptr;
    }
    //now current is a leaf node, at index idx we have the key we are searching for
    return current;
  }
}
//--------------------------------- Search Values -----------------------------------//
template<class Tk, class Tv>
void BPTree<Tk,Tv>::searchValues(Tk key, typename BPTree<Tk,Tv>::LinkedList& values){
  //search the leaf containing the key
  node<Tk,Tv>* leaf = searchLeaf(key);
  //if the key is actually present in the tree
  if(leaf){
    //index corresponding to the key we are searching for
    int i = std::lower_bound(leaf->keys.begin(), leaf->keys.end(), key) - leaf->keys.begin();
    values = leaf->values.at(i);
  } else{
    std::cout<<"the key "<<key<<" is not in the tree"<<std::endl;
  }
}
//--------------------------------- Find parent -------------------------------------//
template<class Tk, class Tv>
typename BPTree<Tk,Tv>::Node* BPTree<Tk,Tv>::findParent(typename BPTree<Tk,Tv>::Node* child){
  Node* tmp = root.get();
  Tk key = child->keys.at(0);
  bool found = 0;
  while(!found){
    int i = std::upper_bound(tmp->keys.begin(), tmp->keys.end(), key) - tmp->keys.begin();
    if(tmp->children.at(i).get() == child){
      found = 1;
    }
    if(!found) tmp = tmp->children.at(i).get();
  }
  return tmp;
}
//----------------------------- INSERTION METHODS -----------------------------------//
//------------------------------- Insert first --------------------------------------//
template<class Tk, class Tv>
void BPTree<Tk,Tv>::insertFirst(Tk key, Tv value){
  if(!root.get()) { //the tree is empty
    root.reset(new Node(true, key, value));
  } else { //tree is non empty
    //starting from the root
    Node* current = root.get();
    Node* parent = nullptr;
    //search for possible position for the input key
    while(!current->leaf){ //not a leaf-->children active
      parent = current; //go a level down the tree
      int idx = std::upper_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();
      current = current->children.at(idx).get();
    }
    LinkedList l{};
    l.insert(value, method::push_back);
    //now current node is a leaf node
    if(current->keys.size()<branchingFactor) { //leaf is not full
      //need to find the position where to insert the key
      int i = std::upper_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();
      //insert at the end (to increase the size)
      current->keys.push_back(key);
      current->values.push_back(l);
      //need to insert at position i
      if(i!=current->keys.size() - 1) {
        for(int j = current->keys.size()-1; j>i; j--) {
          //shifting
          current->keys.at(j) = current->keys.at(j-1);
          //SE BUG PERSISTE PROVA A METTERE =
          current->values.at(j) = std::move(current->values.at(j-1));
        }
        current->keys.at(i) = key;
        current->values.at(i) = std::move(l);
      }
    } else { //if the leaf is full
      std::vector<Tk> fakeNode(current->keys);
      std::vector<LinkedList> fakeNodeValues(current->values);
      //find "theorical" place for the new key
      int i = std::upper_bound(current->keys.begin(), current->keys.end(), key) - current->keys.begin();
      //create space
      fakeNode.push_back(key);
      fakeNodeValues.push_back(l);

      //insert in correct position
      if(i!=fakeNode.size()-1) {
        for(int j=fakeNode.size()-1; j>i; j--) {
          fakeNode.at(j) = fakeNode.at(j-1);
          fakeNodeValues.at(j) = std::move(fakeNodeValues.at(j-1));
        }
        fakeNode.at(i) = key;
        fakeNodeValues.at(i) = std::move(l);
      }

      //need to create a new leaf since we have to split the previous one
      Node* newLeaf = new Node(true);

      //swapping pointers
      Node* tmp = current->next.get();
      current->next.release();
      current->next.reset(newLeaf);
      newLeaf->next.reset(tmp);

      //split the two leaves
      current->keys.resize((branchingFactor)/2 + 1);
      current->values.reserve((branchingFactor)/2 + 1);

      for(int i=0; i<=(branchingFactor)/2; i++){
        current->keys.at(i) = fakeNode.at(i);
        current->values.push_back(fakeNodeValues.at(i));
      }

      for(int i=(branchingFactor)/2+1; i<fakeNode.size(); i++) {
        newLeaf->keys.push_back(fakeNode.at(i));
        newLeaf->values.push_back(fakeNodeValues.at(i));
      }

      //if we splitted the root, we need to create a new root
      if(current == root.get()) {
        root.release();
        Node* newRoot = new Node(false, newLeaf->keys.at(0), current, newLeaf); //not a leaf
        root.reset(newRoot);
      } else { //we didn't split the root
        //insert new key in the parent
        insertInternal(newLeaf->keys.at(0), parent, &newLeaf);
      }
    }
  }
}
//------------------------------- Insert Internal --------------------------------------//
template<class Tk, class Tv>
void BPTree<Tk,Tv>::insertInternal(Tk key, typename BPTree<Tk,Tv>::Node* parent, typename BPTree<Tk,Tv>::Node** child) {
  if((parent)->keys.size()<branchingFactor-1){ //parent is not full
    int i = std::upper_bound((parent)->keys.begin(), (parent)->keys.end(), key) - (parent)->keys.begin();
    //make space
    (parent)->keys.push_back(key);
    (parent)->children.resize((parent)->children.size()+1);
    (parent)->children.reserve((parent)->children.size()+1);
    (parent)->children.at((parent)->children.size()-1).reset((*child));

    if(i!=(parent)->keys.size()-1) {
      //shifting keys
      for(int j=(parent)->keys.size()-1; j>i; j--) {
        (parent)->keys.at(j) = (parent)->keys.at(j-1);
      }
      //shifiting pointers
      for(int j=(parent)->children.size()-1; j>(i+1); j--) {
        (parent)->children.at(j).release();
        (parent)->children.at(j).reset((parent)->children.at(j-1).release());
      }
      //insert in correct position
      (parent)->keys.at(i) = key;
      (parent)->children.at(i+1).release();
      (parent)->children.at(i+1).reset((*child));
    }
    } else { //we need to split also this node
      std::vector<Tk> fakeNode((parent)->keys);
      std::vector<Node*> fakeNodeChildren;

      for(int i=0; i<(parent)->children.size(); i++){
        fakeNodeChildren.push_back((parent)->children.at(i).get());
      }
      //find position for the key
      int i = std::upper_bound((parent)->keys.begin(), (parent)->keys.end(), key) - (parent)->keys.begin();
      fakeNode.push_back(key);
      fakeNodeChildren.push_back((*child));

      if(i!=fakeNode.size()-1){
        for(int j=fakeNode.size()-1; j>i; j--){
          fakeNode.at(j) = fakeNode.at(j-1);
        }
        for(int j=fakeNodeChildren.size()-1; j>(i+1); j--){
          fakeNodeChildren.at(j) = fakeNodeChildren.at(j-1);
        }
        //insert in right position
        fakeNode.at(i) = key;
        fakeNodeChildren.at(i+1) = (*child);
      }

      //actual splitting of the node
      int partitionIdx = fakeNode.size()/2;
      Tk partitionKey = fakeNode.at(partitionIdx);

      (parent)->keys.resize(partitionIdx);
      //(parent)->children.resize(partitionIdx+1);
      (parent)->children.reserve(partitionIdx + 1);


      for(int i=0; i<partitionIdx; i++){
        (parent)->keys.at(i) = fakeNode.at(i);
      }

      for(int i=0; i<partitionIdx + 1; i++){
        (parent)->children.at(i).release();
        (parent)->children.at(i).reset(fakeNodeChildren.at(i));
      }

      Node* newNode = new Node(false);
      for(int i=partitionIdx+1; i<fakeNode.size(); i++){
        newNode->keys.push_back(fakeNode.at(i));
      }

      if((parent)->children.size()>partitionIdx+1){
        for(int i=partitionIdx+1; i<(parent)->children.size(); i++){
          (parent)->children.at(i).release();
          (parent)->children.at(i).reset();
        }
        (parent)->children.resize(partitionIdx+1);
      }

      newNode->children.resize(fakeNodeChildren.size()-(partitionIdx+1));
      newNode->children.reserve(fakeNodeChildren.size()-(partitionIdx+1));

      for(int i=0; i<newNode->children.size(); i++){
        newNode->children.at(i).reset(fakeNodeChildren.at(i+partitionIdx+1));
      }


      if((parent) == root.get()){ //we splitted the root
        root.release();
        Node* newRoot = new Node(false, partitionKey, parent, newNode);
        root.reset(newRoot);
      } else { //we splitted an internal node
        //recursive call
        insertInternal(partitionKey, findParent(parent), &newNode);
      }
    }
}
//-------------------------------- Add Value --------------------------------------//
template<class Tk, class Tv>
void BPTree<Tk,Tv>::addValue(typename BPTree<Tk,Tv>::Node* leaf, Tk key, Tv value){
  //index corresponding to the key we are searching for
  int i = std::lower_bound(leaf->keys.begin(), leaf->keys.end(), key) - leaf->keys.begin();
  leaf->values.at(i).insert(value, method::push_back);
}
//--------------------------------- Insert ----------------------------------------//
template<class Tk, class Tv>
void BPTree<Tk,Tv>::insert(Tk key, Tv value){
  //look if the key is already present
  node<Tk,Tv>* leaf = searchLeaf(key);
  if(leaf){ //if the key is already present
    addValue(leaf, key, value);
  }
  else { //the key is not present in the tree
    insertFirst(key, value);
  }
}
//------------------------------ WRITE METHODS ------------------------------------//
template<class Tk, class Tv>
void BPTree<Tk, Tv>::writeOnFile(){
  //create the file for terms
  std::ofstream dictionary("dictionary.txt");
  //create the file for lists
  std::ofstream posting_lists("posting_lists.txt");

  if(dictionary.is_open() && posting_lists.is_open()){
    Node* firstLeaf = root.get()->leftLeaf();
    while(firstLeaf){
      for(int i=0; i<firstLeaf->keys.size(); i++){
        //write on dictionary the key
        dictionary<<firstLeaf->keys.at(i)<<"\n";
        //write on posting_lists the values
        for(auto v : firstLeaf->values.at(i)){
          posting_lists<<v<<" ";
          //posting_lists.write((char*)&v, sizeof(int));
        }
        posting_lists<<"\n";
      }
      firstLeaf = firstLeaf->next.get();
    }
  }
  else std::cout<<"unable to open file"<<std::endl;
}
//------------------------------ PRINT METHODS ------------------------------------//
//------------------------------- Print Leaves ------------------------------------//
template<class Tk, class Tv>
void BPTree<Tk,Tv>::printLeaves(){
  //pointer to the leftmost leaf starting from the root
  Node* ll = root.get()->leftLeaf();

  if(!ll){ //the tree is empty
    std::cout<<"empty B+tree"<<std::endl;
  }
  while(ll){
    for(int i=0; i<ll->keys.size(); i++){
      std::cout<<"["<<ll->keys.at(i)<<"] --> ";
      std::cout<<"< "<<ll->values.at(i)<<">"<<std::endl;
    }
    ll = ll->next.get();
  }
  std::cout<<std::endl;
}
//------------------------------- Print Levels ------------------------------------//
template<class Tk, class Tv>
void BPTree<Tk,Tv>::printLevels(){
  if(root.get()==nullptr){
    std::cout<<"Empty B+Tree"<<std::endl;
  }

  std::queue<Node*> q;
  q.emplace(root.get());

  while(!q.empty()){
    int size = q.size();
    for(int i=0; i<size; i++){
      Node* u = q.front();
      if(u==nullptr) {
        return;
      }
      q.pop();

      for(Tk val : u->keys){
        std::cout<<val<<" ";
      }
      std::cout<<"||";

      if(u->leaf!=true){
        for(auto& v: u->children){
          Node* w = v.get();
          q.emplace(w);
        }
      }
    }
    std::cout<<std::endl;
  }
}
