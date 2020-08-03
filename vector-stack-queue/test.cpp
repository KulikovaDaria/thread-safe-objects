#include "threadsafe_stack.h"
#include "threadsafe_queue.h"
#include "threadsafe_vector.h"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <thread>

void pushStack(ThreadsafeStack<int>& obj, int num) {
  for (int i = 0; i < num; ++i) {
    obj.push(i);
  }
}

void popStack(ThreadsafeStack<int>& obj, int num) {
  for (int i = 0; i < num; ++i) {
    obj.pop();
  }
}

void topStack(ThreadsafeStack<int>& obj, int num) {
  std::vector<int> v(num);
  for (int i = 0; i < v.size(); ++i) {
    v[i] = obj.top();
    obj.pop();
  }
}

void testStack() {
  ThreadsafeStack<int> obj;
  int n = 1e6;
  pushStack(obj, n);
  auto start = std::chrono::steady_clock::now();
  std::thread th1(pushStack, std::ref(obj), n);
  std::thread th2(pushStack, std::ref(obj), n);
  std::thread th3(popStack, std::ref(obj), n);
  std::thread th4(pushStack, std::ref(obj), n);
  th1.join();
  th2.join();
  th3.join();
  th4.join();
  auto finish = std::chrono::steady_clock::now();
  long long all = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
  long long minwait = 0, maxwait = 0, sumwait = 0;
  for (auto t : obj.wait) {
    sumwait += t.second.count();
    minwait = std::min(minwait, t.second.count());
    maxwait = std::max(maxwait, t.second.count());
  }
  double midwait = sumwait / obj.wait.size();
  long long sumwork = 0;
  for (auto t : obj.work) {
    sumwork += t.second.count();
  }
  std::cout<< "----------СТЕК----------" <<std::endl;
  std::cout << "Минимальное время ожидания: " << minwait << " ns" << std::endl;
  std::cout << "Максимальное время ожидания: " << maxwait << " ns" << std::endl;
  std::cout << "Среднее время ожидания: " << midwait << " ns" << std::endl;
  std::cout << "Коэффициент эффективного использования: " << sumwork * 1. / all << std::endl;
  std::cout << std::endl;
}


void pushQueue(ThreadsafeQueue<int>& obj, int num) {
  for (int i = 0; i < num; ++i) {
    obj.push(i);
  }
}

void popQueue(ThreadsafeQueue<int>& obj, int num) {
  for (int i = 0; i < num; ++i) {
    obj.pop();
  }
}

void frontQueue(ThreadsafeQueue<int>& obj, int num) {
  std::vector<int> v(num);
  for (int i = 0; i < v.size(); ++i) {
    v[i] = obj.front();
    obj.pop();
  }
}

void testQueue() {
  ThreadsafeQueue<int> obj;
  int n = 1e6;
  pushQueue(obj, n);
  auto start = std::chrono::steady_clock::now();
  std::thread th1(pushQueue, std::ref(obj), 2*n);
  std::thread th2(frontQueue, std::ref(obj), n);
  std::thread th3(pushQueue, std::ref(obj), n);
  std::thread th4(popQueue, std::ref(obj), n);
  th1.join();
  th2.join();
  th3.join();
  th4.join();
  auto finish = std::chrono::steady_clock::now();
  long long all = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
  long long minwait = 0, maxwait = 0, sumwait = 0;
  for (auto t : obj.wait) {
    sumwait += t.second.count();
    minwait = std::min(minwait, t.second.count());
    maxwait = std::max(maxwait, t.second.count());
  }
  double midwait = sumwait / obj.wait.size();
  long long sumwork = 0;
  for (auto t : obj.work) {
    sumwork += t.second.count();
  }
  std::cout << "----------ОЧЕРЕДЬ----------" << std::endl;
  std::cout << "Минимальное время ожидания: " << minwait << " ns" << std::endl;
  std::cout << "Максимальное время ожидания: " << maxwait << " ns" << std::endl;
  std::cout << "Среднее время ожидания: " << midwait << " ns" << std::endl;
  std::cout << "Коэффициент эффективного использования: " << sumwork * 1. / all << std::endl;
  std::cout << std::endl;
}

void atVector(ThreadsafeVector<int>& obj, int num) {
  for (int i = 0; i < num; ++i) {
    obj.at(i, i);
  }
}

void pushVector(ThreadsafeVector<int>& obj, int num) {
  for (int i = 0; i < num; ++i) {
    obj.push_back(i);
  }
}

void popVector(ThreadsafeVector<int>& obj, int num) {
  std::vector<int> v(num);
  for (int i = 0; i < num; ++i) {
    v[i] = obj.back();
    obj.pop_back();
  }
}

void testVector() {
  ThreadsafeVector<int> obj(100);
  int n = 1e6;
  auto start = std::chrono::steady_clock::now();
  std::thread th1(&ThreadsafeVector<int>::resize, std::ref(obj), 2*n);
  std::thread th2(popVector, std::ref(obj), n);
  std::thread th3(pushVector, std::ref(obj), n);
  std::thread th4(popVector, std::ref(obj), n);
  th1.join();
  th2.join();
  th3.join();
  th4.join();
  auto finish = std::chrono::steady_clock::now();
  long long all = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
  long long minwait = 0, maxwait = 0, sumwait = 0;
  for (auto t : obj.wait) {
    sumwait += t.second.count();
    minwait = std::min(minwait, t.second.count());
    maxwait = std::max(maxwait, t.second.count());
  }
  double midwait = sumwait / obj.wait.size();
  long long sumwork = 0;
  for (auto t : obj.work) {
    sumwork += t.second.count();
  }
  std::cout << "----------ВЕКТОР----------" << std::endl;
  std::cout << "Минимальное время ожидания: " << minwait << " ns" << std::endl;
  std::cout << "Максимальное время ожидания: " << maxwait << " ns" << std::endl;
  std::cout << "Среднее время ожидания: " << midwait << " ns" << std::endl;
  std::cout << "Коэффициент эффективного использования: " << sumwork * 1. / all << std::endl;
  std::cout << std::endl;
}

int main() {
  setlocale(LC_ALL, "Russian");
  std::cout << std::setprecision(5);
  testStack();
  testQueue();
  testVector();

  return 0;
}