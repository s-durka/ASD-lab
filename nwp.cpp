#include <iostream>
#include <climits>

/*
 * longest common almost increasing subsequence
 * given: array A[n], B[m] of integer > 0 (!), integer C
 * return k = length of longest common ("almost increasing") subsequence c_1,...,c_k of A[] and B[] such that
 * for all i<j c_i <= c_j + C
 * return 20 if k > 20
 */

int my_min(int a, int b) {
    if (a == 0)
        return b;
    if (b == 0)
        return a;
    return std::min(a, b);
}

int longest_common_sequence_c_increasing(int a[], int n, int b[], int m, int c) {
    int t[m][21]; // t[j][l] == max value of longest common almost increasing subsequence of length l, ending at b[j]
    int best_max_so_far[21] = {0};
    int temp[21] = {0};
    for (int i = 0; i < m; i++) {
        for (int l = 0; l <= 20; l++) {
            t[i][l] = 0; //0 znaczy że nie ma takiego subsequence kończącego się na b[l], a x > 0 znaczy że max = x
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j1 = 0; j1 < 21; j1++) // czyszczenie tablicy
            best_max_so_far[j1] = 0;

        for (int j = 0; j < m; j++) {
            std::copy(std::begin(t[j]), std::end(t[j]), std::begin(temp)); // temp = kopia t[j]

            // smallest max so far trzyma najmniejszy z maxów ciągów które kończą się przed b[j]
            // przechodzimy przez kolumnę odp. b[j], updatujemy best_max_so_far i updatujemy j-tą tablicę w shorter_seq_max:

            if (b[j] == a[i]) {
                for (int l = 1; l <= 20; l++) {
                    int shorter_seq_max = best_max_so_far[l - 1];
                    if (l == 1) {
                        t[j][l] = b[j];
                    }
                    else if (shorter_seq_max <= b[j] + c && shorter_seq_max != 0) {
                        int max_of_extended_seq = std::max(shorter_seq_max, b[j]);
                        t[j][l] = my_min(max_of_extended_seq, t[j][l]); // max najlepszego z dwóch ciągów dł. l: tego, co już tu wcześniej było, i wyniku o 1 krótszego wydłużonego o b[j]
                    }
                }
            }

            for (int l = 1; l <= 20; l++) {
                if (temp[l] != 0 && (temp[l] < best_max_so_far[l] || best_max_so_far[l] == 0)) {
                    // chemy trzymać to co było przed zmienieniem na takie, że matchujemy b[j] z a[i]
                    // bo wtedy jak natrafimy znowu na match w b[] później, to chcemy updatować tak jakbyśmy tutaj nie matchowali
                    best_max_so_far[l] = temp[l];
                }
            }
        }
    }

//    for (int l = 1; l <= 20; l++) {
//        for (int j = 0; j < m; j++) {
//            std:: cout << t[j][l] << " ";
//        }
//        std::cout << '\n';
//    }

    for (int l = 20; l > 0; l--) {
        for (int j = m-1; j >= 0; j--) {
            if (t[j][l] != 0) {
                //std:: cout << t[j][l];
                return l;
            }
        }
    }
    return 0;
}


int main() {
    //std::cout << "Hello, World!" << std::endl;

    int n, m, c;
    std::cin >> n;
    std::cin >> m;
    std::cin >> c;
    int a[n];
    int b[m];
    for (int i = 0; i < n; i++)
        std::cin >> a[i];
    for (int j = 0; j < m; j++) {
        std::cin >> b[j];
    }

//    int a[4] = {3,2,1,4};
//    int b[4] = {2,3,1,4};
//    int c = 1;
    int l = longest_common_sequence_c_increasing(a, n, b, m, c);
    std::cout << l;
    return 0;
}