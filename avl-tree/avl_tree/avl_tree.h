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

  // ������ ������� ������ p
  static node* RotateRight(node* p);
  // ����� ������� ������ p
  static node* RotateLeft(node* p);
  // ������������ ���� p
  static node* Balance(node* p);

  // ������� ����� key � ������ � ������ p
  static node* Insert(node* p, const int key);
  
  // ����� ���� � ����������� ������ � ������ p
  static node* FindMin(node* p);
  // ������� ���� � ����������� ������ �� ������ p
  static node* RemoveMin(node* p);
  // �������� ����� key �� ������ p
  static node* Remove(node* p, const int key);
  
  // ����� ����� key � ������ p
  static node* FindByKey(node* p, const int key);
  // ����� k-�� �������� � ������ p
  static node* FindByRank(node* p, const int k);

  node* head;
  mutable std::shared_mutex mutex;
};

#endif // !AVL_TREE
