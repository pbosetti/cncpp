/*
 ____ _____ _      __     __        _
/ ___|_   _| |     \ \   / /__  ___| |_ ___  _ __
\___ \ | | | |      \ \ / / _ \/ __| __/ _ \| '__|
 ___) || | | |___    \ V /  __/ (__| || (_) | |
|____/ |_| |_____|    \_/ \___|\___|\__\___/|_|

Standard Template Library (STL) Vector
Author: Davide Stocco, 2025
*/

#include <iostream>
#include <vector>

using namespace std;

int main()
{
  // Declaring vectors
  vector<int> v1 = {1, 2, 3};
  vector<int> v2{10, 20, 30};
  vector<int> v3;
  vector<int> v4(5, 10);
  vector<int> v5(10);

  // Vector of vectors
  vector<vector<int>> vec;
  vector<int> v2d = {1, 2, 3};
  vec.push_back(v2d);
  vector<vector<int>> matrix(5, vector<int>(5, 0));

  // Accessing elements
  cout << "Element at index 0: " << v1.at(0) << endl;
  cout << "Element at index 1: " << v1[1] << endl;

  // Front and Back
  cout << "First Element: " << v1.front() << endl;
  cout << "Last Element: " << v1.back() << endl;

  // Using auto keyword
  auto num = 100;
  auto & num_ref = num;
  num_ref = 200;
  cout << "Auto Example: " << num << endl;

  // Foreach loop
  for (auto & num : v1) {
    num *= 2;
  }
  for (auto num : v1) {
    cout << num << " ";
  }
  cout << endl;

  // Iterators
  vector<int>::iterator it = v1.begin();
  cout << "First element using iterator: " << *it << endl;

  // Reverse iterators
  for (auto rit = v1.rbegin(); rit != v1.rend(); ++rit) {
    cout << *rit << " ";
  }
  cout << endl;

  // Adding elements
  v1.push_back(50);
  v1.emplace_back(60);
  v1.insert(v1.begin() + 1, 15);

  // Removing elements
  v1.pop_back();
  v1.erase(v1.begin() + 1);
  v1.erase(v1.begin(), v1.begin() + 2);

  // Swapping vectors
  v1.swap(v2);

  // Size and capacity
  cout << "Size: " << v1.size() << endl;
  cout << "Capacity: " << v1.capacity() << endl;

  // Resizing
  v1.resize(8, 99);
  v1.shrink_to_fit();
  cout << "After resizing: " << v1.size() << " " << v1.capacity() << endl;

  // Checking empty
  cout << "Vector is empty: " << v3.empty() << endl;

  // Clearing vector
  v1.clear();
  cout << "After clearing, size: " << v1.size() << endl;

  return 0;
}