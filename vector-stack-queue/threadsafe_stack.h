#ifndef THREADSAFE_STACK_H
#define THREADSAFE_STACK_H

#include <shared_mutex>
#include <stack>
#include <map>
#include <chrono>

template<typename T>
class ThreadsafeStack {
public:
  ThreadsafeStack() = default;
  ThreadsafeStack(const ThreadsafeStack& obj);
  ~ThreadsafeStack() = default;
  ThreadsafeStack operator=(const ThreadsafeStack& obj);
  bool operator==(const ThreadsafeStack& obj);
  bool operator!=(const ThreadsafeStack& obj);
  T top();
  void top(const T& val);
  bool empty();
  ptrdiff_t size();
  void push(const T& val);
  void pop();
  void swap(const ThreadsafeStack& obj);

  std::map<std::thread::id, std::chrono::nanoseconds> wait;
  std::map<std::thread::id, std::chrono::nanoseconds> work;

private:
  std::stack<T> data;
  mutable std::shared_mutex mutex;
};

template<typename T>
ThreadsafeStack<T>::ThreadsafeStack(const ThreadsafeStack<T>& obj) {
  std::lock_guard<std::shared_mutex> lock(mutex);
  data = obj.data;
}

template<typename T>
ThreadsafeStack<T> ThreadsafeStack<T>::operator=(const ThreadsafeStack<T>& obj) {
  auto time1 = std::chrono::steady_clock::now();
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  data = obj.data;
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
  return *this;
}

template<typename T>
bool ThreadsafeStack<T>::operator==(const ThreadsafeStack<T>& obj) {
  auto time1 = std::chrono::steady_clock::now();
  std::shared_lock<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  bool res = (data == obj.data);
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
  return res;
}

template<typename T>
bool ThreadsafeStack<T>::operator!=(const ThreadsafeStack& obj) {
  auto time1 = std::chrono::steady_clock::now();
  std::shared_lock<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  bool res = (data != obj.data);
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
  return res;
}

template<typename T>
T ThreadsafeStack<T>::top() {
  auto time1 = std::chrono::steady_clock::now();
  std::shared_lock<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  T res = data.top();
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
  return res;
}

template<typename T>
void ThreadsafeStack<T>::top(const T& val) {
  auto time1 = std::chrono::steady_clock::now();
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  data.top() = val;
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
}

template<typename T>
bool ThreadsafeStack<T>::empty() {
  auto time1 = std::chrono::steady_clock::now();
  std::shared_lock<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  bool res = data.empty();
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
  return res;
}

template<typename T>
ptrdiff_t ThreadsafeStack<T>::size() {
  auto time1 = std::chrono::steady_clock::now();
  std::shared_lock<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  ptrdiff_t res = data.size();
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
  return res;
}

template<typename T>
void ThreadsafeStack<T>::push(const T& val) {
  auto time1 = std::chrono::steady_clock::now();
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  data.push(val);
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
}

template<typename T>
void ThreadsafeStack<T>::pop() {
  auto time1 = std::chrono::steady_clock::now();
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  if (!data.empty()) {
    data.pop();
  }
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
}

template<typename T>
void ThreadsafeStack<T>::swap(const ThreadsafeStack& obj) {
  auto time1 = std::chrono::steady_clock::now();
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  data.swap(obj.data);
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
}

#endif