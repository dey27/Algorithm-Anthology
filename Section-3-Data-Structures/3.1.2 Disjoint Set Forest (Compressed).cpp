/*

Maintain a set of elements partitioned into non-overlapping subsets using a
collection of trees. Each partition is assigned a unique representative known as
the parent, or root. The following implements two well-known optimizations known
as union-by-rank and path compression. This version uses an std::map for storage
and coordinate compression (thus, element types must meet the requirements of
key types for std::map).

- make_set(x) creates a new partition consisting of the single element x, which
  must not have been previously added to the data structure.
- is_united(x, y) returns whether elements x and y belong to the same partition.
- unite(x, y) replaces the partitions containing x and y with a single new
  partition consisting of the union of elements in the original partitions.
- get_all_sets() returns all current partitions as a vector of vectors.

A precondition to the last three operations is that make_set() must have been
previously called on their arguments.

Time Complexity:
- O(1) per call to the constructor.
- O(log n) per call to make_set(), where n is the number of elements that have
  been added via make_set() so far.
- O(a(n) log n) per call to is_united() and unite(), where n is the number of
  elements that have been added via make_set() so far, and a(n) is the extremely
  slow growing inverse of the Ackermann function (effectively a very small
  constant for all practical values of n).
- O(n) per call to get_all_sets().

Space Complexity:
- O(n) for storage of the disjoint set forest elements.
- O(n) auxiliary per call to get_all_sets().
- O(1) auxiliary per call to all other operations.

*/

#include <map>
#include <vector>

template<class T> class disjoint_set_forest {
  int num_elements, num_sets;
  std::map<T, int> id;
  std::vector<int> root, rank;

  int find_root(int x) {
    if (root[x] != x) {
      root[x] = find_root(root[x]);
    }
    return root[x];
  }

 public:
  disjoint_set_forest() {
    num_elements = num_sets = 0;
  }

  int elements() const {
    return num_elements;
  }

  int sets() const {
    return num_sets;
  }

  void make_set(const T &x) {
    if (id.find(x) != id.end()) {
      return;
    }
    id[x] = num_elements;
    root.push_back(num_elements++);
    rank.push_back(0);
    num_sets++;
  }

  bool is_united(const T &x, const T &y) {
    return find_root(id[x]) == find_root(id[y]);
  }

  void unite(const T &x, const T &y) {
    int r1 = find_root(id[x]);
    int r2 = find_root(id[y]);
    if (r1 == r2) {
      return;
    }
    num_sets--;
    if (rank[r1] < rank[r2]) {
      root[r1] = r2;
    } else {
      root[r2] = r1;
      if (rank[r1] == rank[r2]) {
        rank[r1]++;
      }
    }
  }

  std::vector<std::vector<T> > get_all_sets() {
    std::map<int, std::vector<T> > tmp;
    for (typename std::map<T, int>::iterator it = id.begin(); it != id.end();
         ++it) {
      tmp[find_root(it->second)].push_back(it->first);
    }
    std::vector<std::vector<T> > res;
    for (typename std::map<int, std::vector<T> >::iterator it = tmp.begin();
         it != tmp.end(); ++it) {
      res.push_back(it->second);
    }
    return res;
  }
};

/*** Example Usage and Output:

7 elements in 3 set:
[a, b, f], [c], [d, e, g]

***/

#include <iostream>
using namespace std;

int main() {
  disjoint_set_forest<char> dsf;
  for (char c = 'a'; c <= 'g'; c++) {
    dsf.make_set(c);
  }
  dsf.unite('a', 'b');
  dsf.unite('b', 'f');
  dsf.unite('d', 'e');
  dsf.unite('d', 'g');
  cout << dsf.elements() << " elements in ";
  cout << dsf.sets() << " set:" << endl;
  vector< vector<char> > s = dsf.get_all_sets();
  for (int i = 0; i < (int)s.size(); i++) {
    cout << (i > 0 ? ", [" : "[");
    for (int j = 0; j < (int)s[i].size(); j++) {
      cout << (j > 0 ? ", " : "") << s[i][j];
    }
    cout << "]";
  }
  cout << endl;
  return 0;
}
