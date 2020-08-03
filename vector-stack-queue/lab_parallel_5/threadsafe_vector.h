#pragma once
#ifndef THREADSAFE_VECTOR_H
#define THREADSAFE_VECTOR_H

#include <shared_mutex>
#include <vector>


template<typename T>
class ThreadsafeVector {
public:
  ThreadsafeVector() = default;
  ThreadsafeVector(ptrdiff_t size);
  ThreadsafeVector(const ThreadsafeVector& obj);
  ~ThreadsafeVector() = default;
  ThreadsafeVector operator=(const ThreadsafeVector& obj);
  bool operator==(const ThreadsafeVector& obj);
  bool operator!=(const ThreadsafeVector& obj);
  T at(ptrdiff_t pos);
  void at(ptrdiff_t pos, const T& val);
  T operator[](ptrdiff_t pos);
  T front();
  void front(const T& val);
  T back();
  void back(const T& val);
  bool empty();
  ptrdiff_t size();
  ptrdiff_t max_size();
  void reserve(ptrdiff_t size);
  ptrdiff_t capacity();
  void shrink_to_fit();
  void clear();
  void push_back(const T& val);
  void pop_back();
  void resize(ptrdiff_t size);
  void swap(const ThreadsafeVector& obj);

  std::map<std::thread::id, std::chrono::nanoseconds> wait;
  std::map<std::thread::id, std::chrono::nanoseconds> work;

private:
  std::vector<T> data;
  mutable std::shared_mutex mutex;
};

template<typename T>
ThreadsafeVector<T>::ThreadsafeVector(ptrdiff_t size) {
  std::lock_guard<std::shared_mutex> lock(mutex);
  data.resize(size);
}

template<typename T>
ThreadsafeVector<T>::ThreadsafeVector(const ThreadsafeVector<T>& obj) {
  std::lock_guard<std::shared_mutex> lock(mutex);
  data = obj.data;
}

template<typename T>
ThreadsafeVector<T> ThreadsafeVector<T>::operator=(const ThreadsafeVector<T>& obj) {
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
bool ThreadsafeVector<T>::operator==(const ThreadsafeVector<T>& obj) {
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
bool ThreadsafeVector<T>::operator!=(const ThreadsafeVector<T>& obj) {
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
T ThreadsafeVector<T>::at(ptrdiff_t pos) {
  auto time1 = std::chrono::steady_clock::now();
  std::shared_lock<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  T res = data.at(pos);
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
  return res;
}

template<typename T>
void ThreadsafeVector<T>::at(ptrdiff_t pos, const T& val) {
  auto time1 = std::chrono::steady_clock::now();
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  data.at(pos) = val;
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
}

template<typename T>
T ThreadsafeVector<T>::operator[](ptrdiff_t pos) {
  auto time1 = std::chrono::steady_clock::now();
  std::shared_lock<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  T res = data[pos];
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
  return res;
}

template<typename T>
T ThreadsafeVector<T>::front() {
  auto time1 = std::chrono::steady_clock::now();
  std::shared_lock<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  T res = data.front();
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
  return res;
}

template<typename T>
void ThreadsafeVector<T>::front(const T& val) {
  auto time1 = std::chrono::steady_clock::now();
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  data.front() = val;
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
}

template<typename T>
T ThreadsafeVector<T>::back() {
  auto time1 = std::chrono::steady_clock::now();
  std::shared_lock<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  T res = data.back();
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
  return res;
}

template<typename T>
void ThreadsafeVector<T>::back(const T& val) {
  auto time1 = std::chrono::steady_clock::now();
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  data.back() = val;
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
}

template<typename T>
bool ThreadsafeVector<T>::empty() {
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
ptrdiff_t ThreadsafeVector<T>::size() {
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
ptrdiff_t ThreadsafeVector<T>::max_size() {
  auto time1 = std::chrono::steady_clock::now();
  std::shared_lock<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  ptrdiff_t res = data.max_size();
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
  return res;

}

template<typename T>
void ThreadsafeVector<T>::reserve(ptrdiff_t size) {
  auto time1 = std::chrono::steady_clock::now();
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  data.reserve(size);
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
}

template<typename T>
ptrdiff_t ThreadsafeVector<T>::capacity() {
  auto time1 = std::chrono::steady_clock::now();
  std::shared_lock<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  ptrdiff_t res = data.capacity();
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
  return res;
}

template<typename T>
void ThreadsafeVector<T>::shrink_to_fit() {
  auto time1 = std::chrono::steady_clock::now();
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  data.srink_to_fit();
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
}

template<typename T>
void ThreadsafeVector<T>::clear() {
  auto time1 = std::chrono::steady_clock::now();
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  data.clear();
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
}

template<typename T>
void ThreadsafeVector<T>::push_back(const T& val) {
  auto time1 = std::chrono::steady_clock::now();
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  data.push_back(val);
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
}

template<typename T>
void ThreadsafeVector<T>::pop_back() {
  auto time1 = std::chrono::steady_clock::now();
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  data.pop_back();
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
}

template<typename T>
void ThreadsafeVector<T>::resize(ptrdiff_t size) {
  auto time1 = std::chrono::steady_clock::now();
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  data.resize(size);
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
}

template<typename T>
void ThreadsafeVector<T>::swap(const ThreadsafeVector& obj) {
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