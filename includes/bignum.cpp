#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

namespace {
  constexpr int toPower(int a, int n) {
    int ret = 1;
    for (int i = 0; i < n; i++)
      ret *= a;
    return ret;
  }
};

template<int Digit> class Bignum;
template<int Digit>
std::ostream& operator<< (std::ostream &o, Bignum<Digit> &x);

template<int Digit>
class Bignum {
  public:
    static const int Base = toPower(10, Digit);
    bool negative;
    std::vector<int> a;
    Bignum(int val = 0);
    Bignum<Digit>& operator *= (const int k);
    Bignum<Digit>& operator /= (const int k);
    Bignum<Digit>& operator += (const Bignum<Digit> &x);
    Bignum<Digit>& operator -= (const Bignum<Digit> &x);
    friend std::ostream& operator<< <> (std::ostream &o, Bignum<Digit> &x);
  protected:
    void handle();
};

template<int Digit>
Bignum<Digit>::Bignum(int val) {
  if (val < 0) {
    negative = true;
    val = -val;
  } else {
    negative = false;
  }

  a.resize(1);
  a[0] = val;
  handle();
}

template<int Digit>
void Bignum<Digit>::handle() {
  for (int i = 0; i < (int)a.size(); i++) {
    if (a[i] >= Base) {
      if (i+1 == (int)a.size()) a.push_back(0);
      a[i+1] += a[i] / Base;
      a[i] %= Base;
    }
  }

  while (a.size() > 1 && a.back() == 0)
    a.pop_back();

  if (a.size() == 1 && a[0] == 0) negative = false;
}

template<int Digit>
Bignum<Digit>& Bignum<Digit>::operator *= (const int k) {
  for (auto &&v : a) v *= k;
  handle();
  return (*this);
}

template<int Digit>
Bignum<Digit>& Bignum<Digit>::operator /= (const int k) {
  int r = 0;
  for (int i = (int)a.size() - 1; i >= 0; i--) {
    r = r * Base + a[i];
    a[i] = r / k;
    r %= k;
  }
  handle();
  return (*this);
}

template<int Digit>
Bignum<Digit>& Bignum<Digit>::operator += (const Bignum<Digit> &x) {
  if (negative != x.negative) {
    negative = !negative;
    (*this) -= x;
    negative = !negative;
    return (*this);
  }

  a.resize(std::max(a.size(), x.a.size()), 0);
  for (auto i = 0; i < x.a.size(); i++)
    a[i] += x.a[i];
  handle();
  return (*this);
}

template<int Digit>
Bignum<Digit>& Bignum<Digit>::operator -= (const Bignum<Digit> &x) {
  if (negative != x.negative) {
    negative = !negative;
    (*this) += x;
    negative = !negative;
    return (*this);
  }

  a.resize(std::max(a.size(), x.a.size()), 0);
  int r = 0;
  for (auto i = 0; i < a.size(); i++) {
    a[i] = a[i] + r - (i < x.a.size() ? x.a[i] : 0);
    if (a[i] < 0) {
      a[i] += Base;
      r = -1;
    } else
      r = 0;
  }
  
  if (r == -1) {
    negative = !negative;
    a[0] = Base - a[0];
    for (auto i = 1; i < a.size(); i++)
      a[i] = Base - 1 - a[i];
  }
  handle();
  return (*this);
}

template<int Digit>
std::ostream& operator<< (std::ostream& out, Bignum<Digit> &x) {
  if (x.negative) out << "-";
  out << x.a.back();
  for (int i = (int)x.a.size() - 2; i >= 0; i--) {
    out << std::setfill('0') << std::setw(Digit) << x.a[i];
  }
  return out;
}


/*
int main() {
  Bignum<3> v(100000), w(-998);
  std::cout << w << std::endl;
  w += v;
  std::cout << w << std::endl;
}
*/
