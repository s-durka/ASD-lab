#include <iostream>
#include <algorithm>

void fillSuffixSumArr(int n, int prices[], long long int suffixSums[]) {
    long long int sum = 0;
    for (int i = n - 1; i >= 0; i--) {
        sum += prices[i];
        suffixSums[i] = sum;
    }
}

void fillPrefixArrays(int n, int prices[], int maxOddInPrefix[], int maxEvenInPrefix[]) {
    // wypełniamy 2 tablice maksów przed:
    int i = 0;
    int maxOdd = -1;
    int maxEven = -1;
    while (i < n) {
        if (prices[i] % 2 == 0)
            // prices[i] > maxEven bo tablica jest rosnąca
            maxEven = prices[i];
        else {
            // prices[i] > maxOdd bo tablica jest rosnąca
            maxOdd = prices[i];

        }
        maxEvenInPrefix[i] = maxEven;
        maxOddInPrefix[i] = maxOdd;
        i++;
    }
}

void fillSuffixArrays(int n, int prices[], int minOddInSuffix[], int minEvenInSuffix[]) {
    // wypełniamy 2 tablice minimów po prawej (i włącznie z i-tym):
    int i = n -1;
    int minOdd = -1; // znacznik
    int minEven = -1; // znacznik
    while (i >= 0) {
        if (prices[i] % 2 == 0)
            // prices[i] < minEven (jeśli minEven != -1) bo tablica jest rosnąca
            minEven = prices[i];
        else {
            // prices[i] < minOdd (jeśli minOdd != -1) bo tablica jest rosnąca
            minOdd = prices[i];
        }
        minOddInSuffix[i] = minOdd;
        minEvenInSuffix[i] = minEven;
        i--;
    }
}

long long int calculateMaxOddSum(int k, int n, long long int suffixSums[], int maxOddInPrefix[], int maxEvenInPrefix[], int minOddInSuffix[], int minEvenInSuffix[]) {
    // liczy największą nieparzystą sumę sufiksową k elementów
    // k - liczba produktów

    long long int suffixSum = suffixSums[n - k];
    if (suffixSum % 2 == 1)
        return suffixSum;
    // else -- suma jest parzysta
    long long int s1;
    long long int s2;
    if (minEvenInSuffix[n - k] == -1 || n - k - 1 < 0 || maxOddInPrefix[n - k - 1] == -1)
        s1 = -1;
    else
        s1 = suffixSum - minEvenInSuffix[n - k] + maxOddInPrefix[n - k - 1];

    if (minOddInSuffix[n - k] == -1 || n - k - 1 < 0 || maxEvenInPrefix[n - k - 1] == -1)
        s2 = -1;
    else
        s2 = suffixSum - minOddInSuffix[n - k] + maxEvenInPrefix[n - k - 1];

    if (s1 > s2) return s1;
    return s2;
}

int main() {
    // std::cout << "Hello, World!" << std::endl;

    int n;
    std::cin >> n; // 1 linijka
    int prices[n];
    for (int i = 0; i < n; i++) // 2 linijka
        std::cin >> prices[i];
    int numberOfDays;
    std::cin >> numberOfDays; // 3 linijka

    long long int suffixSums[n];
    int maxOddInPrefix[n];
    int maxEvenInPrefix[n];
    int minOddInSuffix[n];
    int minEvenInSuffix[n];

    fillSuffixSumArr(n, prices, suffixSums);
    fillPrefixArrays(n, prices, maxOddInPrefix, maxEvenInPrefix);
    fillSuffixArrays(n, prices, minOddInSuffix, minEvenInSuffix);

    for (int i = 0; i < numberOfDays; i++) {
        int nrOfProducts; // liczba produktów tego dnia
        std::cin >> nrOfProducts;
        printf("%lld\n", calculateMaxOddSum(nrOfProducts, n, suffixSums, maxOddInPrefix, maxEvenInPrefix, minOddInSuffix, minEvenInSuffix));
    }

    return 0;
}