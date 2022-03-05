#include <iostream>
#include <vector>
using std::cin;
using std::cout;
using std::vector;

struct path_t {
    int length;
    int second_node;
    int common_root;
};
int get_bits_nr(int n) { //ile bitów potrzeba do zapisania liczby n
    int i = 0;
    while(n > 0){
        n /= 2;
        i++;
    }
    return i;
}

int set_height_rec(int i, int left_child[], int right_child[], int height[][2]) {
    if (i != - 1) {
        int lh = set_height_rec(left_child[i], left_child, right_child, height);
        int rh = set_height_rec(right_child[i], left_child, right_child, height);
        int l_leaf, r_leaf;
        l_leaf = height[left_child[i]][1];
        r_leaf = height[right_child[i]][1];

        if (left_child[i] == -1 && right_child[i] == -1) {
            height[i][0] = 0;
            height[i][1] = i;
        }
        else {
            if (lh > rh) {
                height[i][0] = lh;
                height[i][1] = l_leaf;
            }
            else {
                height[i][0] = rh;
                height[i][1] = r_leaf;
            }
            height[i][0]++; // dodajemy jeden bo wysokość od syna + 1 za połączenie od węzła do syna
        }
        return height[i][0];

    }
    else {
        return -1; // jeśli węzeł będzie liściem to zwróci 0
    }
}

void set_heights(int left_child[], int right_child[], int height[][2]) {
    // 1 == root node
    set_height_rec(1, left_child, right_child, height);
}

void set_longest_path(path_t parents_longest_path_up, int node, path_t longest_path[], int height[][2], int left_child[], int right_child[], int parent[], int n) {
    // "parents_longest_p_up" przekazywany w dół rekursji do synów
    if (node == -1)
        return;

    path_t my_longest_p_up;
    if (parent[node] == -1) {
        my_longest_p_up.length = 0;
        my_longest_p_up.second_node = node;
        my_longest_p_up.common_root = node;
    }
    else {
        int other_child;
        if (node == left_child[parent[node]]) { // jesli jest lewym synem
            other_child = right_child[parent[node]];
        }
        else { // jest psynem
            other_child = left_child[parent[node]];
        }
        int path_up1 = parents_longest_path_up.length + 1;
        int path_up2;
        if (other_child == -1)
            path_up2 = -1;
        else
            path_up2 = 2 + height[other_child][0];
        if (path_up1 > path_up2) {
            my_longest_p_up.length = path_up1;
            my_longest_p_up.second_node = parents_longest_path_up.second_node;
            my_longest_p_up.common_root = parents_longest_path_up.common_root;
        }
        else {
            my_longest_p_up.length = path_up2;
            my_longest_p_up.second_node = height[other_child][1];
            my_longest_p_up.common_root = parent[node];
        }
    }

    int path_down = height[node][0];
    if (my_longest_p_up.length > path_down) {
        longest_path[node].length = my_longest_p_up.length;
        longest_path[node].second_node = my_longest_p_up.second_node;
        longest_path[node].common_root = my_longest_p_up.common_root;
    }
    else {
        longest_path[node].length = path_down;
        longest_path[node].common_root = node;
        longest_path[node].second_node = height[node][1];
    }
    set_longest_path(my_longest_p_up, right_child[node], longest_path, height, left_child, right_child, parent, n);
    set_longest_path(my_longest_p_up, left_child[node], longest_path, height, left_child, right_child, parent, n);
}

void set_l_p(path_t longest_path[], int height[][2], int left_child[], int right_child[], int parent[], int n) {
    path_t nothing;
    nothing.second_node = -1;
    nothing.length = -1;
    nothing.common_root =1;
    set_longest_path(nothing, 1, longest_path, height, left_child, right_child, parent, n);
}

void set_depth(int i, int n, int d, int depth[], int left_child[], int right_child[]){
    if(i != -1){
        depth[i] = d;
        set_depth(left_child[i], n, d + 1, depth, left_child, right_child);
        set_depth(right_child[i], n, d + 1, depth, left_child, right_child);
    }
}

int find_node_k_up(int node, int k, vector<vector<int>>& binary_predecessors) {
    int i = 0;
    int curr_node = node;
    while (k > 0) {
        if (k % 2 == 1) {
          curr_node = binary_predecessors[i][curr_node]; // węzeł o 2^i wyżej od aktualnego
        }
        k = k/2;
        i++;
    }
    return curr_node;
}

int find_node_d_away(int node, int d, path_t longest_path[], int depth[], vector<vector<int>>& binary_predecessors) {
    path_t longest_p = longest_path[node];
    if (longest_p.length < d)
        return -1;
    else if (longest_p.length == d)
        return longest_p.second_node;
    else {
        int k;
        int root_to_a = depth[node] - depth[longest_p.common_root]; // dystans od wsp. korzenia do node
        int root_to_n2 = depth[longest_p.second_node] - depth[longest_p.common_root]; // // dystans od wsp. korzenia do drugiego node (liścia)
        if (longest_p.common_root == node) {
            k = root_to_n2 - d; // dystans od korzenia do drugiego node minus d
            return find_node_k_up(longest_p.second_node, k, binary_predecessors);
        }
        else if (longest_p.common_root == longest_p.second_node) {
            k = root_to_a - d;
            return find_node_k_up(node, k, binary_predecessors);
        }
        else {
            if (d < root_to_a) { // szukany węzeł na scieżce od root do a
                k = d;
                return find_node_k_up(node, k, binary_predecessors);
            }
            else { // szukany wezel na sciezce od root do n2
                d -= root_to_a;
                k = root_to_n2 - d;
                return find_node_k_up(longest_p.second_node, k, binary_predecessors);
            }
        }
    }
}

int main() {
//    std::cout << "Hello, World!" << std::endl;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int n = -10;
    cin >> n;

    int parents[n+1];
    int left_child[n+1];
    int right_child[n+1];
    int depth[n+1];

    int l, p;
    for (int i = 1; i <= n; i++) {
        cin >> l;
        cin >> p;
        left_child[i] = l;
        right_child[i] = p;
        // jeśli == -1 to znaczy ze nie ma liscia
        if (l != -1) parents[l] = i;
        if (p != -1) parents[p] = i;
    }
    parents[1] = -1;

    /*
    * lpu[i].path == dł najdłuższej ścieżki od węzła i idąc w górę, i potem do jakiegoś innego węzła
    * lpu[i].second_node == numer węzła v takiegp, że ścieżka od i do v jest najdłuższa
    * lpu[i].common_root == wspólny korzeń v oraz i
    */
    path_t longest_path[n+1];

    /* height[i][0] == dł najdłuższej ścieżki do liścia od danego węzła
     * height[i][1] == numer węzła (liścia) o najniższej głębokości w poddrzewie węzła i */
    int height[n+1][2];

    // najpierw wczytaj input do tablic

    set_heights(left_child, right_child, height);
    set_l_p(longest_path, height, left_child, right_child, parents, n);

    vector<vector<int>> binary_predecessors;
    vector<int> row1;
    row1.push_back(0); // tak samo jak w tablicach, zerowy element jest niewazny -- dummy
    for (int i =1; i <=n; i++)
        row1.push_back(parents[i]);

    int jumps = get_bits_nr(n - 1); //1 + (int)floor(log(n));
    binary_predecessors.push_back(row1);
    // dynamiczne uzupełnianie wektorów 2d:

    for(int j = 1; j < jumps; j++){
        vector<int> v;
        v.push_back(0);
        for(int i = 1; i <= n; i++){
            int ancestor_1, ancestor_2;
            ancestor_1 = binary_predecessors[j-1][i]; // numer wierzchołka znajdującego się 2^(j-1) krawędzi nad wierzchołkiem i
            if(ancestor_1 != -1)
                ancestor_2 = binary_predecessors[j-1][ancestor_1]; // numer wierzchołka znajdującego się 2^(j-1) krawędzi nad ancestor1, czyli 2*2^(j-1) = 2^j krawędzi nad wierzchołkiem i
            else
                ancestor_2 = -1;
            v.push_back(ancestor_2);
            //jumps[j][i] = ancestor_2;
        }
        binary_predecessors.push_back(v);
    }

    set_depth(1, n, 0, depth, left_child, right_child);

    // DEBUG
//    for (int i = 1; i <= n; i++) {
//        cout << parents[i] << " ";
//    }
//    cout << '\n';
//    for (int i = 1; i <= n; i++) {
//        cout << i << ": " << left_child[i] << ", " << right_child[i] << " | ";
//    }
//    cout << '\n';
//    cout << "\nheights:\n";
//    for (int i = 1; i <= n; i++) {
//        cout << i << "-- "<< "h: " << height[i][0] << ", v: " <<height[i][1] << " ";
//    }
//    cout << "\nlongest path:\n";
//    for (int i = 1; i <= n; i++) {
//        path_t p = longest_path[i];
//        cout << i << ": "<< "l: " << p.length << ", r: " << p.common_root << ", n2: " << p.second_node << "\n";
//    }
//    cout << "\nbinary przodkowie:\n";
//    for (int j = 0; j < jumps; j++) {
//        vector<int> v = binary_predecessors[j];
//        for (int i = 1; i <=n; i++) {
//            cout << v[i] << " ";
//        }
//        cout << "\n";
//    }
//    cout << "\n";
//    for (int i = 1; i <n; i++)
//        cout << depth[i] << " ";
//    cout << "\n";
    // DEBUG koniec


    // teraz wczytywanie poleceń bajtyny
    int m;
    cin >> m;
    int node_nr;
    int d;
    for (int i = 0; i < m; i++ ) {
        cin >> node_nr;
        cin >> d;
        int out = find_node_d_away(node_nr, d, longest_path, depth, binary_predecessors);
        cout << out << '\n';
    }
    return 0;
}