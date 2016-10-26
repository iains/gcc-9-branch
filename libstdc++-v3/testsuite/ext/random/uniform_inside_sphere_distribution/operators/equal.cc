// { dg-do run { target cxx11 } }
// { dg-require-cstdint "" }
//
// Copyright (C) 2014-2016 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// Class template uniform_inside_sphere_distribution

#include <ext/random>
#include <testsuite_hooks.h>

void
test01()
{
  bool test [[gnu::unused]] = true;

  __gnu_cxx::uniform_inside_sphere_distribution<3> u(2.0), v(2.0);

  VERIFY( u == v );
}

int
main()
{
  test01();
  return 0;
}
