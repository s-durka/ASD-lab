#include <iostream>
#include <set>

int main() {
    std::ios_base::sync_with_stdio(false);

    int n, m;
    std::cin >> n;
    std::cin >> m;

    std::set<int> first;
    std::set<int> second;

    for (int i = 0; i < m; i++) {
        int from, to;
        char sign;
        std::cin >> sign;
        std::cin >> from;
        std::cin >> to;

        if (sign == '+') {
            if (from == 1) {
                first.insert(to);
            }
            else {
                second.insert(from);
            }
        }
        else if (sign == '-') {
            if (from == 1) {
                first.erase(to);
            }
            else {
                second.erase(from);
            }
        }
        int max_left = 0;
        int min_right = n+1;
        if (!first.empty())
            max_left = *first.rbegin();
        if (!second.empty()) {
            min_right = *second.begin();
        }
        if (min_right <= max_left)
            std::cout << 0 << "\n";
        else {
            std::cout << min_right - max_left - 1;
            std::cout << "\n";
        }
    }
    return 0;
}