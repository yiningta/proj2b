// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef BINARYPQ_H
#define BINARYPQ_H

#include <algorithm>
#include <cstddef>

#include "Eecs281PQ.hpp"

// A specialized version of the priority queue ADT implemented as a binary heap.
template <typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
  // This is a way to refer to the base class object.
  using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
  // Description: Construct an empty PQ with an optional comparison functor.
  // Runtime: O(1)
  explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) : BaseClass{comp} {
    // TODO: Implement this function, or verify that it is already done
  } // BinaryPQ

  // Description: Construct a PQ out of an iterator range with an optional
  //              comparison functor.
  // Runtime: O(n) where n is number of elements in range.
  template <typename InputIterator>
  BinaryPQ(InputIterator start, InputIterator end,
           COMP_FUNCTOR comp = COMP_FUNCTOR())
      : BaseClass{comp}, data{start, end} {
    // TODO: Implement this function
    updatePriorities();
  } // BinaryPQ

  // Description: Destructor doesn't need any code, the data vector will
  //              be destroyed automatically.
  virtual ~BinaryPQ() = default;

  // Description: Copy constructors don't need any code, the data vector
  //              will be copied automatically.
  BinaryPQ(const BinaryPQ &) = default;
  BinaryPQ(BinaryPQ &&) noexcept = default;

  // Description: Copy assignment operators don't need any code, the data
  //              vector will be copied automatically.
  BinaryPQ &operator=(const BinaryPQ &) = default;
  BinaryPQ &operator=(BinaryPQ &&) noexcept = default;

  // Description: Assumes that all elements inside the heap are out of order and
  //              'rebuilds' the heap by fixing the heap invariant.
  // Runtime: O(n)
  virtual void updatePriorities() {
    // TODO: Implement this function.
    if (data.empty())
      return;
    for (size_t i = (size_t)data.size() / 2; i-- > 0;) {
      fixDown(i);
    }
  } // updatePriorities()

  // Description: Add a new element to the PQ.
  // Runtime: O(log(n))
  virtual void push(const TYPE &val) {
    // TODO: Implement this function.
    data.push_back(val);
    fixUp(data.size() - 1);
  } // push()

  // Description: Remove the most extreme (defined by 'compare') element
  //              from the PQ.
  // Note: We will not run tests on your code that would require it to pop
  // an element when the PQ is empty. Though you are welcome to if you are
  // familiar with them, you do not need to use exceptions in this project.
  // Runtime: O(log(n))
  virtual void pop() {
    // TODO: Implement this function.
    std::swap(data[0], data[data.size() - 1]);
    data.pop_back();
    fixDown(0);
  } // pop()

  // Description: Return the most extreme (defined by 'compare') element of
  //              the PQ. This should be a reference for speed. It MUST
  //              be const because we cannot allow it to be modified, as
  //              that might make it no longer be the most extreme element.
  // Runtime: O(1)
  virtual const TYPE &top() const {
    // TODO: Implement this function.
    return data.front();
    // These lines are present only so that this provided file compiles.

  } // top()

  // Description: Get the number of elements in the PQ.
  // Runtime: O(1)
  [[nodiscard]] virtual std::size_t size() const {
    // TODO: Implement this function. Might be very simple,
    // depending on your implementation.
    return data.size(); // TODO: Delete or change this line
  }                     // size()

  // Description: Return true if the PQ is empty.
  // Runtime: O(1)
  [[nodiscard]] virtual bool empty() const {
    // TODO: Implement this function. Might be very simple,
    // depending on your implementation.
    return data.empty(); // TODO: Delete or change this line
  }                      // empty()

private:
  // Note: This vector *must* be used for your PQ implementation.
  std::vector<TYPE> data;
  // NOTE: You are not allowed to add any member variables. You don't need
  //       a "heapSize", since you can call your own size() member
  //       function, or check data.size().

  // TODO: Add any additional member functions you require here.
  //       For instance, you might add fixUp() and fixDown().
  void fixDown(size_t k) {
    size_t current = k;
    while (true) {
      size_t leftChild = 2 * current + 1;
      size_t rightChild = 2 * current + 2;

      if (leftChild >= data.size()) {
        break;
      }
      if (rightChild >= data.size()) {
        if (this->compare(data[current], data[leftChild])) {
          std::swap(data[current], data[leftChild]);
        }
        break;
      }
      if (this->compare(data[leftChild], data[rightChild])) {
        if (this->compare(data[current], data[rightChild])) {
          std::swap(data[current], data[rightChild]);
          current = rightChild;
        } else {
          break;
        }
      } else {
        if (this->compare(data[current], data[leftChild])) {
          std::swap(data[current], data[leftChild]);
          current = leftChild;
        } else {
          break;
        }
      }
    }
  }

  void fixUp(size_t k) {
    size_t current = k;
    while (current > 0) {
      size_t parent = (current - 1) / 2;
      if (this->compare(data[current], data[parent])) {
        break;
      } else {
        std::swap(data[current], data[parent]);
        current = parent;
      }
    }
  }
}; // BinaryPQ

#endif // BINARYPQ_H
