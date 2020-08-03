#pragma once
#ifndef THREADSAFE_QUEUE_H
#define THREADSAFE_QUEUE_H

#include <shared_mutex>
#include <queue>


template<typename T>
class ThreadsafeQueue {
public:
  ThreadsafeQueue() = default;
  ThreadsafeQueue(const ThreadsafeQueue& obj);
  ~ThreadsafeQueue() = default;
  ThreadsafeQueue operator=(const ThreadsafeQueue& obj);
  bool operator==(const ThreadsafeQueue& obj);
  bool operator!=(const ThreadsafeQueue& obj);
  T front();
  void front(const T& val);
  T back();
  void back(const T& val);
  bool empty();
  ptrdiff_t size();
  void push(const T& val);
  void pop();
  void swap(const ThreadsafeQueue& obj);

  std::map<std::thread::id, std::chrono::nanoseconds> wait;
  std::map<std::thread::id, std::chrono::nanoseconds> work;

private:
  std::queue<T> data;
  mutable std::shared_mutex mutex;
};

template<typename T>
ThreadsafeQueue<T>::ThreadsafeQueue(const ThreadsafeQueue<T>& obj) {
  std::lock_guard<std::shared_mutex> lock(mutex);
  data = obj.data;
}

template<typename T>
ThreadsafeQueue<T> ThreadsafeQueue<T>::operator=(const ThreadsafeQueue<T>& obj) {
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
bool ThreadsafeQueue<T>::operator==(const ThreadsafeQueue<T>& obj) {
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
bool ThreadsafeQueue<T>::operator!=(const ThreadsafeQueue& obj) {
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
T ThreadsafeQueue<T>::front() {
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
void ThreadsafeQueue<T>::front(const T& val) {
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
T ThreadsafeQueue<T>::back() {
  auto time1 = std::chrono::steady_clock::now();
  std::shared_lock<std::mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  T res = data.back();
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
  return res;
}

template<typename T>
inline void ThreadsafeQueue<T>::back(const T& val) {
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
bool ThreadsafeQueue<T>::empty() {
  auto time1 = std::chrono::steady_clock::now();
  std::shared_lock<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  bool res = data.empty();
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
}

template<typename T>
ptrdiff_t ThreadsafeQueue<T>::size() {
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
void ThreadsafeQueue<T>::push(const T& val) {
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
void ThreadsafeQueue<T>::pop() {
  auto time1 = std::chrono::steady_clock::now();
  std::lock_guard<std::shared_mutex> lock(mutex);
  auto time2 = std::chrono::steady_clock::now();
  auto th_id = std::this_thread::get_id();
  wait[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time2 - time1);
  data.pop();
  auto time3 = std::chrono::steady_clock::now();
  work[th_id] += std::chrono::duration_cast<std::chrono::nanoseconds>(time3 - time2);
}

template<typename T>
inline void ThreadsafeQueue<T>::swap(const ThreadsafeQueue& obj) {
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