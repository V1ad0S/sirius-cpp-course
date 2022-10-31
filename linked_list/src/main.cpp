#include <fstream>
#include <iostream>

#include "exceptions.h"
#include "linkedlist/mylist.h"

void process_file(const char *filename, MyList &list) {
  std::ifstream file(filename);
  if (!file) {
    throw ReadingError::CantOpenFile(filename);
  }

  int x, line_num = 1;
  while (!file.fail()) {
    file >> x;

    list.add_node(x);
    line_num++;
  }
  if (file.eof()) {
    list.delete_element(x);
    return;
  }
  throw ReadingError::BadData(line_num);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Wrong arguments" << std::endl;
    return -1;
  }
  MyList l;
  process_file(argv[1], l);
  l.print_list();
  return 0;
}
