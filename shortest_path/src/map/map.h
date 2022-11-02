#pragma once

typedef int Type;

class Map {
 private:
  Type* data = nullptr;
  int n = 0;
  int m = 0;

 public:
  ~Map();

  const Type& operator()(int i, int j) const;

  void read_file(const char* filename, int N, int M);
};
