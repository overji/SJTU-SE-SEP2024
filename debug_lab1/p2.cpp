#include <iostream>
#include <vector>

int binary_search(const std::vector<int> &arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left < right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    if (!arr.empty() && arr[left] == target) {
        return left;
    }

    return -1;
}

int main() {
    std::vector<int> arr;
    int n, element, target;

    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::cin >> element;
        arr.push_back(element);
    }

    std::cin >> target;

    int result = binary_search(arr, target);
    std::cout << result << std::endl;

    return 0;
}