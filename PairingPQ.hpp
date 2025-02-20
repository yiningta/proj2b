// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include <deque>
#include <utility>

#include "Eecs281PQ.hpp"

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template <typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
  // This is a way to refer to the base class object.
  using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
  // Each node within the pairing heap
  class Node {
  public:
    // Description: Custom constructor that creates a node containing
    //              the given value.
    explicit Node(const TYPE &val)
        : elt{val}, child(nullptr), sibling(nullptr), parent(nullptr) {}

    // Description: Allows access to the element at that Node's position.
    //              There are two versions, getElt() and a dereference
    //              operator, use whichever one seems more natural to you.
    // Runtime: O(1) - this has been provided for you.
    const TYPE &getElt() const { return elt; }
    const TYPE &operator*() const { return elt; }

    // The following line allows you to access any private data
    // members of this Node class from within the PairingPQ class.
    // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
    // function).
    friend PairingPQ;

  private:
    TYPE elt;
    Node *child;
    Node *sibling;
    Node *parent;
    // TODO: Add and initialize one extra pointer (parent or previous) as
    // desired.
  }; // Node

  // Description: Construct an empty pairing heap with an optional
  //              comparison functor.
  // Runtime: O(1)
  explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) : BaseClass{comp} {
    // TODO: Implement this function.
    this->root = nullptr;
    this->nodeCount = 0;
  } // PairingPQ()

  // Description: Construct a pairing heap out of an iterator range with an
  //              optional comparison functor.
  // Runtime: O(n) where n is number of elements in range.
  template <typename InputIterator>
  PairingPQ(InputIterator start, InputIterator end,
            COMP_FUNCTOR comp = COMP_FUNCTOR())
      : BaseClass{comp} {
    // TODO: Implement this function.
    this->root = nullptr;
    this->nodeCount = 0;
    for (InputIterator it = start; it != end; it++) {
      PairingPQ::push(*it);
    }
  } // PairingPQ()

  // Description: Copy constructor.
  // Runtime: O(n)
  PairingPQ(const PairingPQ &other) : BaseClass{other.compare} {
    // TODO: Implement this function.
    // NOTE: The structure does not have to be identical to the original,
    //       but it must still be a valid pairing heap.
    this->root = nullptr;
    this->nodeCount = 0;
    if (other.empty()) {
      return;
    }
    std::deque<Node *> deq;
    deq.push_back(other.root);
    while (!deq.empty()) {
      const Node *current = deq.front();
      deq.pop_front();
      if (current->sibling != nullptr) {
        deq.push_back(current->sibling);
      }
      if (current->child != nullptr) {
        deq.push_back(current->child);
      }
      this->push(current->elt);
    }
  } // PairingPQ()

  // Description: Copy assignment operator.
  // Runtime: O(n)
  PairingPQ &operator=(const PairingPQ &rhs) {
    // TODO: Implement this function.
    // HINT: Use the copy-swap method from the "Arrays and Containers"
    // lecture.
    PairingPQ temp(rhs);
    std::swap(nodeCount, temp.nodeCount);
    std::swap(root, temp.root);
    return *this;
  } // operator=()

  // Description: Destructor
  // Runtime: O(n)
  ~PairingPQ() {
    // TODO: Implement this function.
    if (root == nullptr) {
      return;
    }
    std::deque<Node *> deq;
    deq.push_back(root);
    while (!deq.empty()) {
      Node *current = deq.front();
      deq.pop_front();
      if (current->sibling != nullptr) {
        deq.push_back(current->sibling);
      }
      if (current->child != nullptr) {
        deq.push_back(current->child);
      }
      delete current;
    }
  } // ~PairingPQ()

  // Description: Move constructor and assignment operators don't need any
  //              code, the members will be reused automatically.
  PairingPQ(PairingPQ &&) noexcept = default;
  PairingPQ &operator=(PairingPQ &&) noexcept = default;

  // Description: Assumes that all elements inside the pairing heap are out
  //              of order and 'rebuilds' the pairing heap by fixing the
  //              pairing heap invariant.  You CANNOT delete 'old' nodes
  //              and create new ones!
  // Runtime: O(n)
  virtual void updatePriorities() {
    // TODO: Implement this function.
    if (empty()) {
      return;
    }
    std::deque<Node *> deq;
    deq.push_back(root->child);
    while (!deq.empty()) {
      Node *current = deq.front();
      deq.pop_front();
      if (current->sibling != nullptr) {
        deq.push_back(current->sibling);
      }
      if (current->child != nullptr) {
        deq.push_back(current->child);
      }
      current->parent = nullptr;
      current->sibling = nullptr;
      current->child = nullptr;
      root = meld(root, current);
    }
  } // updatePriorities()

  // Description: Add a new element to the pairing heap. This is already
  //              done. You should implement push functionality entirely
  //              in the addNode() function, and this function calls
  //              addNode().
  // Runtime: O(1)
  virtual void push(const TYPE &val) { addNode(val); } // push()

  // Description: Remove the most extreme (defined by 'compare') element
  //              from the pairing heap.
  // Note: We will not run tests on your code that would require it to pop
  // an element when the pairing heap is empty. Though you are welcome to
  // if you are familiar with them, you do not need to use exceptions in
  // this project.
  // Runtime: Amortized O(log(n))
  virtual void pop() {
    // TODO: Implement this function.
    if (nodeCount == 1) {
      root = nullptr;
      nodeCount = 0;
      delete root;
      return;
    }
    std::deque<Node *> temp;
    Node *current = root->child;
    delete root;
    while (current != nullptr) {
      temp.push_back(current);
      if (current->sibling != nullptr) {
        current = current->sibling;
      } else {
        break;
      }
    }
    while (temp.size() > 1) {
      Node *first = temp.front();
      temp.pop_front();
      Node *second = temp.front();
      temp.pop_front();
      first->parent = nullptr;
      first->sibling = nullptr;
      second->parent = nullptr;
      second->sibling = nullptr;
      temp.push_back(meld(first, second));
    }
    this->root = temp.front();
    this->nodeCount--;
  } // pop()

  // Description: Return the most extreme (defined by 'compare') element of
  //              the pairing heap. This should be a reference for speed.
  //              It MUST be const because we cannot allow it to be
  //              modified, as that might make it no longer be the most
  //              extreme element.
  // Runtime: O(1)
  virtual const TYPE &top() const {
    // TODO: Implement this function
    return root->elt; // TODO: Delete or change this line
  }                   // top()

  // Description: Get the number of elements in the pairing heap.
  // Runtime: O(1)
  [[nodiscard]] virtual std::size_t size() const {
    // TODO: Implement this function
    return nodeCount; // TODO: Delete or change this line
  }                   // size()

  // Description: Return true if the pairing heap is empty.
  // Runtime: O(1)
  [[nodiscard]] virtual bool empty() const {
    // TODO: Implement this function
    return root == nullptr; // TODO: Delete or change this line
  }                         // empty()

  // Description: Updates the priority of an element already in the pairing
  //              heap by replacing the element refered to by the Node with
  //              new_value.  Must maintain pairing heap invariants.
  //
  // PRECONDITION: The new priority, given by 'new_value' must be more
  //              extreme (as defined by comp) than the old priority.
  //
  // Runtime: As discussed in reading material.
  void updateElt(Node *node, const TYPE &new_value) {
    // TODO: Implement this function
    node->elt = new_value;
    if (node == root) {
      return;
    }
    if (node->parent->child == node) {
      node->parent->child = node->sibling;
    } else {
      Node *leftMost = node->parent->child;
      while (leftMost->sibling != node) {
        leftMost = leftMost->sibling;
      }
      leftMost->sibling = node->sibling;
    }
    node->parent = nullptr;
    node->sibling = nullptr;

    root = meld(root, node);

  } // updateElt()

  // Description: Add a new element to the pairing heap. Returns a Node*
  //              corresponding to the newly added element.
  // Runtime: O(1)
  // NOTE: Whenever you create a node, and thus return a Node *, you must
  //       be sure to never move or copy/delete that node in the future,
  //       until it is eliminated by the user calling pop(). Remember this
  //       when you implement updateElt() and updatePriorities().
  Node *addNode(const TYPE &val) {
    // TODO: Implement this function
    Node *newNode = new Node(val);
    if (root == nullptr) {
      this->root = newNode;
    } else {
      this->root = meld(root, newNode);
    }
    this->nodeCount++;
    return newNode;
  } // addNode()

private:
  // TODO: Add any additional member variables or member functions you
  // require here.
  // TODO: We recommend creating a 'meld' function (see the Pairing Heap
  // papers).
  Node *meld(Node *root, Node *newNode) {
    if (root == nullptr) {
      return newNode;
    }
    if (newNode == nullptr) {
      return root;
    }
    if (this->compare(root->getElt(), newNode->getElt())) {
      root->parent = newNode;
      if (newNode->child != nullptr) {
        root->sibling = newNode->child;
      }
      newNode->child = root;

      return newNode;
    } else {
      newNode->parent = root;
      if (root->child != nullptr && root->child != newNode) {
        newNode->sibling = root->child;
      }
      root->child = newNode;
    }
    return root;
  }

  // NOTE: For member variables, you are only allowed to add a "root
  //       pointer" and a "count" of the number of nodes. Anything else
  //       (such as a deque) should be declared inside of member functions
  //       as needed.
  Node *root;
  size_t nodeCount;
};

#endif // PAIRINGPQ_H
