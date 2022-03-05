#include <iostream>

int count_starting_sequences(int seq[], int n) {

    int l[n][n]; // tablica zliczajaca na ile sposobow mozna stworzyc ciag przez dodanie z lewej
    int r[n][n]; // // tablica zliczajaca na ile sposobow mozna stworzyc ciag przez dodanie z lewej

    for (int i = 0; i < n; i++) {
        l[0][i] = 1;
        r[0][i] = 0;
    }

    for (int length = 1; length < n; length++) { // dlugosc ciagu - 1 (bo od zera) -- ciagi o dl. 1 - l[0], ciagi o dl. 2 - tab[1]..
        for (int i = 0; i < n - length; i++) {
            l[length][i] = 0;
            if (seq[i] < seq[i+1]) { // ostatni dodany z lewej, teraz dodajemy z lewej
                l[length][i] += l[length - 1][i+1];
            }
            if (seq[i] < seq[i+length]) { // ostatni dodany z prawej, teraz dodajemy z lewej
                l[length][i] += r[length - 1][i+1];
            }
            l[length][i] = l[length][i] % 1000000000;

            r[length][i] = 0;
            if (seq[i+length] > seq[i]) { // ostatni dodany z lewej, teraz dodajemy z prawej
                r[length][i] += l[length - 1][i];
            }
            if (seq[i+length] > seq[i+length - 1]) { // ostatni dodany z prawej, teraz dodajemy tez z prawej
                r[length][i] += r[length - 1][i];
            }
            r[length][i] = r[length][i] % 1000000000;
        }
    }
    return (r[n -1][0] + l[n -1][0]) % 1000000000;
}


int main() {
    int n;
    std::cin >> n;
    int seq[n];
    for (int i = 0; i < n; i++)
        std::cin >> seq[i];
    int ans = count_starting_sequences(seq, n);
    std::cout << ans << "\n";


    return 0;
}