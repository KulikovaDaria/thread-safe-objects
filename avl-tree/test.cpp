#include "avl_tree.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <thread>


void insert(AVLtree& tree) {
  for (int i = 0; i < 100000; ++i) {
    tree.Insert(i);
  }
}

void del(AVLtree& tree) {
  for (int i = 10; i < 100000; ++i) {
    tree.Remove(i);
  }
}


void insertRand(AVLtree& tree) {
  for (int i = 0; i < 100000; ++i) {
    tree.Insert(rand());
  }
}

int main() {
  setlocale(LC_ALL, "Russian");
  std::cout << std::setprecision(5);

  AVLtree tree;
  tree.Insert(5);
  tree.Insert(10);
  tree.Insert(1);
  tree.Insert(7);
  tree.Remove(5);
  std::cout << "Дерево содержит элемент 5: " << tree.FindByKey(5) << std::endl;
  std::cout << "Дерево содержит элемент 7: " << tree.FindByKey(7) << std::endl;
  int val = 0;
  bool ex = tree.FindByRank(2, val);
  std::cout << "2-ый элемент = " << val << std::endl;
  std::cout << std::endl;

  auto start = std::chrono::steady_clock::now();
  std::thread th1(insert, std::ref(tree));
  std::thread th2(del, std::ref(tree));
  std::thread th3(insertRand, std::ref(tree));
  th1.join();
  th2.join();
  th3.join();
  auto finish = std::chrono::steady_clock::now();
  long long all = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
  long long sumwork = 0;
  for (auto t : tree.work) {
    sumwork += t.second.count();
  }
  std::cout << "Коэффициент эффективного использования: " << sumwork * 1. / all << std::endl;

  return 0;
}