#pragma once

#include <atomic>
#include <functional>
#include <memory>

namespace ac::util {

template<typename Type> struct Node {
  Node *_next;
  Type item;
};

template<typename T, typename Alloc = std::allocator<Node<T>>>
class LocklessLIFOQueue {
  using value_type = Node<T>;

public:
  LocklessLIFOQueue(const Alloc& allocator = Alloc())
   : _begin(nullptr)
   , _allocator(allocator)
  {}

  void push(T item)
  {
    using allocator_traits = typename std::allocator_traits<decltype(_allocator)>::template rebind_traits<Node<T>>;
    value_type* new_node = allocator_traits::allocate(_allocator, 1);
    new((void *) new_node) value_type ({ nullptr, item });

    value_type* begin = _begin;
    do {
      new_node->_next = begin;
    } while(!_begin.compare_exchange_strong(begin, new_node));
  }

  void consume_all(std::function<void (const T&)> func)
  {
    value_type* it = pop_all();

    while (it)
    {
      value_type* next = it->_next;
      func(it->item);
      using allocator_traits = typename std::allocator_traits<decltype(_allocator)>::template rebind_traits<Node<T>>;
      allocator_traits::deallocate(_allocator, it, 1);
      it = next;
    };
  }

  bool empty() const { return _begin == nullptr; }

protected:
  value_type *pop_all()
  {
    value_type* node = nullptr;
    value_type* out = nullptr;
    do {
      node = out = _begin;
    } while(!_begin.compare_exchange_strong(node, nullptr));
    return out;
  }

  std::atomic<value_type *> _begin;
  Alloc _allocator;
};

} // namespace ac::util
