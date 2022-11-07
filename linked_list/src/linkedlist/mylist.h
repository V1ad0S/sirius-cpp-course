#pragma once

typedef int T;

class Node {
 private:
  T data;
  Node* pnext = nullptr;

 public:
  Node(T _data) : data(_data){};
  friend class MyList;
};

class MyList {
 private:
  Node* head = nullptr;

 public:
  ~MyList();

  void add_node(T value);
  void print_list() const;
  void delete_element(T value);
};
