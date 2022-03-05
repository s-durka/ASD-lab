#include <iostream>
#include <vector>
using std::vector;
using std::cin;
using std::cout;

/*
 * x = WIERSZ
 * y = KOLUMNA
 * !!!!!!!!!!!!!
 */

// zmienne globalne żeby było szybciej i łatwiej:
int nrOfLeavesSmall;
int nrOfNodesSmall;
int hSmall;
int nrOfLeavesBig;
int nrOfNodesBig;
int hBig;

int left(int i) { return 2 * i + 1; }
int right(int i) {return 2 * i + 2; }

int my_min(int a, int b) {
    if (a == 0) return b;
    if (b == 0) return a;
    if (a < b) return a;
    return b;
}

int build_tree_y_rec(bool max, vector<int>& segTree, int i, int hNow, int h) {
    if (hNow == h)
        return segTree[i];
    if (max) {
        segTree[i] = std::max(build_tree_y_rec(max, segTree, left(i), hNow + 1, h),
                              build_tree_y_rec(max, segTree, right(i), hNow + 1, h));
    }
    else {
        segTree[i] = my_min(build_tree_y_rec(max, segTree, left(i), hNow + 1, h),
                              build_tree_y_rec(max, segTree, right(i), hNow + 1, h));
    }
    return segTree[i];
}

vector<int> merge_two_trees(bool max, vector<int> t1, vector<int> t2) {
    // same rozmiar wektorów t1 i t2
    vector<int> t3;
    if (max) {
        for (int i = 0; i < t1.size(); i++) {
            t3.push_back(std::max(t1[i], t2[i]));
        }
    }
    else {
        for (int i = 0; i < t1.size(); i++) {
            t3.push_back(my_min(t1[i], t2[i]));
        }
    }

    return t3;
}

vector<int> build_tree_x_rec(bool max, vector<int> *bigTree, int j, int hNow, int h) {
    if (hNow == h)
        return bigTree[j];
    bigTree[j] = merge_two_trees(max,build_tree_x_rec(max, bigTree, left(j), hNow + 1, h),
                                 build_tree_x_rec(max, bigTree, right(j), hNow + 1, h));
    return bigTree[j];
}

vector<int> build_tree_y(bool max, int m, vector<int> row, int nrOfNodes, int nrOfLeaves, int h) {
    vector<int> segTree (nrOfNodes, 0);
    int firstLeaf = nrOfNodes - nrOfLeaves; // bez + 1 bo zaczynamy od zera tablicę
    for (int i = 0; i < m; i++) {
        segTree[firstLeaf + i] = row[i];
    }

    build_tree_y_rec(max, segTree, 0, 0, h);
    return segTree;
}

void build_tree_x(bool max, vector<int> *bigTree, int m, vector<int> *a, int n) {
    int firstLeaf = nrOfNodesBig - nrOfLeavesBig; // bez + 1 bo zaczynamy od zera tablicę
    int i = 0;
    while (i < n) {
        bigTree[firstLeaf + i] = build_tree_y(max, m, a[i], nrOfNodesSmall, nrOfLeavesSmall, hSmall);
        i++;
    }
    int j = firstLeaf + i;
    while (j < nrOfNodesBig) { // dopełniamy zerowymi wektorami ostatni poziom drzewa
        bigTree[j] = vector<int> (nrOfNodesSmall, 0);
        j++;
    }
    build_tree_x_rec(max, bigTree, 0, 0, hBig);
}

int findInSmallTreeRec(bool max, vector<int>& smallTree, int i, int from, int to, int lowerB, int upperB) {
    // lowerB, upperB - w tym przedziale
    if (from == lowerB && to == upperB) {
            return smallTree[i];
    }
    int mid = lowerB + (upperB - lowerB)/2;
    int lowerL = lowerB;
    int upperL = mid;
    int lowerR = mid + 1;
    int upperR = upperB;

    if (from > upperL) { // tylko prawe poddrzewo
        return findInSmallTreeRec(max, smallTree, right(i), from, to, lowerR, upperR);
    } else if (to < lowerR) { // tylko lewe poddrzewo
        return findInSmallTreeRec(max, smallTree, left(i), from, to, lowerL, upperL);
    } else {
        int l = findInSmallTreeRec(max, smallTree, left(i), from, upperL, lowerL, upperL);
        int r = findInSmallTreeRec(max, smallTree, right(i), lowerR, to, lowerR, upperR);
        if (max) {
            return std::max(l, r);
        } else {
            return my_min(l, r);
        }
    }
}

int findInSmallTree(bool max, vector<int>& smallTree, int from, int to) {
    return findInSmallTreeRec(max, smallTree, 0, from, to, 0, nrOfLeavesSmall - 1);
}

int findInBigTreeRec(bool max, vector<int> *bigTree, int i, int fromX, int toX, int lowerBX, int upperBX, int fromSmall, int toSmall) {
    if (fromX == lowerBX && toX == upperBX) {
        return findInSmallTree(max, bigTree[i], fromSmall, toSmall);
    }
    int mid = lowerBX + (upperBX - lowerBX)/2;
    int lowerL = lowerBX;
    int upperL = mid;
    int lowerR = mid + 1;
    int upperR = upperBX;

    if (fromX > upperL) { // tylko prawe poddrzewo
        return findInBigTreeRec(max, bigTree, right(i), fromX, toX, lowerR, upperR, fromSmall, toSmall);
    } else if (toX < lowerR) { // tylko lewe poddrzewo
        return findInBigTreeRec(max, bigTree, left(i), fromX, toX, lowerL, upperL, fromSmall, toSmall);
    } else {
        int l = findInBigTreeRec(max, bigTree, left(i), fromX, upperL, lowerL, upperL, fromSmall, toSmall);
        int r = findInBigTreeRec(max, bigTree, right(i), lowerR, toX, lowerR, upperR, fromSmall, toSmall);
        if (max) {
            return std::max(l, r);
        } else {
            return my_min(l, r);
        }
    }

}

int findInBigTree(bool max, vector<int> *bigTree, int x1, int x2, int y1, int y2) {
    return findInBigTreeRec(max, bigTree, 0, x1, x2, 0, nrOfLeavesBig - 1, y1, y2);
}

int maxDiffInRectangle(vector<int> *bigTreeMax, vector<int> *bigTreeMin, int x1, int x2, int y1, int y2) {
    int maxV = findInBigTree(true, bigTreeMax, x1, x2, y1, y2);
    int minV = findInBigTree(false, bigTreeMin, x1, x2, y1, y2);
    return maxV - minV;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    int n, m, k;
    cin >> n;
    cin >> m;
    cin >> k;

    vector<int> field[n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int val;
            cin >> val;
            field[i].push_back(val);
        }
    }

    hSmall = 0;
    nrOfLeavesSmall = 1;
    while (nrOfLeavesSmall < m) {
        nrOfLeavesSmall *= 2;
        hSmall++;
    }
    nrOfNodesSmall = nrOfLeavesSmall * 2 - 1;

    hBig = 0;
    nrOfLeavesBig = 1;
    while (nrOfLeavesBig < n) {
        nrOfLeavesBig *= 2;
        hBig++;
    }
    nrOfNodesBig = nrOfLeavesBig * 2 - 1;

    vector<int> bigTreeMax[nrOfNodesBig];
    vector<int> bigTreeMin[nrOfNodesBig];
    build_tree_x(true, bigTreeMax, m, field, n); // True <=> buduj drzewo MAX
    build_tree_x(false, bigTreeMin, m, field, n); // True <=> buduj drzewo MAX


//    cout << "\nMax:\n";
//    for (int j = 0; j < nrOfNodesBig; j++) {
//        for (int i = 0; i < nrOfNodesSmall; i++) {
//            cout << bigTreeMax[j][i] << " ";
//        }
//        cout << '\n';
//    }
//
//    cout << "\nMin:\n";
//    for (int j = 0; j < nrOfNodesBig; j++) {
//        for (int i = 0; i < nrOfNodesSmall; i++) {
//            cout << bigTreeMin[j][i] << " ";
//        }
//        cout << '\n';
//    }

    int x1, x2, y1, y2;
    for (int i = 0; i < k; i++) {
        cin >> x1;
        cin >> y1;
        cin >> x2;
        cin >> y2;
        int diff = maxDiffInRectangle(bigTreeMax, bigTreeMin, x1, x2, y1, y2);
        cout << diff << "\n";
    }

    return 0;
}