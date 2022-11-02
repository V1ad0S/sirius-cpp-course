#pragma once

enum class Color {
  RED,
  BLACK,
};

struct NodeData {
  int value;
  int key;

  bool operator<(const NodeData& other) const;
};

class RBNode {
 private:
  Color col;
  NodeData data;
  RBNode* left;
  RBNode* right;
  RBNode* parent;

 public:
  RBNode(RBNode* _parent, NodeData data, Color _col = Color::BLACK);
  bool is_leaf() const;
  Color color() const;
  void change_color();  // need???
  int height() const;
  int black_height() const;

  friend class RBTree;
};

class RBTree {
 private:
  RBNode* head;
  void rotate_right(RBNode* target_vertex);
  void rotate_left(RBNode* target_vertex);
  void insert_fix(RBNode* target_vertex);
  void erase_fix(RBNode* target_vertex);

 public:
  void insert(NodeData data);
  void erase(NodeData data);
};
