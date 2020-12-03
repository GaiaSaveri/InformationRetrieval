/**
  *\file linkedlist.hpp
  *\authors Alberto Sartori, Gaia Saveri
  *\brief Class implementing a linked list.
  */
#ifndef __LINKEDLIST_
#define __LINKEDLIST_

#include<iostream>
#include<memory> 
#include<iterator>
#include<algorithm>
#include<vector>

enum class method {push_back, push_front};

/**
 *\tparam T type of the value contained in each node of the linked list
 */
template<class T>
class List {

  template<class Tk, class Tv> friend class BPTree;
  /**
   *\brief Struct for the nodes of the linked list.
   */
  struct node {
    /** Unique pointer to next node in the linked list. */
    std::unique_ptr<node> next;
    /** Value contained in the current node. */
    T value;
    /**
     *\brief Copy constructor.
     *\param v Value to be copied in the node.
     *\param p Pointer to be copied in the attribute next of the current node.
     */
    node(const T& v, node* p) : next{p}, value{v} {}
    /**
     *\brief Move constructor.
     *\param v Value to be moved in the current node.
     *\param p Pointer to be moved in the attribute next of the current node.
     */
    node(T&& v, node* p) : next{p}, value(std::move(v)) {}
    /**
     *\brief Custom constructor.
     *\param p Unique pointer to a node.
     */
    explicit node(const std::unique_ptr<node>& p) : value{p->value} {
      if(p->next) {
        next.reset(p->next);
      }
    }
  };

  /** Head (first node) of the linked list. */
  std::unique_ptr<node> head;
  /**
   *\brief Function to append a node at the end of the linked list.
   *\param v Value to be inserted in the new node.
   */
  template<class OT>
  void push_back(OT&& v);
  /**
   *\brief Append a node at the beginning of the linked list.
   *\param v Value to be inserted in the new node.
   */
  template<class OT>
  void push_front(OT&& v){
    auto h = head.release();
    auto new_node = new node {v,h};
    head.reset(new_node);
    head.reset(new node{v, head.release()});
  }
  /**
   *\brief Function to get a pointer to the last node in the linked list.
   *\return node* Pointer to the last node in the linked list.
   */
  node* tail() noexcept;

public:
  /**
   *\brief Default constructor.
   */
  List() noexcept = default;
  /**
   *\brief Move contructor.
   *\param l List to be moved in the current list.
   */
  List(List&& l) noexcept = default;
  /**
   *\brief Move assignment.
   *\param l List to be moved in the current list.
   *\return List& Midified list.
   */
  List& operator=(List&& l) noexcept = default;
  /**
   *\brief Copy constructor.
   *\param l List to be copied in the current list.
   */
  List(const List& l);
  /**
   *\brief Copy assignment.
   *\param l List to copied in the current list.
   *\return List& Modified list.
   */
  List& operator=(const List& l);
  /**
   *\brief Function to insert an element inside a linked list.
   *\param v Value to insert
   *\param m Method that specifies if we need to insert at the beginning of the end of the list.
   */
  template<class OT>
  void insert(OT&& v, const method m);
  /**
   *\brief Function to intersect two sorted linked lists.
   *\param l List we want to intersect with our current list.
   *\param i List containing the intersection.
   */
  void intersection(const List& l, List& i);
  /**
   *\brief Function to perform the union of two sorted linked lists.
   *\param l List we want to merge with our current list.
   *\param u List containig the union.
   */
  void union_list(const List& l, List& u);
  /**
   *\brief Function to find the elements in the range [min, max] not contained in the current list.
   *\param c List containing the result.
   *\param min Minimum admitted value.
   *\param max Maximum admitted value.
   */
  void complement(List& c, T min, T max);
  /**
   *\brief Function to find elements that are in the current sorted list but not in the input list.
   *\param l Sorted list we want to exclude the values of.
   *\param a List containing the result.
   */
  void andnot(const List& l, List& a);
  /**
   *\brief Overload of the operator << to visualize the linked list.
   *\param os Stream to which the nodes are sent.
   *\param l List we want to print.
   *\return std::ostream& Strem to which nodes have been sent.
   */
  template<class O>
  friend std::ostream& operator<<(std::ostream&, const List<O>&);
  /**
   *\brief Function to destroy the current list, by resetting the head.
   */
  void destroyList(){
    head.reset();
  }
  /**
   *\brief Iterator to traverse the linked list.
   */
  template<typename O>
  class __iterator;

  using iterator = __iterator<T>;
  using const_iterator = __iterator<const T>;

  /**
   *\brief Function used to start iterations on the list.
   *\return iterator Iterator pointing to the head of the list.
   */
  iterator begin() noexcept { return iterator{head.get()}; }
  /**
   *\brief Function used to finish an iteration on the list.
   *\return iterator Iterator pointing to one past the last element of the list.
   */
  iterator end() { return iterator{nullptr}; }
  /**
   *\brief Function used to start iterations on the list.
   *\return const_iterator Constant iterator pointing to the head of the list.
   */
  const_iterator begin() const { return const_iterator{head.get()}; }
  /**
   *\brief Function used to finish an iteration on the list.
   *\return const_iterator Constant iterator pointing to one past the last element of the list.
   */
  const_iterator end() const { return const_iterator{nullptr}; }
  /**
   *\brief Function used to start iterations on the list.
   *\return const_iterator Constant iterator pointing to the head of the list.
   */
  const_iterator cbegin() const { return const_iterator{head.get()}; }
  /**
   *\brief Function used to finish an iteration on the list.
   *\return const_iterator Constant iterator pointing to one past the last element of the list.
   */
  const_iterator cend() const { return const_iterator{nullptr}; }
};

template<typename T>
template<typename O>
class List<T>::__iterator {
  using node = typename List<T>::node;
  /** Raw pointer to a node. */
  node* current;

public:
  /**
   *\brief Custom constructor.
   *\param x Pointer to a node.
   */
  explicit __iterator(node* x) noexcept : current{x} {}

  using value_type = O;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;
  using reference = value_type&;
  using pointer = value_type*;

  /**
   *\brief Overload of the dereference operator *.
	 *\return reference Value of the node the current iterator is pointing to.
   */
  reference operator*() const noexcept { return current->value; }
  /**
	 *\brief Overload of the arrow operator ->.
	 *\return pointer Current node iterator is pointing to.
	 */
  pointer operator->() const noexcept { return &(*(*this)); }
  /**
   *\brief Overload of the pre-increment operator ++.
   *\return __iterator& Incremented iterator.
   */
  __iterator& operator++() noexcept {
    current = current->next.get();
    return *this;
  }
  /**
   *\brief Overload of the post-increment operator ++.
   *\return iterator& Iterator before advancing to the next node.
   */
  __iterator& operator++(int) noexcept {
    __iterator tmp{current};
    ++(*this);
    return tmp;
  }
  /**
   *\brief Overload of the operator ==.
   *\param a Iterator that is going to be on the left hand side of the operator.
   *\param b Iterator that is going to be on the right hand side of the operator.
   *\return bool True if and only if the input iterators point to the same node.
   */
  friend bool operator==(const __iterator& a, const __iterator& b) {
    return a.current == b.current;
  }
  /**
   *\brief Overload of the operator !=.
   *\param a Iterator that is going to be on the left hand side of the operator.
   *\param b Iterator that is going to be on the right hand side of the operator.
   *\return bool True if and only if input iterators point to different nodes.
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

template<class T>
List<T>& List<T>::operator=(const List<T>& l){
  head.reset();
  auto tmp = l;
  (*this) = std::move(tmp);
  return *this;
}


template<class T>
void List<T>::intersection(const List& l, List& i){
  auto tmp1 = this->head.get();
  auto tmp2 = l.head.get();
  T v1, v2;
  while(tmp1 && tmp2){
    v1 = tmp1->value;
    v2 = tmp2->value;
    if(v1==v2) {
      i.insert(v1, method::push_back);
      tmp1 = tmp1->next.get();
      tmp2 = tmp2->next.get();
    } else if(v1<v2) {
      tmp1 = tmp1->next.get();
    } else if(v1>v2){
      tmp2 = tmp2->next.get();
    }
  }
}

template<class T>
void List<T>::union_list(const List& l, List& u){
  auto tmp1 = this->head.get();
  auto tmp2 = l.head.get();
  T v1, v2;
  while(tmp1 && tmp2){
    v1 = tmp1->value;
    v2 = tmp2->value;
    if(v1==v2){
      u.insert(v1, method::push_back);
      tmp1 = tmp1->next.get();
      tmp2 = tmp2->next.get();
    } else if(v1<v2) {
      u.insert(v1, method::push_back);
      tmp1 = tmp1->next.get();
    } else if(v1>v2){
      u.insert(v2, method::push_back);
      tmp2 = tmp2->next.get();
    }
  }
  while(tmp1){
    v1 = tmp1->value;
    u.insert(v1, method::push_back);
    tmp1 = tmp1->next.get();
  }
  while(tmp2){
    v2 = tmp2->value;
    u.insert(v2, method::push_back);
    tmp2 = tmp2->next.get();
  }
}

template<class T>
void List<T>::complement(List& c, T min, T max){
  auto tmp = this->head.get();
  T v;
  while(tmp){
    v = tmp->value;
    while(min<v){
      c.insert(min, method::push_back);
      min++;
    }
    min = v + 1;
    tmp = tmp->next.get();
  }
  while(min<=max){
    c.insert(min, method::push_back);
    min++;
  }
}

template<class T>
void List<T>::andnot(const List& l, List& a){
  auto tmp1 = this->head.get();
  auto tmp2 = l.head.get();
  T v1, v2;
  while(tmp1 && tmp2){
    v1 = tmp1->value;
    v2 = tmp2->value;
    if(v1==v2){
      //u.insert(v1, method::push_back);
      tmp1 = tmp1->next.get();
      tmp2 = tmp2->next.get();
    } else if(v1<v2) {
      a.insert(v1, method::push_back);
      tmp1 = tmp1->next.get();
    } else if(v1>v2){
      //u.insert(v2, method::push_back);
      tmp2 = tmp2->next.get();
    }
  }
  while(tmp1){
    v1 = tmp1->value;
    a.insert(v1, method::push_back);
    tmp1 = tmp1->next.get();
  }
}

#endif
