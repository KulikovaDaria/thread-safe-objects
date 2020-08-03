#ifndef AVL_TREE
#define AVL_TREE
#include <shared_mutex>
#include <chrono>
#include <map>

class AVLtree {
public:

  void Insert(const int key);
  void Remove(const int key);
  bool FindByKey(const int key);
  bool FindByRank(const int rank, int& val);

  std::map<std::thread::id, std::chrono::nanoseconds> work;

private:
  struct node {
    int key;
    int rank;
    unsigned char height;
    node* left;
    node* right;
    node(int k) 
      :key(k), rank(1), left(nullptr), right(nullptr), height(1) {}
  };

  static unsigned char Height(node* p) { return p ? p->height : 0; }
  static int BFactor(node* p) { return Height(p->right) - Height(p->left); }
  static int Rank(node* p) { return p ? p->rank : 0; }
  static void FixHeight(node* p);

  // Правый поворот вокруг p
  static node* RotateRight(node* p);
  // Левый поворот вокруг p
  static node* RotateLeft(node* p);
  // Балансировка узла p
  static node* Balance(node* p);

  // Вставка ключа key в дерево с корнем p
  static node* Insert(node* p, const int key);
  
  // Поиск узла с минимальным ключом в дереве p
  static node* FindMin(node* p);
  // Удалеие узла с минимальным ключом из дерева p
  static node* RemoveMin(node* p);
  // Удаление ключа key из дерева p
  static node* Remove(node* p, const int key);
  
  // Поиск ключа key в дереве p
  static node* FindByKey(node* p, const int key);
  // Поиск k-го элемента в дереве p
  static node* FindByRank(node* p, const int k);

  node* head;
  mutable std::shared_mutex mutex;
};

#endif // !AVL_TREE
