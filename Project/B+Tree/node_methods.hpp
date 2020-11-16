#include<vector>

template<class k, class v, class c>
node<k,v,c>** node<k,v,c>::findParent(node<k,v,c>* current, node<k,v,c>* child){
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

template<class k, class v, class c>
node<k,v,c>* node<k,v,c>::leftLeaf(){
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
