#include <iostream>
#include <algorithm>
#include <limits.h>
#include <string>

int smallestGap(std::string matrix, int n) {
    // zwraca najmniejszą odległość pomiędzy dwoma różnymi elementami w tablicy
    // jeśli nie ma dwóch różnych elementów w tablicy, zwraca INT_MAX;
    int minGap = INT_MAX;
    if (n == 0) return 0;
    int i = 0;
    while (i < n && matrix[i] == '*')
        i++;
    if (i == n) return INT_MAX;
    char prev = matrix[i];
    i++;
    int currGap = 0;
     while (i < n) {
         if (matrix[i] == '*')
             currGap++;
         else if (matrix[i] != prev){
            minGap = std::min(currGap, minGap);
            prev = matrix[i];
            currGap = 0;
        }
         else // matrix[i] != '*' i matrix[i] == prev
             currGap = 0;
        i++;
    }
     return minGap;
}



int main() {
    std::string str;
    getline (std::cin, str);
    int gap = smallestGap(str, str.length());
    if (gap == INT_MAX){
        std::cout << 1;
    }
    else {
        std::cout << str.length() - gap;
    }
    return 0;
}