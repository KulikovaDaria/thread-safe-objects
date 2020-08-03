#include "avl_tree.h"
#include <algorithm>

void AVLtree::Insert(const int key) {
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time1 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  head = Insert(head, key);
  auto time2 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
}

void AVLtree::Remove(const int key) {
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time1 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  if (FindByKey(head, key)) {
    head = Remove(head, key);
  }
  auto time2 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
}

bool AVLtree::FindByKey(const int key) {
  std::shared_lock<std::shared_mutex> lock(mutex);
  auto time1 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  node* res = FindByKey(head, key);
  auto time2 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  return res == nullptr ? false : true;
}

bool AVLtree::FindByRank(const int rank, int& val) {
  std::shared_lock<std::shared_mutex> lock(mutex);
  auto time1 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  node* res = FindByRank(head, rank);
  if (res != nullptr) {
    val = res->key;
  }
  auto time2 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  return res == nullptr ? false : true;
}

void AVLtree::FixHeight(node* p) {
  unsigned char hleft = Height(p->left);
  unsigned char hright = Height(p->right);
  p->height = std::max(hleft, hright) + 1;
}

// Правый поворот вокруг p
AVLtree::node* AVLtree::RotateRight(node* p) {
  node* q = p->left;
  p->left = q->right;
  q->right = p;
  FixHeight(p);
  FixHeight(q);
  p->rank -= q->rank;
  return q;
}

AVLtree::node* AVLtree::RotateLeft(node* p) {
  node* q = p->right;
  p->right = q->left;
  q->left = p;
  FixHeight(p);
  FixHeight(q);
  q->rank += p->rank;
  return q;
}

// Балансировка узла p
AVLtree::node* AVLtree::Balance(node* p) {
  FixHeight(p);
  if (BFactor(p) == 2) {
    if (BFactor(p->right) < 0) {
      p->right = RotateRight(p->right);
    }
    return RotateLeft(p);
  }
  if (BFactor(p) == -2) {
    if (BFactor(p->left) > 0) {
      p->left = RotateLeft(p->left);
    }
    return RotateRight(p);
  }
  return p;
}

// Вставка ключа key в дерево с корнем p
AVLtree::node* AVLtree::Insert(node* p, int key) {
  if (!p) {
    return new node(key);
  }
  if (key < p->key) {
    (p->rank)++;
    p->left = Insert(p->left, key);
  }
  else {
    p->right = Insert(p->right, key);
  }
  return Balance(p);
}

// Поиск узла с минимальным ключом в дереве p
AVLtree::node* AVLtree::FindMin(node* p) {
  return p->left ? FindMin(p->left) : p;
}

// Удалеие узла с минимальным ключом из дерева p
AVLtree::node* AVLtree::RemoveMin(node* p) {
  if (p->left == nullptr) {
    return p->right;
  }
  (p->rank)--;
  p->left = RemoveMin(p->left);
  return Balance(p);
}

// Удаление ключа key из дерева p
AVLtree::node* AVLtree::Remove(node* p, const int key) {
  if (!p) {
    return nullptr;
  }
  if (key < p->key) {
    (p->rank)--;
    p->left = Remove(p->left, key);
  }
  else {
    if (key > p->key) {
      p->right = Remove(p->right, key);
    }
    else { // key == p->key
      node* left = p->left;
      node* right = p->right;
      int rank = p->rank;
      delete p;
      if (!right) {
        return left;
      }
      node* min_node = FindMin(right);
      min_node->right = RemoveMin(right);
      min_node->left = left;
      min_node->rank = rank;
      return Balance(min_node);
    }
  }
  return Balance(p);
}

// Поиск k-го элемента в дереве p
AVLtree::node* AVLtree::FindByRank(node* p, const int k) {
  if (!p) {
    return nullptr;
  }
  if (k == p->rank) {
    return p;
  }
  if (k < p->rank) {
    return FindByRank(p->left, k);
  }
  return FindByRank(p->right, k - p->rank);
}

// Поиск ключа key в дереве p
AVLtree::node* AVLtree::FindByKey(node* p, const int key) {
  if (!p || p->key == key) {
    return p;
  }
  if (key < p->key) {
    return FindByKey(p->left, key);
  }
  return FindByRank(p->right, key);
}
