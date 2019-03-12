//------------------------------------------------------------------------
#include <cstdio>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
//------------------------------------------------------------------------
int perm3[6][3] = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};
//------------------------------------------------------------------------
class Sudoku {
public:
  unsigned char data[81];

  Sudoku() {
    for (int i = 0; i < 81; i++) {
      data[i] = 9;
    }
  }

  Sudoku(const char* str) {
    for (int i = 0; i < 81; i++) {
      data[i] = str[i] - '0';
    }
  }

  Sudoku & operator=(Sudoku const &g2) {
    for (int i = 0; i < 81; i++) {
      data[i] = g2.data[i];
    }
    return (*this);
  }

  Sudoku(unsigned char data2[81]) {
    for (int i = 0; i < 81; i++) {
      data[i] = data2[i];
    }
  }

  bool operator==(const Sudoku &rhs) const {
    for (int i = 0; i < 81; i++) {
      if (rhs.data[i] != this->data[i])return false;
    }
    return true;
  }
  bool operator!=(const Sudoku &rhs) const {
    return !(*this == rhs);
  }

  bool operator<(const Sudoku &rhs) const {
    for (int i = 0; i < 81; i++) {
      if (rhs.data[i] != this->data[i]) {
        return (this->data[i] < rhs.data[i]);
      }
    }
    return false;
  }

  void show(void) {
    for (int i = 0; i < 81; i++) {
      printf("%c", data[i] + '0');
    }
    printf("\n");
  }

  std::string str(void) {
    std::stringstream ss;
    for (int i = 0; i < 81; i++) {
      ss << (int)(data[i]);
    }
    return ss.str();
  }

  Sudoku head(void) {
    unsigned char data2[81] = {};
    for (int i = 0; i < 27; i++) {
      data2[i] = data[i];
    }
    return Sudoku(data2);
  }

  Sudoku renumbering(void) {
    unsigned char data2[81] = {};
    int h[9] = {};
    int index = 0;
    for (int i = 0; i < 81; i++) {
      if (data[i] == 0)continue;
      unsigned char c = data[i];
      if (h[c - 1] == 0) {
        index++;
        h[c - 1] = index;
      }
      data2[i] = h[c - 1];
    }
    return Sudoku(data2);
  }

  Sudoku perm_restrbox(int ai[3], int aj[3]) {
    unsigned char data2[81];
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 9; j++) {
        data2[j + i * 9] = data[j + i * 9];
        data2[j + (i + 3) * 9] = data[j + (ai[i] + 3) * 9];
        data2[j + (i + 6) * 9] = data[j + (aj[i] + 6) * 9];
      }
    }
    return Sudoku(data2);
  }

  Sudoku perm_columns(int ai[3], int aj[3], int ak[3]) {
    unsigned char data2[81];
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 3; j++) {
        data2[j + i * 9] = data[ai[j] + i * 9];
        data2[j + 3 + i * 9] = data[aj[j] + 3 + i * 9];
        data2[j + 6 + i * 9] = data[ak[j] + 6 + i * 9];
      }
    }
    return Sudoku(data2);
  }

  Sudoku perm_toprbox(int a[3]) {
    unsigned char data2[81];
    for (int i = 0; i < 9; i++) {
      data2[i] = data[i + a[0] * 9];
      data2[i + 9] = data[i + a[1] * 9];
      data2[i + 18] = data[i + a[2] * 9];
    }
    for (int i = 27; i < 81; i++) {
      data2[i] = data[i];
    }
    return Sudoku(data2);
  }

  Sudoku perm_cbox(int a[3]) {
    unsigned char data2[81];
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 3; j++) {
        data2[j + i * 9] = data[a[0] * 3 + j + i * 9];
        data2[j + 3 + i * 9] = data[a[1] * 3 + j + i * 9];
        data2[j + 6 + i * 9] = data[a[2] * 3 + j + i * 9];
      }
    }
    return Sudoku(data2);
  }
  Sudoku perm_rbox(int a[3]) {
    unsigned char data2[81];
    for (int i = 0; i < 27; i++) {
      data2[i] = data[i + a[0] * 27];
      data2[i + 27] = data[i + a[1] * 27];
      data2[i + 54] = data[i + a[2] * 27];
    }
    return Sudoku(data2);
  }
  Sudoku transpose(void) {
    unsigned char data2[81];
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        data2[j + i * 9] = data[i + j * 9];
      }
    }
    return Sudoku(data2);
  }
};
//------------------------------------------------------------------------
class MinlexSearcher {
private:
  Sudoku min, max;
public:
  void perm_restrbox(Sudoku &g) {
    for (auto ai : perm3) {
      for (auto aj : perm3) {
        Sudoku g2 = g.perm_restrbox(ai, aj).renumbering();
        if (g2 < min) {
          min = g2;
        }
      }
    }
  }

  void perm_columns(Sudoku &g) {
    for (auto ai : perm3) {
      for (auto aj : perm3) {
        for (auto ak : perm3) {
          Sudoku g2 = g.perm_columns(ai, aj, ak).renumbering();
          if (min.head() < g2.head()) continue;
          perm_restrbox(g2);
        }
      }
    }
  }
  void perm_toprbox(Sudoku &g) {
    for (auto a : perm3) {
      Sudoku g2 = g.perm_toprbox(a);
      perm_columns(g2);
    }
  }
  void perm_cbox(Sudoku &g) {
    for (auto a : perm3) {
      Sudoku g2 = g.perm_cbox(a);
      perm_toprbox(g2);
    }
  }
  std::string search(Sudoku &g) {
    min = max;
    for (auto a : perm3) {
      Sudoku g2 = g.perm_rbox(a);
      perm_cbox(g2);
    }
    Sudoku gt = g.transpose();
    for (auto a : perm3) {
      Sudoku g2 = gt.perm_rbox(a);
      perm_cbox(g2);
    }
    return min.str();
  }
};
//------------------------------------------------------------------------
void
test(void) {
  std::string str = "207005000000340000150000009005000001040000320000016500000002084700000010010580000";
  std::string ans = "000000012000034005006007300001300007053080000080000100010005090200100000700400030";
  Sudoku g(str.c_str());
  MinlexSearcher s;
  g.show();
  std::string min = s.search(g);
  std::cout << min << std::endl;
  if (min == ans) {
    std::cout << "OK" << std::endl;
  } else {
    std::cout << "NG" << std::endl;
  }
}
//------------------------------------------------------------------------
void
input(const char*filename) {
  std::string line;
  MinlexSearcher s;
  std::ifstream ifs(filename);
  while (getline(ifs, line)) {
    Sudoku g(line.c_str());
    std::cout << s.search(g) << std::endl;
  }
}
//------------------------------------------------------------------------
int
main(int argc, char **argv) {
  if (argc > 1) {
    input(argv[1]);
  } else {
    test();
  }
}
//------------------------------------------------------------------------
