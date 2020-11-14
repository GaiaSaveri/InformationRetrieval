/**
  *\file linkedlist.h
  *\author Gaia Saveri
  *\brief Class implementing a linked list
  */
#ifndef __LINKEDLIST_
#define __LINKEDLIST_

#include<iostream>
#include<memory> //unique_ptr
#include<iterator>
#include<algorithm>
#include<vector>

enum class method {push_back, push_front};

/**
  *\tparam T type of the value contained in each node of the linked list
  */
template<class T>
class List {

  template<class Tk, class Tv, class Tc> friend class BPTree;
  /**
    *\brief struct for the nodes of the linked list
    */
  struct node {
    /** unique pointer to next node in the linked list */
    std::unique_ptr<node> next;
    /** value contained in the current node */
    T value;
    /**
      *\brief copy constructor
      *\param v value to be copied in the node
      *\param p pointer to be copied in the node
      */
    node(const T& v, node* p) : next{p}, value{v} {}
    /**
      *\brief move constructor
      */
    node(T&& v, node* p) : next{p}, value(std::move(v)) {}
    /**
      *\brief constructor
      */
    explicit node(const std::unique_ptr<node>& p) : value{p->value} {
      if(p->next) {
        next.reset(p->next);
      }
    }
  };

  /** head of the linked list */
  std::unique_ptr<node> head;
  /**
    * \brief append a node at the end of the linked list.
    */
  template<class OT>
  void push_back(OT&& v);
  /**
    *\brief append a node in front of the linked list.
    */
  template<class OT>
  void push_front(OT&& v){
    auto h = head.release();
    auto new_node = new node {v,h};
    head.reset(new_node);
    head.reset(new node{v, head.release()});
  }

  /**
   *\brief get a pointer to the last node in the linked list.
   */
  node* tail() noexcept;

public:
  /** default constructor */
  List() noexcept = default;

  /** move contructor */
  List(List&& l) noexcept = default;
  /** move operator */
  List& operator=(List&& l) noexcept = default;

  /** copy constructor */
  List(const List& l);
  /** copy operator */
  List& operator=(const List& l);

  /**
    *\brief function to insert an element inside a linked list
    *\param v value to insert
    *\param m specifies if we need to insert back or front
    */
  template<class OT>
  void insert(OT&& v, const method m);

  /**
   *\brief overload of the operator << to visualize the linked list
   */
  template<class O>
  friend std::ostream& operator<<(std::ostream&, const List<O>&);

  /**
   *\brief iterator to traverse the linked list.
   */
  template<typename O>
  class __iterator;

  using iterator = __iterator<T>;
  using const_iterator = __iterator<const T>;

  iterator begin() noexcept { return iterator{head.get()}; }
  iterator end() { return iterator{nullptr}; }

  const_iterator begin() const { return const_iterator{head.get()}; }
  const_iterator end() const { return const_iterator{nullptr}; }

  const_iterator cbegin() const { return const_iterator{head.get()}; }
  const_iterator cend() const { return const_iterator{nullptr}; }
};

template<typename T>
template<typename O>
class List<T>::__iterator {
  using node = typename List<T>::node;
  node* current;

public:
  /** custom constructor */
  explicit __iterator(node* x) noexcept : current{x} {}

  using value_type = O;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;
  using reference = value_type&;
  using pointer = value_type*;

  reference operator*() const noexcept { return current->value; }
  pointer operator->() const noexcept { return &(*(*this)); }

  /**
   *\brief overload of the pre-increment operator.
   */
  __iterator& operator++() noexcept {
    current = current->next.get();
    return *this;
  }

  /**
   *\brief overload of the post-increment operator.
   */
  __iterator& operator++(int) noexcept {
    __iterator tmp{current};
    ++(*this);
    return tmp;
  }

  /**
   *\brief overload of the operator ==.
   */
  friend bool operator==(const __iterator& a, const __iterator& b) {
    return a.current == b.current;
  }
  /**
   *\brief overload of the operator !=.
   */
  friend bool operator!=(const __iterator& a, const __iterator& b) {
    return !(a == b);
 }
};

//linked-list methods implementation

template<class T>
typename List<T>::node* List<T>::tail() noexcept {
  auto tmp = head.get();
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
    head.reset(new node{v,nullptr});
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
  last->next.reset(new node{v, nullptr});
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
  auto tmp = l.head.get();
  while(tmp){
    insert(tmp->value,method::push_back);
    tmp=tmp->next.get();
  }
}

#endif