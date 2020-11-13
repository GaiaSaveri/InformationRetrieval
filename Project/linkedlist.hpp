/**
  *\file linkedlist.h
  *\author Gaia Saveri
  *\brief Class implementing a linked list
  */

#include<iostream>
#include<memory> //unique_ptr

enum class method {push_back, push_front};

/**
  *\tparam T type of the value contained in each node of the linked list
  */
template<class T>
class List {
  /**
    *\brief struct for the nodes of the linked list
    */
  struct node {
    std::unique_ptr<node> next;
    T value;
    /**
      *\brief copy constructor
      *\param v value to be copied in the node
      *\param p pointer to be copied in the node
      */
    node(const T& v, node* p) : next{p}, value{v} {
    }
    /**
      *\brief move constructor
      */
    node(T&& v, node* p) : next{p}, value(std::move(v)) {

    }
    /**
      *\brief constructor
      */
    explicit node(const std::unique_ptr<node>& p) : value{p->value} {
      if(p->next) {
        next = std::make_unique<node>(p->next);
      }
    }
  };

  //head of the linked list
  std::unique_ptr<node> head;

  /**
    * \brief append a node at the end of the linked list
    */
  template<class OT>
  void push_back(OT&& v);
  /**
    *\brief append a node in front of the linked list
    */
  template<class OT>
  void push_front(OT&& v){
    head = std::make_unique<node>(std::forward<OT>(v), head.release());
  }

  //get a pointer to the last node of the linked list
  node* tail() noexcept;

public:
  //default ctor
  List() noexcept = default;

  //move semantics
  List(List&& l) noexcept = default;
  List& operator=(List&& l) noexcept = default;

  //copy semantic
  List(const List& l);
  List& operator=(const List& l);

  /**
    *\brief function to insert an element inside a linked list
    *\param v value to insert
    *\param m specifies if we need to insert back or front
    */
  template<class OT>
  void insert(OT&& v, const method m);

  template<class O>
  friend std::ostream& operator<<(std::ostream&, const List<O>&);
};

//methods implementation
template<class T>
typename List<T>::node* List<T>::tail() noexcept {
  auto tmp = tmp.get();
  while(tmp->next){
    tmp = tmp->next.get();
  }
  return tmp;
}

template<class T>
template<class OT>
void List<T>::insert(OT&& v, const method m){
  //if the list is empty
  if(!head) {
    head = std::make_unique<node>(std::forward<OT>(v), nullptr);
    return;
  }
  switch(m) {
    case method::push_back:
      push_back(std::forward<OT>(v));
      break;
    case method::push_front:
      push_front(std::forward<OT>(v));
      break;
  };
}

template<class T>
template<class OT>
void List<T>::push_back(OT&& v){
  node* last = tail();
  last->next = std::make_unique<node>(std::forward<OT>(v), nullptr);
}

template<class T>
std::ostream& operator<<(std::ostream& os, const List<T>& l){
  auto tmp = l.head.get();
  while(tmp) {
    os << tmp->value << " ";
    tmp = tmp->next.get();
  }
  return os;
}

template<class T>
List<T>::List(const List& l){
  head = std::make_unique<node>(l.head);
}
