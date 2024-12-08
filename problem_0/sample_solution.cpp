#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <thread>
#include <mutex>
#include <cmath>

std::mutex mtx;

std::istream& operator>>(std::istream& in, __int128& value) {
    std::string s;
    in >> s;
    value = 0;
    bool negative = false;
    size_t i = 0;
    if (s[0] == '-') {
        negative = true;
        i = 1;
    }
    for (; i < s.size(); ++i) {
        value = value * 10 + (s[i] - '0');
    }
    if (negative) value = -value;
    return in;
}

std::ostream& operator<<(std::ostream& out, __int128 value) {
    if (value == 0) {
        out << '0';
        return out;
    }
    std::string s;
    bool negative = false;
    if (value < 0) {
        negative = true;
        value = -value;
    }
    while (value > 0) {
        s += '0' + static_cast<int>(value % 10);
        value /= 10;
    }
    if (negative) s += '-';
    std::reverse(s.begin(), s.end());
    out << s;
    return out;
}

void factorize(__int128& n, __int128 start, __int128 end, std::vector<__int128>& factors) {
    for (__int128 p = start; p <= end; ++p) {
        while (n % p == 0) {
            std::lock_guard<std::mutex> lock(mtx);
            factors.push_back(p);
            n /= p;
        }
    }
}

int main() {
    __int128 n;
    std::cin >> n;
    if (n <= 1) {
        return 0;
    }

    std::vector<__int128> factors;
    const int num_threads = 10;
    std::vector<std::thread> threads;

    __int128 limit = static_cast<__int128>(std::sqrt(static_cast<double>(n))) + 1;
    __int128 range = (limit - 2 + num_threads - 1) / num_threads;

    __int128 original_n = n;

    for (int i = 0; i < num_threads; ++i) {
        __int128 start = 2 + i * range;
        __int128 end = (i == num_threads - 1) ? limit : std::min(start + range - 1, limit);
        threads.emplace_back(factorize, std::ref(n), start, end, std::ref(factors));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    if (n > 1) {
        std::lock_guard<std::mutex> lock(mtx);
        factors.push_back(n);
    }

     __int128 product = 1;
    for (const auto& factor : factors) {
        product *= factor;
    }

    if (product != original_n) {
        factors.clear();
        n = original_n;
        for (__int128 p = 2; p * p <= n; ++p) {
            while (n % p == 0) {
                factors.push_back(p);
                n /= p;
            }
        }
        if (n > 1) {
            factors.push_back(n);
        }
    }

    for (const auto& factor : factors) {
        std::cout << factor << ' ';
    }
    std::cout << '\n';

    return 0;
}