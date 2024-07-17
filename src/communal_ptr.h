#pragma once

#include <vector>

namespace paige {

// TODO: Define type `CommunalPtr`, which takes a single template type argument
// and has the same interface as `std::shared_ptr`.

class CommunalPtr {
 private:
  // std::vector<typename T> references;
  std::vector<CommunalPtr*>* references;
  int* value;

 public:
  // default constructor
  explicit CommunalPtr() {
    references->push_back(this);
  }

  // non default constructor
  explicit CommunalPtr(int val) {
    int* value = new int;
    *value = val;
    auto* references = new std::vector<CommunalPtr*>;
    references->push_back(this);
  }

  // copy constructor
  CommunalPtr(const CommunalPtr& ptr) : value(ptr.value) {
    references->push_back(this);
  }

  void swap(CommunalPtr& ptr1) {
    std::swap(ptr1.references, this->references);
    std::swap(ptr1.value, this->value);
  }

  // overload assignment opperator
  CommunalPtr& operator=(const CommunalPtr& ptr) {
    CommunalPtr(ptr).swap(*this);
    return *this;
  }

  void get() {}

  bool empty() {
    return references->size() == 0;
  }

  // destructor
  ~CommunalPtr() {
    if (empty()) {
      delete value;
      delete references;
    }
  }
};

}  // namespace paige
