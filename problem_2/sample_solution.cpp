#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <thread>
#include <future>

void merge(std::vector<size_t>& array, size_t start, size_t mid, size_t end) {
    size_t left_size = mid - start + 1;
    size_t right_size = end - mid;

    std::vector<size_t> left(left_size);
    std::vector<size_t> right(right_size);

    for (size_t i = 0; i < left_size; ++i)
        left[i] = array[start + i];
    for (size_t i = 0; i < right_size; ++i)
        right[i] = array[mid + 1 + i];

    size_t i = 0, j = 0, k = start;
    while (i < left_size && j < right_size) {
        if (left[i] <= right[j]) {
            array[k++] = left[i++];
        } else {
            array[k++] = right[j++];
        }
    }
    while (i < left_size) {
        array[k++] = left[i++];
    }
    while (j < right_size) {
        array[k++] = right[j++];
    }
}

void parallel_merge_sort(std::vector<size_t>& array, size_t start, size_t end, size_t depth = 0) {
    if (start >= end) {
        return;
    }

    size_t mid = start + (end - start) / 2;

    if (depth < 4) {
        std::thread left_thread(parallel_merge_sort, std::ref(array), start, mid, depth + 1);
        std::thread right_thread(parallel_merge_sort, std::ref(array), mid + 1, end, depth + 1);

        left_thread.join();
        right_thread.join();
    } else {
        parallel_merge_sort(array, start, mid, depth + 1);
        parallel_merge_sort(array, mid + 1, end, depth + 1);
    }

    merge(array, start, mid, end);
}

std::vector<size_t> read_array() {
    size_t length, a, b, p;
    std::cin >> length >> a >> b >> p;
    std::vector<size_t> result(length);
    result[0] = a % p;
    for (size_t i = 1; i < result.size(); ++i) {
        result[i] = (result[i - 1] * a + b) % p;
    }
    return result;
}

int main() {
    auto array = read_array();

    parallel_merge_sort(array, 0, array.size() - 1);

    size_t k;
    std::cin >> k;
    for (size_t i = k - 1; i < array.size(); i += k) {
        std::cout << array[i] << ' ';
    }
    std::cout << "\n";

    return 0;
}
