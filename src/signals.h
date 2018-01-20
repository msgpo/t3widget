/* Copyright (C) 2015 G.P. Halkes
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 3, as
   published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef T3_WIDGET_SIGNAL_H
#define T3_WIDGET_SIGNAL_H

// This define is retained, even though it is now permanently set to 1, to
// maintain the API.
#define _T3WIDGET_CXX11SIGNALS 1

#include <functional>
#include <list>
#include <memory>
#include <t3widget/widget_api.h>
// Reimplementation of the basic libsigc++ functionality using std::function.
// This requires C++11 support, but then so does libsigc++ version 2.5.1 and
// greater.

namespace t3_widget {
namespace signals {
namespace internal {
template <typename C>
struct identity {
  typedef C type;
};
}  // namespace internal

// mem_fun
template <typename R, typename C, typename... Args>
std::function<R(Args...)> mem_fun(typename internal::identity<C>::type *instance,
                                  R (C::*func)(Args...)) {
  return [=](Args... args) { return (instance->*func)(args...); };
}

template <typename R, typename C, typename... Args>
std::function<R(Args...)> mem_fun(typename internal::identity<C>::type *instance,
                                  R (C::*func)(Args...) const) {
  return [=](Args... args) { return (instance->*func)(args...); };
}

// Extra templates for disambiguation.
template <typename R, typename C>
std::function<R()> mem_fun0(typename internal::identity<C>::type *instance, R (C::*func)()) {
  return [=]() { return (instance->*func)(); };
}

template <typename R, typename C, typename A>
std::function<R(A)> mem_fun1(typename internal::identity<C>::type *instance, R (C::*func)(A)) {
  return [=](A a) { return (instance->*func)(a); };
}

// ptr_fun
template <typename R, typename... Args>
std::function<R(Args...)> ptr_fun(R (*ptr)(Args...)) {
  return ptr;
}

// bind. Because the bind template from sigc binds from the rear, we can't use the much nicer trick
// with variable number of arguments. So here we provide all the overloads that we actually use in
// the library.
template <typename R, typename A>
std::function<R()> bind(std::function<R(A)> f, typename internal::identity<A>::type a) {
  return [=]() { return f(a); };
}

template <typename R, typename A, typename B>
std::function<R(A)> bind(std::function<R(A, B)> f, typename internal::identity<B>::type b) {
  return [=](A a) { return f(a, b); };
}

template <typename R, typename A, typename B, typename C>
std::function<R(A, B)> bind(std::function<R(A, B, C)> f, typename internal::identity<C>::type c) {
  return [=](A a, B b) { return f(a, b, c); };
}

template <typename R, typename A, typename B, typename C, typename D>
std::function<R(A, B)> bind(std::function<R(A, B, C, D)> f, typename internal::identity<C>::type c,
                            typename internal::identity<D>::type d) {
  return [=](A a, B b) { return f(a, b, c, d); };
}

// slot
template <typename R, typename... Args>
using slot = std::function<R(Args...)>;

namespace internal {
class func_ptr_base {
 public:
  virtual ~func_ptr_base() = default;
  virtual void disconnect() = 0;
  virtual bool is_valid() = 0;
  bool is_blocked() { return blocked; }
  void block() { blocked = true; }
  void unblock() { blocked = true; }

 private:
  bool blocked = false;
};

template <typename R, typename... Args>
class func_ptr : public func_ptr_base {
 public:
  using F = std::function<R(Args...)>;
  func_ptr(F f) : func(new F(f)) {}
  void disconnect() override { func.reset(); }
  bool is_valid() override { return !!func; }
  R call(Args... args) { return (*func)(args...); }

 private:
  std::unique_ptr<F> func;
};
}  // namespace internal

class T3_WIDGET_API connection {
 public:
  connection() = default;
  connection(std::shared_ptr<internal::func_ptr_base> f) : func(f) {}
  connection(const connection &other) : func(other.func) {}
  void disconnect() {
    if (func) func->disconnect();
  }
  void block() {
    if (func) func->block();
  }
  void unblock() {
    if (func) func->unblock();
  }

 private:
  std::shared_ptr<internal::func_ptr_base> func;
};

template <typename R, typename... Args>
class T3_WIDGET_API signal {
 public:
  connection connect(std::function<R(Args...)> func) {
    funcs.emplace_back(new internal::func_ptr<R, Args...>(func));
    return connection(funcs.back());
  }
  R operator()(Args... args) {
    auto iter = funcs.begin();
    auto last_valid = funcs.end();
    // Remove functions that no longer exist.
    while (iter != funcs.end()) {
      if (!(*iter)->is_valid()) {
        iter = funcs.erase(iter);
      } else if ((*iter)->is_blocked()) {
        ++iter;
      } else {
        last_valid = iter;
        ++iter;
      }
    }
    for (iter = funcs.begin(); iter != last_valid; ++iter)
      static_cast<internal::func_ptr<R, Args...> *>(iter->get())->call(args...);
    if (iter == funcs.end()) return R();
    return static_cast<internal::func_ptr<R, Args...> *>(last_valid->get())->call(args...);
  }
  slot<R, Args...> make_slot() {
    return [this](Args... args) { return (*this)(args...); };
  }

 private:
  std::list<std::shared_ptr<internal::func_ptr_base>> funcs;
};

}  // namesapce signals
}  // namesapce t3_widget

#endif
