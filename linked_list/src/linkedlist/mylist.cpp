#include "mylist.h"

#include <iostream>

void MyList::add_node(T value) {
  if (head == nullptr) {
    head = new Node(value);
    return;
  }
  Node *elem = head;
  while (elem->pnext != nullptr) {
    elem = elem->pnext;
  }
  elem->pnext = new Node(value);
}

void MyList::print_list() const {
  if (head == nullptr) {
    std::cout << "List is empty" << std::endl;
    return;
  }
  Node *elem = head;
  while (elem != nullptr) {
    std::cout << elem->data << ' ';
    elem = elem->pnext;
  }
  std::cout << std::endl;
}

void MyList::delete_element(T value) {
  while (head != nullptr && head->data == value) {
    Node *tmp = head->pnext;
    delete head;
    head = tmp;
  }
  if (head == nullptr) {
    return;
  }
  Node *elem = head->pnext;
  Node *elem_prev = head;
  while (elem != nullptr) {
    if (elem->data == value) {
      Node *tmp = elem->pnext;
      delete elem;
      elem_prev->pnext = tmp;
      elem = tmp;
    } else {
      elem_prev = elem_prev->pnext;
      elem = elem->pnext;
    }
  }
}
