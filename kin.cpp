#include <iostream>
#define DZIESIEC_DO_9 1000000000;

void count_next_inversions(int inversions[], int arr[], int n, int segment_tree[], int m) {
    for (int s = 0; s < m; s++)
        segment_tree[s] = 0;

    for (int i = 0; i < n; i++) {
        int value = arr[i];
        int j = 0;
        int l = 1; int r = n;
        int inv_count = 0; // licznik k-1 inwersji kończących się na liczbach większych od value (aktualizowany przechodząc przez drzewo segmentów)
        /*
         * schodzimy ścieżką do liścia trzymającego info o value,
         * od razu aktualizując ścieżkę dla nast iteracji
         * oraz zliczając k-1 inwersje dla liczb większych od value, które pojawiły się do tej pory w arr[]
         */
        while (l < r) { // !(l == value && p == value) to samo co l!=val or p!=val
            segment_tree[j] = (segment_tree[j] + inversions[i]) % DZIESIEC_DO_9; // od razu aktualizujemy ścieżkę do liścia dla następnej iteracji
            int mid = (l+r)/2;
            if (value > mid) {
                // idziemy do prawego poddrzewa
                l = mid + 1;
                j = 2*j + 2; //psyn
            }
            else {
                //idziemy do lewego syna
                r = mid;
                j = 2*j + 1; // lsyn
                if(j + 1 < m){
                    inv_count += segment_tree[j+1]; // zwiększamy licznik k-1 inwersji kończących się na liczbach większych od value
                    inv_count = inv_count % DZIESIEC_DO_9;
                }
            }
        }
        segment_tree[j] = (segment_tree[j] + inversions[i]) % DZIESIEC_DO_9;
        inversions[i] = inv_count; // teraz już w tablicy jest liczba k-inwersji kończących się na value, a nie (k-1)-inwersji tak jak poprzednio
    }

}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n; // dł. ciągu
    int k; // k-inwersje
    std::cin >> n;
    std::cin >> k;
    int sequence[n];
    for (int i = 0; i < n; i++) {
        std::cin >> sequence[i];
    }
    int inversions[n];
    for (int i = 0; i < n; i++)
        inversions[i] = 1;
    int m = 1;
    while (m < n) {
        m *= 2;
    }
    int size = 2 * m - 1;
    int segment_tree[size];

    for (int c = 1; c < k; c++) {
        //std::cout << c << '\n';
        for (int i = 0; i < n; i ++) {
            int inv = inversions[i];
            //std::cout << inv << ' ';
        }
        //std::cout << '\n';

        count_next_inversions(inversions, sequence, n, segment_tree, size);
    }
    int sum = 0;
    for (int i = 0; i < n; i ++){
        int inv = inversions[i];
        //std::cout << inv << ' ';
        sum += inv;
        sum = sum % DZIESIEC_DO_9;
    }
    std::cout << sum << '\n';
    return 0;
}