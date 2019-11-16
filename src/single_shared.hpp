#pragma once

#include <memory>

template <class T>
using single_shared = std::__shared_ptr<T, __gnu_cxx::_S_single>;

template <class T>
using mutex_shared = std::__shared_ptr<T, __gnu_cxx::_S_mutex>;

template <class T>
using atomic_shared = std::__shared_ptr<T, __gnu_cxx::_S_atomic>;
