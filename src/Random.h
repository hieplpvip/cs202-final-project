// Adopted from testlib's random_t
// https://github.com/MikeMirzayanov/testlib/blob/master/testlib.h

#ifndef RANDOM_H
#define RANDOM_H

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <set>
#include <string>
#include <vector>

const long long __RANDOM_LONGLONG_MAX = 9223372036854775807LL;

class Random {
private:
  unsigned long long seed;
  static const unsigned long long multiplier = 0x5DEECE66DLL;
  static const unsigned long long addend = 0xBLL;
  static const unsigned long long mask = (1LL << 48) - 1;
  static const int lim = 25;

  long long nextBits(int bits) {
    if (bits <= 48) {
      seed = (seed * multiplier + addend) & mask;
      return (long long)(seed >> (48 - bits));
    } else {
      if (bits > 63)
        __random_fail("Random::nextBits(int bits): n must be less than 64");

      int lowerBitCount = (Random::version == 0 ? 31 : 32);

      long long left = (nextBits(31) << 32);
      long long right = nextBits(lowerBitCount);

      return left ^ right;
    }
  }

  static void __random_fail(const std::string &message) {
    printf("%s", message.c_str());
    exit(1);
  }

  template <typename T>
  static inline T __random_min(const T &a, const T &b) {
    return a < b ? a : b;
  }

  template <typename T>
  static inline T __random_max(const T &a, const T &b) {
    return a > b ? a : b;
  }

public:
  static const int version = -1;

  /* New Random with fixed seed. */
  Random()
      : seed(3905348978240129619LL) {
  }

  /* Sets seed by command line. */
  void setSeed(int argc, char *argv[]) {
    Random p;

    seed = 3905348978240129619LL;
    for (int i = 1; i < argc; i++) {
      std::size_t le = std::strlen(argv[i]);
      for (std::size_t j = 0; j < le; j++)
        seed = seed * multiplier + (unsigned int)(argv[i][j]) + addend;
      seed += multiplier / addend;
    }

    seed = seed & mask;
  }

  /* Sets seed by given value. */
  void setSeed(long long _seed) {
    _seed = (_seed ^ multiplier) & mask;
    seed = _seed;
  }

  /* Random value in range [0, n-1]. */
  int next(int n) {
    if (n <= 0)
      __random_fail("Random::next(int n): n must be positive");

    if ((n & -n) == n)  // n is a power of 2
      return (int)((n * (long long)nextBits(31)) >> 31);

    const long long limit = INT_MAX / n * n;

    long long bits;
    do {
      bits = nextBits(31);
    } while (bits >= limit);

    return int(bits % n);
  }

  /* Random value in range [0, n-1]. */
  unsigned int next(unsigned int n) {
    if (n >= INT_MAX)
      __random_fail("Random::next(unsigned int n): n must be less INT_MAX");
    return (unsigned int)next(int(n));
  }

  /* Random value in range [0, n-1]. */
  long long next(long long n) {
    if (n <= 0)
      __random_fail("Random::next(long long n): n must be positive");

    const long long limit = __RANDOM_LONGLONG_MAX / n * n;

    long long bits;
    do {
      bits = nextBits(63);
    } while (bits >= limit);

    return bits % n;
  }

  /* Random value in range [0, n-1]. */
  unsigned long long next(unsigned long long n) {
    if (n >= (unsigned long long)(__RANDOM_LONGLONG_MAX))
      __random_fail("Random::next(unsigned long long n): n must be less LONGLONG_MAX");
    return (unsigned long long)next((long long)(n));
  }

  /* Random value in range [0, n-1]. */
  long next(long n) {
    return (long)next((long long)(n));
  }

  /* Random value in range [0, n-1]. */
  unsigned long next(unsigned long n) {
    if (n >= (unsigned long)(LONG_MAX))
      __random_fail("Random::next(unsigned long n): n must be less LONG_MAX");
    return (unsigned long)next((unsigned long long)(n));
  }

  /* Returns random value in range [from,to]. */
  int next(int from, int to) {
    return int(next((long long)to - from + 1) + from);
  }

  /* Returns random value in range [from,to]. */
  unsigned int next(unsigned int from, unsigned int to) {
    return (unsigned int)(next((long long)to - from + 1) + from);
  }

  /* Returns random value in range [from,to]. */
  long long next(long long from, long long to) {
    return next(to - from + 1) + from;
  }

  /* Returns random value in range [from,to]. */
  unsigned long long next(unsigned long long from, unsigned long long to) {
    if (from > to)
      __random_fail("Random::next(unsigned long long from, unsigned long long to): from can't not exceed to");
    return next(to - from + 1) + from;
  }

  /* Returns random value in range [from,to]. */
  long next(long from, long to) {
    return next(to - from + 1) + from;
  }

  /* Returns random value in range [from,to]. */
  unsigned long next(unsigned long from, unsigned long to) {
    if (from > to)
      __random_fail("Random::next(unsigned long from, unsigned long to): from can't not exceed to");
    return next(to - from + 1) + from;
  }

  /* Random double value in range [0, 1). */
  double next() {
    long long left = ((long long)(nextBits(26)) << 27);
    long long right = nextBits(27);
    return (double)(left + right) / (double)(1LL << 53);
  }

  /* Random double value in range [0, n). */
  double next(double n) {
    return n * next();
  }

  /* Random double value in range [from, to). */
  double next(double from, double to) {
    if (from > to)
      __random_fail("Random::next(double from, double to): from can't not exceed to");
    return next(to - from) + from;
  }

  /* Returns random element from container. */
  template <typename Container>
  typename Container::value_type any(const Container &c) {
    size_t size = c.size();
    if (size <= 0)
      __random_fail("Random::any(const Container& c): c.size() must be positive");
    return *(c.begin() + next(size));
  }

  /* Returns random element from iterator range. */
  template <typename Iter>
  typename Iter::value_type any(const Iter &begin, const Iter &end) {
    int size = int(end - begin);
    if (size <= 0)
      __random_fail("Random::any(const Iter& begin, const Iter& end): range must have positive length");
    return *(begin + next(size));
  }

  /*
   * Weighted next. If type == 0 than it is usual "next()".
   *
   * If type = 1, than it returns "max(next(), next())"
   * (the number of "max" functions equals to "type").
   *
   * If type < 0, than "max" function replaces with "min".
   */
  int wnext(int n, int type) {
    if (n <= 0)
      __random_fail("Random::wnext(int n, int type): n must be positive");

    if (abs(type) < Random::lim) {
      int result = next(n);

      for (int i = 0; i < +type; i++)
        result = __random_max(result, next(n));

      for (int i = 0; i < -type; i++)
        result = __random_min(result, next(n));

      return result;
    } else {
      double p;

      if (type > 0)
        p = std::pow(next() + 0.0, 1.0 / (type + 1));
      else
        p = 1 - std::pow(next() + 0.0, 1.0 / (-type + 1));

      return int(n * p);
    }
  }

  /* See wnext(int, int). It uses the same algorithms. */
  long long wnext(long long n, int type) {
    if (n <= 0)
      __random_fail("Random::wnext(long long n, int type): n must be positive");

    if (abs(type) < Random::lim) {
      long long result = next(n);

      for (int i = 0; i < +type; i++)
        result = __random_max(result, next(n));

      for (int i = 0; i < -type; i++)
        result = __random_min(result, next(n));

      return result;
    } else {
      double p;

      if (type > 0)
        p = std::pow(next() + 0.0, 1.0 / (type + 1));
      else
        p = std::pow(next() + 0.0, -type + 1);

      return __random_min(__random_max((long long)(double(n) * p), 0LL), n - 1LL);
    }
  }

  /* See wnext(int, int). It uses the same algorithms. */
  double wnext(int type) {
    if (abs(type) < Random::lim) {
      double result = next();

      for (int i = 0; i < +type; i++)
        result = __random_max(result, next());

      for (int i = 0; i < -type; i++)
        result = __random_min(result, next());

      return result;
    } else {
      double p;

      if (type > 0)
        p = std::pow(next() + 0.0, 1.0 / (type + 1));
      else
        p = std::pow(next() + 0.0, -type + 1);

      return p;
    }
  }

  /* See wnext(int, int). It uses the same algorithms. */
  double wnext(double n, int type) {
    if (n <= 0)
      __random_fail("Random::wnext(double n, int type): n must be positive");

    if (abs(type) < Random::lim) {
      double result = next();

      for (int i = 0; i < +type; i++)
        result = __random_max(result, next());

      for (int i = 0; i < -type; i++)
        result = __random_min(result, next());

      return n * result;
    } else {
      double p;

      if (type > 0)
        p = std::pow(next() + 0.0, 1.0 / (type + 1));
      else
        p = std::pow(next() + 0.0, -type + 1);

      return n * p;
    }
  }

  /* See wnext(int, int). It uses the same algorithms. */
  unsigned int wnext(unsigned int n, int type) {
    if (n >= INT_MAX)
      __random_fail("Random::wnext(unsigned int n, int type): n must be less INT_MAX");
    return (unsigned int)wnext(int(n), type);
  }

  /* See wnext(int, int). It uses the same algorithms. */
  unsigned long long wnext(unsigned long long n, int type) {
    if (n >= (unsigned long long)(__RANDOM_LONGLONG_MAX))
      __random_fail("Random::wnext(unsigned long long n, int type): n must be less LONGLONG_MAX");

    return (unsigned long long)wnext((long long)(n), type);
  }

  /* See wnext(int, int). It uses the same algorithms. */
  long wnext(long n, int type) {
    return (long)wnext((long long)(n), type);
  }

  /* See wnext(int, int). It uses the same algorithms. */
  unsigned long wnext(unsigned long n, int type) {
    if (n >= (unsigned long)(LONG_MAX))
      __random_fail("Random::wnext(unsigned long n, int type): n must be less LONG_MAX");

    return (unsigned long)wnext((unsigned long long)(n), type);
  }

  /* Returns weighted random value in range [from, to]. */
  int wnext(int from, int to, int type) {
    if (from > to)
      __random_fail("Random::wnext(int from, int to, int type): from can't not exceed to");
    return wnext(to - from + 1, type) + from;
  }

  /* Returns weighted random value in range [from, to]. */
  int wnext(unsigned int from, unsigned int to, int type) {
    if (from > to)
      __random_fail("Random::wnext(unsigned int from, unsigned int to, int type): from can't not exceed to");
    return int(wnext(to - from + 1, type) + from);
  }

  /* Returns weighted random value in range [from, to]. */
  long long wnext(long long from, long long to, int type) {
    if (from > to)
      __random_fail("Random::wnext(long long from, long long to, int type): from can't not exceed to");
    return wnext(to - from + 1, type) + from;
  }

  /* Returns weighted random value in range [from, to]. */
  unsigned long long wnext(unsigned long long from, unsigned long long to, int type) {
    if (from > to)
      __random_fail(
          "Random::wnext(unsigned long long from, unsigned long long to, int type): from can't not exceed to");
    return wnext(to - from + 1, type) + from;
  }

  /* Returns weighted random value in range [from, to]. */
  long wnext(long from, long to, int type) {
    if (from > to)
      __random_fail("Random::wnext(long from, long to, int type): from can't not exceed to");
    return wnext(to - from + 1, type) + from;
  }

  /* Returns weighted random value in range [from, to]. */
  unsigned long wnext(unsigned long from, unsigned long to, int type) {
    if (from > to)
      __random_fail("Random::wnext(unsigned long from, unsigned long to, int type): from can't not exceed to");
    return wnext(to - from + 1, type) + from;
  }

  /* Returns weighted random double value in range [from, to). */
  double wnext(double from, double to, int type) {
    if (from > to)
      __random_fail("Random::wnext(double from, double to, int type): from can't not exceed to");
    return wnext(to - from, type) + from;
  }

  /* Returns weighted random element from container. */
  template <typename Container>
  typename Container::value_type wany(const Container &c, int type) {
    size_t size = c.size();
    if (size <= 0)
      __random_fail("Random::wany(const Container& c, int type): c.size() must be positive");
    return *(c.begin() + wnext(size, type));
  }

  /* Returns weighted random element from iterator range. */
  template <typename Iter>
  typename Iter::value_type wany(const Iter &begin, const Iter &end, int type) {
    int size = int(end - begin);
    if (size <= 0)
      __random_fail(
          "Random::any(const Iter& begin, const Iter& end, int type): range must have positive length");
    return *(begin + wnext(size, type));
  }

  /* Returns random permutation of the given size (values are between `first` and `first`+size-1)*/
  template <typename T, typename E>
  std::vector<E> perm(T size, E first) {
    if (size <= 0)
      __random_fail("Random::perm(T size, E first = 0): size must be positive");
    std::vector<E> p(size);
    E current = first;
    for (T i = 0; i < size; i++)
      p[i] = current++;
    if (size > 1)
      for (T i = 1; i < size; i++)
        std::swap(p[i], p[next(i + 1)]);
    return p;
  }

  /* Returns random permutation of the given size (values are between 0 and size-1)*/
  template <typename T>
  std::vector<T> perm(T size) {
    return perm(size, T(0));
  }

  /* Returns `size` unordered (unsorted) distinct numbers between `from` and `to`. */
  template <typename T>
  std::vector<T> distinct(int size, T from, T to) {
    std::vector<T> result;
    if (size == 0)
      return result;

    if (from > to)
      __random_fail("Random::distinct expected from <= to");

    if (size < 0)
      __random_fail("Random::distinct expected size >= 0");

    uint64_t n = to - from + 1;
    if (uint64_t(size) > n)
      __random_fail("Random::distinct expected size <= to - from + 1");

    double expected = 0.0;
    for (int i = 1; i <= size; i++)
      expected += double(n) / double(n - i + 1);

    if (expected < double(n)) {
      std::set<T> vals;
      while (int(vals.size()) < size) {
        T x = T(next(from, to));
        if (vals.insert(x).second)
          result.push_back(x);
      }
    } else {
      if (n > 1000000000)
        __random_fail("Random::distinct here expected to - from + 1 <= 1000000000");
      std::vector<T> p(perm(int(n), from));
      result.insert(result.end(), p.begin(), p.begin() + size);
    }

    return result;
  }

  /* Returns `size` unordered (unsorted) distinct numbers between `0` and `upper`-1. */
  template <typename T>
  std::vector<T> distinct(int size, T upper) {
    if (size < 0)
      __random_fail("Random::distinct expected size >= 0");
    if (size == 0)
      return std::vector<T>();

    if (upper <= 0)
      __random_fail("Random::distinct expected upper > 0");
    if (size > upper)
      __random_fail("Random::distinct expected size <= upper");

    return distinct(size, T(0), upper - 1);
  }

  /* Returns random (unsorted) partition which is a representation of sum as a sum of integers not less than min_part. */
  template <typename T>
  std::vector<T> partition(int size, T sum, T min_part) {
    if (size < 0)
      __random_fail("Random::partition: size < 0");
    if (size == 0 && sum != 0)
      __random_fail("Random::partition: size == 0 && sum != 0");
    if (min_part * size > sum)
      __random_fail("Random::partition: min_part * size > sum");
    if (size == 0 && sum == 0)
      return std::vector<T>();

    T sum_ = sum;
    sum -= min_part * size;

    std::vector<T> septums(size);
    std::vector<T> d = distinct(size - 1, T(1), T(sum + size - 1));
    for (int i = 0; i + 1 < size; i++)
      septums[i + 1] = d[i];
    sort(septums.begin(), septums.end());

    std::vector<T> result(size);
    for (int i = 0; i + 1 < size; i++)
      result[i] = septums[i + 1] - septums[i] - 1;
    result[size - 1] = sum + size - 1 - septums.back();

    for (std::size_t i = 0; i < result.size(); i++)
      result[i] += min_part;

    T result_sum = 0;
    for (std::size_t i = 0; i < result.size(); i++)
      result_sum += result[i];
    if (result_sum != sum_)
      __random_fail("Random::partition: partition sum is expected to be the given sum");

    if (*std::min_element(result.begin(), result.end()) < min_part)
      __random_fail("Random::partition: partition min is expected to be no less than the given min_part");

    if (int(result.size()) != size || result.size() != (size_t)size)
      __random_fail("Random::partition: partition size is expected to be equal to the given size");

    return result;
  }

  /* Returns random (unsorted) partition which is a representation of sum as a sum of positive integers. */
  template <typename T>
  std::vector<T> partition(int size, T sum) {
    return partition(size, sum, T(1));
  }
};

#endif
