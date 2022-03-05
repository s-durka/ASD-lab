#include <iostream>
using std::cout;
using std::cin;


void colorRec(int segmentTree[], int i, int from, int to, int lowerBound, int upperBound, char color);
void colorSegment(int segmentTree[], int nrOfLeaves, int from, int to, char color) {
    int lowerBound = 1; int upperBound = nrOfLeaves; // przedział w korzeniu
    colorRec(segmentTree, 0, from, to, lowerBound, upperBound, color);
}

/*
 * lowerBound, upperBound - definiują przedział w akt. węźle
 * from, to - przedział do kolorowania
 * color - 0 lub 1 - biały albo czarny
 * trzymamy w węźle liczbę białych
 */
void colorRec(int segmentTree[], int i, int from, int to, int lowerBound, int upperBound, char color) {
    int bialePodI = segmentTree[i];
    if (from == lowerBound && to == upperBound) {
        if (color == 'B') {
            segmentTree[i] = upperBound - lowerBound + 1;
        } else {
            segmentTree[i] = 0;
        }
    }
    else {
        if (color == 'B' && segmentTree[i] == upperBound - lowerBound + 1)
            return;
        if (color == 'C' && segmentTree[i] == 0)
            return;

        int leftChildLowerBound = lowerBound;
        int leftChildUpperBound = lowerBound + (upperBound - lowerBound)/2; //  UPPER BOUND PRAWEGO SYNA
        int rightChildLowerBound = leftChildUpperBound + 1;
        int rightChildUpperBound = upperBound;
        int li = 2 * i + 1;
        int ri = 2 * i + 2;

        /* idąc w dół, jeśli węzeł był wcześniej pomalowany cały na biało albo czarno,
         * chcemy -- kolorując przedział w *tylko* lewym synu, przekazać, to że psyn jest cały czarny/biały
         * i vice versa
         */
        if (segmentTree[i] == 0) { // całe czarne
            segmentTree[li] = 0;
            segmentTree[ri] = 0;
        } else if (segmentTree[i] == upperBound - lowerBound + 1) { // całe białe
            segmentTree[li] = leftChildUpperBound - leftChildLowerBound + 1; // zapisujemy info, że lsyn dalej biały
            segmentTree[ri] = rightChildUpperBound - rightChildLowerBound + 1; // zapisujemy info, że lsyn dalej biały

        }

        if (from > leftChildUpperBound) { // kolorujemy tylko prawe poddrzewo
            colorRec(segmentTree, ri, from, to, rightChildLowerBound, rightChildUpperBound, color);
        } else if (to < rightChildLowerBound) { // malujemy tylko lewe poddrzewo
            colorRec(segmentTree, li, from, to, leftChildLowerBound, leftChildUpperBound, color);
        } else {
            colorRec(segmentTree, li, from, leftChildUpperBound, leftChildLowerBound, leftChildUpperBound, color);
            colorRec(segmentTree, ri, rightChildLowerBound, to, rightChildLowerBound, rightChildUpperBound, color);
        }
        // backtracking :
        int whiteInL =  segmentTree[li];
        int whiteInR = segmentTree[ri];
        int whiteBacktracked =  segmentTree[li] + segmentTree[ri];
        segmentTree[i] = segmentTree[li] + segmentTree[ri];
    }

}

int main() {
    /*
     * tworzymy pełne drzewo binarne przedziałowe w tablicy
     * mieszczące wszystkie przedziały od 1 do n:
     * h = sufit z log n
     * nrOfLeaves = liczba węzłów = 2^(h-1) - 1
     */
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    int n, m;
    cin >> n;
    cin >> m;
    int h = 0;
    int nrOfLeaves = 1;
    while (nrOfLeaves < n) {
        nrOfLeaves *= 2;
        h++;
    }
    int nrOfNodes = nrOfLeaves * 2 - 1;

    int segmentTree[nrOfNodes];
    segmentTree[0] = 0; // ulica cała czarna najpierw
    for (int i = 0; i < nrOfNodes; i++) {
        segmentTree[i] = 0;
    }

    int from, to;
    char color;
    for (int i = 0; i < m; i++) {
        cin >> from;
        cin >> to;
        cin >> color;
        colorSegment(segmentTree, nrOfLeaves, from, to, color);
        cout << segmentTree[0] << "\n";
    }
    return 0;
}