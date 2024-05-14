#ifndef __shared_ptr_

#include <cstdint>
#include <map>

namespace my_shared_ptr {

template <typename T>
class shared_ptr {
 public:
  shared_ptr() = delete;

  shared_ptr<T>(T* ptr) : ptr_(ptr) {
    if (ref_table_.find(ptr_) == ref_table_.end()) {
      ref_table_[ptr] = 1;
    } else {
      ++ref_table_[ptr];
    }
  }
  shared_ptr(const shared_ptr<T>& old) : ptr_(old.ptr_) { ++ref_table_[ptr_]; }
  shared_ptr(shared_ptr<T>&& old) : ptr_(old.ptr_) { old.ptr_ = nullptr; }
  ~shared_ptr() {
    if (ptr_ && --ref_table_[ptr_] == 0) {
      delete ptr_;
    }
  }

  inline T* get(void) { return ptr_; };
  inline uint32_t GetRefCount(void) { return ref_table_[ptr_]; }

  inline T* operator->() { return get(); }
  inline T& operator*() { return *get(); }

  inline bool operator==(const T* const rhs) { return ptr_ == rhs; }
  inline bool operator==(const shared_ptr<T>& rhs) { return ptr_ == rhs.ptr_; }

 private:
  static std::map<void*, uint32_t> ref_table_;

  T* ptr_;
};

template <typename T>
std::map<void*, uint32_t> shared_ptr<T>::ref_table_;

// reference: https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
  return shared_ptr(new T(std::forward<Args>(args)...));
}

}  // namespace my_shared_ptr

#endif