/***************************************************************************
* ALPS library
*
* test_main.h test for fixed-capacity containers
*
* $Id$
*
* Copyright (C) 2002-2003 by Synge Todo <wistaria@comp-phys.org>
*
* Permission is hereby granted, free of charge, to any person or organization 
* obtaining a copy of the software covered by this license (the "Software") 
* to use, reproduce, display, distribute, execute, and transmit the Software, 
* and to prepare derivative works of the Software, and to permit others
* to do so for non-commerical academic use, all subject to the following:
*
* The copyright notice in the Software and this entire statement, including 
* the above license grant, this restriction and the following disclaimer, 
* must be included in all copies of the Software, in whole or in part, and 
* all derivative works of the Software, unless such copies or derivative 
* works are solely in the form of machine-executable object code generated by 
* a source language processor.

* In any scientific publication based in part or wholly on the Software, the
* use of the Software has to be acknowledged and the publications quoted
* on the web page http://www.alps.org/license/ have to be referenced.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT 
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
* DEALINGS IN THE SOFTWARE.
*
**************************************************************************/

#include <boost/random.hpp>
#include <cstdlib>
#include <iostream>
#include <vector>

template<class T>
struct non_pod {
  BOOST_STATIC_CONSTANT(int, magic = -3);

  non_pod() {
    if (init_ == magic) throw std::logic_error("non_pod");
    init_ = magic;
    data_ = -1.0;
  }
  non_pod(const non_pod& x) {
    if (init_ == magic) throw std::logic_error("non_pod");
    init_ = magic;
    data_ = x.data_;
  }
  non_pod(const T& x) {
    if (init_ == magic) throw std::logic_error("non_pod");
    init_ = magic;
    data_ = x;
  }
  ~non_pod() {
    if (init_ != magic) throw std::logic_error("non_pod");
    init_ = 0;
    data_ = -1.0;
  }
  non_pod& operator=(const non_pod& x) {
    if (init_ != magic) throw std::logic_error("non_pod");
    data_ = x.data_;
    return *this;
  }
  non_pod& operator=(const T& x) {
    if (init_ != magic) throw std::logic_error("non_pod");
    data_ = x;
    return *this;
  }
  
  bool operator==(const non_pod& x) const {
    if (init_ != magic || x.init_ != magic) throw std::logic_error("non_pod");
    return data_ == x.data_;
  }
  bool operator!=(const non_pod& x) const { return !operator==(x); }
  
  friend bool operator==(T x, const non_pod& y) { return y == x; }
  friend bool operator!=(T x, const non_pod& y) { return y != x; }
  friend std::ostream& operator<<(std::ostream& os, const non_pod& x) {
    os << x.data_;
    return os;
  }
  
  T data_;
  int init_;
};


template<class S, class T, class U>
bool check(const S& s, const T& t, const U& u) {
  bool check = true;
  if (s.size() != t.size() || s.size() != u.size()) return false;
  typename T::const_iterator t_iter = t.begin();
  typename U::const_iterator u_iter = u.begin();
  for (typename S::const_iterator iter = s.begin(); iter != s.end();
       ++iter) {
#ifdef VERBOSE    
    std::cout << "(" << *iter << "," << *t_iter << "," << *u_iter << ") ";
#endif
    if (*iter != *t_iter || *iter != *u_iter) check = false;
    ++t_iter;
    ++u_iter;
  }
#ifdef VERBOSE    
  std::cout << std::endl;
#endif
  return check;
}


template<class Vec, class RNG>
void make_array(Vec& vec, RNG& rng, std::size_t n) {
  vec.clear();
  for (std::size_t i = 0; i < n; ++i) vec.push_back(rng());
}

template<class S, class T, class U>
void test_main(std::size_t m, std::size_t n) {
  S s;
  T t;
  U u;
    
  std::vector<double> v;
  boost::lagged_fibonacci607 rng;
  
  for (std::size_t i = 0; i < m; ++i) {
#ifdef VERBOSE
    std::cout << i << ' ';
#endif
    double r = rng();
    if (r < 0.1) {
      // clear
#ifdef VERBOSE
      std::cout << "clear\n";
#endif
      s.clear();
      t.clear();
      u.clear();
    } else if (r < 0.2) {
      // push_back
      double d = rng();
      if (s.size() < n) {
#ifdef VERBOSE
        std::cout << "push_back " << d << std::endl;
#endif
        s.push_back(d);
        t.push_back(d);
        u.push_back(d);
      } else {
#ifdef VERBOSE
        std::cout << std::endl;
#endif
      }
#ifdef DEQUE
    } else if (r < 0.25) {
      // push_front
      double d = rng();
      if (s.size() < n) {
#ifdef VERBOSE
        std::cout << "push_front " << d << std::endl;
#endif
        s.push_front(d);
        t.push_front(d);
        u.push_front(d);
      } else {
#ifdef VERBOSE
        std::cout << std::endl;
#endif
      }
#endif // DEQUE
    } else if (r < 0.3) {
      // pop_back
      if (!s.empty()) {
#ifdef VERBOSE
        std::cout << "pop_back\n";
#endif
        s.pop_back();
        t.pop_back();
        u.pop_back();
      } else {
#ifdef VERBOSE
        std::cout << std::endl;
#endif
      }
#ifdef DEQUE      
    } else if (r < 0.35) {
      // pop_front
      if (!s.empty()) {
#ifdef VERBOSE
        std::cout << "pop_front\n";
#endif
        s.pop_front();
        t.pop_front();
        u.pop_front();
      } else {
#ifdef VERBOSE
        std::cout << std::endl;
#endif
      }
#endif // DEQUE
    } else if (r < 0.4) {
      // resize
      double d = rng();
      std::size_t p = int(n * rng());
#ifdef VERBOSE
      std::cout << "resize " << p << ' ' << d << std::endl;
#endif
      s.resize(p, d);
      t.resize(p, d);
      u.resize(p, d);
    } else if (r < 0.7) {
      // insert
      std::size_t p = int(s.size() * rng());
      std::size_t x = int((n - s.size()) * rng());
      if (rng() < 0.5) {
        double d = rng();
#ifdef VERBOSE
	std::cout << "insert " << p << ' ' << x << ' ' << d << std::endl;
#endif
	if (x != 1) {
	  s.insert(s.begin() + p, x, d);
	  t.insert(t.begin() + p, x, d);
	  u.insert(u.begin() + p, x, d);
	} else {
	  s.insert(s.begin() + p, d);
	  t.insert(t.begin() + p, d);
	  u.insert(u.begin() + p, d);
	}
      } else {
#ifdef VERBOSE
	std::cout << "insert sequence\n";
#endif
	make_array(v, rng, x);
	s.insert(s.begin() + p, v.begin(), v.end());
	t.insert(t.begin() + p, v.begin(), v.end());
	u.insert(u.begin() + p, v.begin(), v.end());
      }
    } else {
      // erase
      std::size_t p = int(s.size() * rng());
      std::size_t x = p + int((s.size() - p) * rng());
#ifdef VERBOSE
      std::cout << "erase " << p << ' ' << x << std::endl;
#endif
      if (x != 1) {
	s.erase(s.begin() + p, s.begin() + x);
	t.erase(t.begin() + p, t.begin() + x);
	u.erase(u.begin() + p, u.begin() + x);
      } else {
	s.erase(s.begin() + p);
	t.erase(t.begin() + p);
	u.erase(u.begin() + p);
      }
    } 
    
    if (!check(s, t, u)) {
      std::cout << "Error occured!\n";
      std::exit(-1);
    }
  }
  
  return;
}
