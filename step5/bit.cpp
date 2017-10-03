//------------------------------------------------------------------------
#include <cstdio>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <fstream>
#include <functional>
//------------------------------------------------------------------------
typedef unsigned __int128 mbit;
//------------------------------------------------------------------------
mbit column_bits[9] = {};
bool hlb_table[8][6] = {{true, true, true, true, true, true}, {true, false, true, false, false, false}, {false, true, false, false, true, false}, {true, true, false, false, false, false}, {false, false, false, true, false, true}, {false, false, true, true, false, false}, {false, false, false, false, true, true}, {true, true, true, true, true, true}};
int perm3[6][3] = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};
//------------------------------------------------------------------------
void
mysort(mbit v[9]) {
  int j;
  for (int i = 1; i < 9; i++) {
    mbit tmp = v[i];
    if (v[i - 1] < tmp) {
      j = i;
      do {
        v[j] = v[j - 1];
        j--;
      } while (j > 0 && v[j - 1] < tmp);
      v[j] = tmp;
    }
  }
}
//------------------------------------------------------------------------
mbit left_most_bit(mbit v) {
  mbit vt = 0;
  while (v) {
    vt = v;
    v ^= v & (-v);
  }
  return vt;
}
//------------------------------------------------------------------------
void
show_bit(mbit a) {
  for (int i = 0; i < 81; i++) {
    if (a & (mbit(1) << (80 - i))) {
      printf("1");
    } else {
      printf("0");
    }
  }
  printf("\n");
}
//------------------------------------------------------------------------
class Sudoku {
private:
  unsigned char data[81];
  mbit bdata[9];
  void array2bit(void) {
    for (int i = 0; i < 9; i++) {
      bdata[i] = 0;
    }
    for (int i = 0; i < 81; i++) {
      if (data[i] != 0) {
        bdata[data[i] - 1] |= mbit(1) << (80 - i);
      }
    }
  }

public:
  void bit2array(void) {
    std::fill(&data[0], &data[81], 0);
    for (int i = 0; i < 81; i++) {
      for (int j = 0; j < 9; j++) {
        if (bdata[j] & (mbit(1) << i)) {
          data[80 - i] = j + 1;
          continue;
        }
      }
    }
  }

  Sudoku() {
    for (int i = 0; i < 81; i++) {
      data[i] = 9;
    }
    array2bit();
  }

  Sudoku(Sudoku const &g2) {
    for (int i = 0; i < 9; i++) {
      bdata[i] = g2.bdata[i];
    }
  }

  Sudoku(const char* str) {
    for (int i = 0; i < 81; i++) {
      data[i] = str[i] - '0';
    }
    array2bit();
  }
  Sudoku(mbit bdata2[9]) {
    for (int i = 0; i < 9; i++) {
      bdata[i] = bdata2[i];
    }
  }
  Sudoku(unsigned char data2[81]) {
    for (int i = 0; i < 81; i++) {
      data[i] = data2[i];
    }
    array2bit();
  }

  bool operator==(const Sudoku &rhs) const {
    for (int i = 0; i < 9; i++) {
      if (rhs.bdata[i] != this->bdata[i])return false;
    }
    return true;
  }
  bool operator!=(const Sudoku &rhs) const {
    return !(*this == rhs);
  }

  bool operator<(const Sudoku &rhs) const {
    mbit vmax = 0;
    bool less = false;
    for (int i = 0; i < 9; i++) {
      mbit v = bdata[i] ^ rhs.bdata[i];
      v = left_most_bit(v);
      if (v >= vmax) {
        vmax = v;
        less = (bdata[i] < rhs.bdata[i]);
      }
    }
    return less;
  }

  void headline_bits(int &h1, int &h2, int &h3) {
    mbit v = 0;
    for (int i = 0; i < 9; i++) {
      v |= bdata[i];
    }
    h1 = 0;
    h2 = 0;
    h3 = 0;
    for (int i = 0; i < 3; i++) {
      if (v & (mbit(1) << (80 - i))) h1 += (1 << (2 - i));
      if (v & (mbit(1) << (77 - i))) h2 += (1 << (2 - i));
      if (v & (mbit(1) << (74 - i))) h3 += (1 << (2 - i));
    }
  }

  int headline_index(void) {
    mbit v = 0;
    for (int i = 0; i < 9; i++) {
      v |= bdata[i];
    }
    std::vector<int> a(3, 0);
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 3; k++) {
        if (v & (mbit(1) << (80 - k - j * 3)))a[j]++;
      }
    }
    std::sort(a.begin(), a.end());
    int index = 0;
    for (int i = 0; i < 3; i++) {
      int t = ((1 << a[i]) - 1) << (2 - i) * 3;
      index += t;
    }
    return index;
  }

  int headbox_index(bool sort) {
    int min = 16 * 3 + 4 * 3 + 3;
    mbit v = 0;
    for (int i = 0; i < 9; i++) {
      v |= bdata[i];
    }
    for (int i = 0; i < 3; i++) {
      std::vector<int> a(3, 0);
      for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 3; k++) {
          if (v & (mbit(1) << (80 - k - (j + i * 3) * 3)))a[j]++;
        }
      }
      if (sort) {
        std::sort(a.begin(), a.end());
      }
      int index = 0;
      for (int i = 0; i < 3; i++) {
        int t = ((1 << a[i]) - 1) << (2 - i) * 3;
        index += t;
      }
      if (index < min) {
        min = index;
      }
    }
    return min;
  }

  void show(void) {
    bit2array();
    for (int i = 0; i < 81; i++) {
      printf("%c", data[i] + '0');
    }
    printf("\n");
  }

  std::string str(void) {
    bit2array();
    std::stringstream ss;
    for (int i = 0; i < 81; i++) {
      ss << (int)(data[i]);
    }
    return ss.str();
  }

  Sudoku head(void) {
    mbit bdata2[9];
    mbit mask = ((mbit(1) << 27) - 1) << 54;
    for (int i = 0; i < 9; i++) {
      bdata2[i] = bdata[i] & mask;
    }
    return Sudoku(bdata2);
  }


  Sudoku renumbering(void) {
    mbit bdata2[9];
    for (int i = 0; i < 9; i++) {
      bdata2[i] = bdata[i];
    }
    //std::sort(&bdata2[0], &bdata2[9], std::greater<mbit>());
    mysort(bdata2);
    return Sudoku(bdata2);
  }

  Sudoku perm_restrbox2(int a[3], int pos) {
    mbit bdata2[9] = {};
    mbit mask = ((mbit(1) << 81) - 1 ) ^ ((mbit(1) << 27) - 1) << (27 * (1 - pos));
    const mbit t0 = ((mbit(1) << 9) - 1) << (45 - pos * 27);
    const mbit t1 = ((mbit(1) << 9) - 1) << (36 - pos * 27);
    const mbit t2 = ((mbit(1) << 9) - 1) << (27 - pos * 27);
    mbit v[3];
    for (int i = 0; i < 9; i++) {
      v[0] = (bdata[i] & t0) >> (45 - pos * 27);
      v[1] = (bdata[i] & t1) >> (36 - pos * 27);
      v[2] = (bdata[i] & t2) >> (27 - pos * 27);
      bdata2[i] = bdata[i] & mask;
      bdata2[i] |= v[a[0]] << (45 - pos * 27);
      bdata2[i] |= v[a[1]] << (36 - pos * 27);
      bdata2[i] |= v[a[2]] << (27 - pos * 27);
    }
    return Sudoku(bdata2);
  }

  Sudoku perm_restrbox(int ai[3], int aj[3]) {
    mbit bdata2[9];
    const mbit mask = ((mbit(1) << 27) - 1) << 54;
    const mbit t0 = ((mbit(1) << 9) - 1) << 45;
    const mbit t1 = ((mbit(1) << 9) - 1) << 36;
    const mbit t2 = ((mbit(1) << 9) - 1) << 27;
    const mbit t3 = ((mbit(1) << 9) - 1) << 18;
    const mbit t4 = ((mbit(1) << 9) - 1) << 9;
    const mbit t5 = ((mbit(1) << 9) - 1) << 0;
    mbit v[6];
    for (int i = 0; i < 9; i++) {
      v[0] = (bdata[i] & t0) >> 45;
      v[1] = (bdata[i] & t1) >> 36;
      v[2] = (bdata[i] & t2) >> 27;
      v[3] = (bdata[i] & t3) >> 18;
      v[4] = (bdata[i] & t4) >> 9;
      v[5] = (bdata[i] & t5) >> 0;
      bdata2[i] = bdata[i] & mask;
      bdata2[i] |= v[ai[0]] << 45;
      bdata2[i] |= v[ai[1]] << 36;
      bdata2[i] |= v[ai[2]] << 27;
      bdata2[i] |= v[aj[0] + 3] << 18;
      bdata2[i] |= v[aj[1] + 3] << 9;
      bdata2[i] |= v[aj[2] + 3] << 0;
    }
    return Sudoku(bdata2);
  }

  Sudoku perm_columns(int ai[3], int aj[3], int ak[3]) {
    mbit bdata2[9];
    mbit v[9];
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        v[j] = (bdata[i] & column_bits[j]) >> (8 - j);
      }
      bdata2[i] = v[ai[0]] << 8;
      bdata2[i] |= v[ai[1]] << 7;
      bdata2[i] |= v[ai[2]] << 6;
      bdata2[i] |= v[aj[0] + 3] << 5;
      bdata2[i] |= v[aj[1] + 3] << 4;
      bdata2[i] |= v[aj[2] + 3] << 3;
      bdata2[i] |= v[ak[0] + 6] << 2;
      bdata2[i] |= v[ak[1] + 6] << 1;
      bdata2[i] |= v[ak[2] + 6] << 0;
    }
    return Sudoku(bdata2);
  }

  Sudoku perm_toprbox(int a[3]) {
    mbit bdata2[9];
    mbit mask = ((mbit(1) << 54) - 1);
    mbit t0 = ((mbit(1) << 9) - 1) << 72;
    mbit t1 = ((mbit(1) << 9) - 1) << 63;
    mbit t2 = ((mbit(1) << 9) - 1) << 54;
    mbit v[3];
    for (int i = 0; i < 9; i++) {
      v[0] = (bdata[i] & t0) >> 18;
      v[1] = (bdata[i] & t1) >> 9;
      v[2] = (bdata[i] & t2) >> 0;
      bdata2[i] = bdata[i] & mask;
      bdata2[i] |= v[a[0]] << 18;
      bdata2[i] |= v[a[1]] << 9;
      bdata2[i] |= v[a[2]] << 0;
    }
    return Sudoku(bdata2);
  }

  Sudoku perm_cbox(int a[3]) {
    mbit bdata2[9];
    mbit t2 = 0;
    for (int i = 0; i < 9; i++) {
      t2 |= (mbit(7) << (i * 9));
    }
    mbit t1 = t2 << 3;
    mbit t0 = t2 << 6;
    mbit v[3];
    for (int i = 0; i < 9; i++) {
      v[0] = (bdata[i] & t0) >> 6;
      v[1] = (bdata[i] & t1) >> 3;
      v[2] = (bdata[i] & t2);
      bdata2[i]  = v[a[0]] << 6;
      bdata2[i] |= v[a[1]] << 3;
      bdata2[i] |= v[a[2]];
    }
    return Sudoku(bdata2);
  }
  Sudoku perm_rbox(int a[3]) {
    mbit bdata2[9];
    mbit v[3];
    mbit t0 = (mbit(1) << 27) - 1;
    mbit t1 = t0 << 27;
    mbit t2 = t0 << 54;
    for (int i = 0; i < 9; i++) {
      v[0] = bdata[i] & t0;
      v[1] = (bdata[i] & t1) >> 27;
      v[2] = (bdata[i] & t2) >> 54;
      bdata2[i]  = v[2 - a[2]];
      bdata2[i] |= v[2 - a[1]] << 27;
      bdata2[i] |= v[2 - a[0]] << 54;
    }
    return Sudoku(bdata2);
  }
  Sudoku transpose(void) {
    bit2array();
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
void
init_bits(void) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      column_bits[i] |= (mbit(1 << (8 - i)) << (j * 9));
    }
  }
}
//------------------------------------------------------------------------
class MinlexSearcher {
private:
  Sudoku min, max;
public:
  void perm_restrbox(Sudoku &g) {
    for (auto ai : perm3) {
      Sudoku g1 = g.perm_restrbox2(ai, 0);
      for (auto aj : perm3) {
        Sudoku g2 = g1.perm_restrbox2(aj, 1).renumbering();
        //Sudoku g2 = g.perm_restrbox(ai,aj).renumbering();
        if (g2 < min) {
          min = g2;
        }
      }
    }
  }
  void perm_columns(Sudoku &g) {
    int h1, h2, h3;
    g.headline_bits(h1, h2, h3);
    for (int k = 0; k < 6; k++) {
      if (!hlb_table[h3][k])continue;
      int *ak = perm3[k];
      for (int j = 0; j < 6; j++) {
        if (!hlb_table[h2][j])continue;
        int *aj = perm3[j];
        for (int i = 0; i < 6; i++) {
          if (!hlb_table[h1][i])continue;
          int *ai = perm3[i];
          Sudoku g2 = g.perm_columns(ai, aj, ak).renumbering();
          if (min.head() < g2.head()) continue;
          perm_restrbox(g2);
        }
      }
    }
  }
  void perm_toprbox(Sudoku &g, int hb_min) {
    for (auto a : perm3) {
      Sudoku g2 = g.perm_toprbox(a);
      if (g2.headline_index() > hb_min)continue;
      perm_columns(g2);
    }
  }
  void perm_cbox(Sudoku &g, int hb_min) {
    for (auto a : perm3) {
      Sudoku g2 = g.perm_cbox(a);
      if (g2.headbox_index(false) > hb_min)continue;
      perm_toprbox(g2, hb_min);
    }
  }

  std::string search(Sudoku &g) {
    min = max;
    std::vector<Sudoku> v;
    std::vector<int> vi;
    for (auto a : perm3) {
      Sudoku g2 = g.perm_rbox(a);
      v.push_back(g2);
      vi.push_back(g2.headbox_index(true));
    }
    Sudoku gt = g.transpose();
    for (auto a : perm3) {
      Sudoku g2 = gt.perm_rbox(a);
      v.push_back(g2);
      vi.push_back(g2.headbox_index(true));
    }
    int hb_min = *std::min_element(vi.begin(), vi.end());
    for (size_t i = 0; i < v.size(); i++) {
      if (vi[i] > hb_min) continue;
      perm_cbox(v[i], hb_min);
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
  init_bits();
  if (argc > 1) {
    input(argv[1]);
  } else {
    test();
  }
}
//------------------------------------------------------------------------
