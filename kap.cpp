// kapitan zadanie zaliczeniowe

#include <iostream>
#include <bits/stdc++.h>
using namespace std;
using point_t = pair<int,int>;

// the distance between two points on the graph is
// equal to min(|x1-x2|, |y1-y2|)

void add_neighbors(vector<point_t>& islands, map<point_t, vector<point_t>>& neighbors) {
    // dodaje neighborsów do mapy neighborsów
    for (int i = 0; i < islands.size(); i++) {
        if (i+1 < islands.size()) {
            neighbors.at(islands[i]).push_back(islands[i + 1]);
        }
        if (i > 0) {
            neighbors.at(islands[i]).push_back(islands[i - 1]);
        }
    }
}

int weight(point_t p1, point_t p2) {
    return min(abs(p1.first - p2.first), abs(p1.second - p2.second));
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    point_t start_island;
    point_t end_island;
    set<pair<int, point_t>> priority_q;
    map<point_t, int> distances_map;
    vector<point_t> islands; // będzie sortowany wzgl x, potem y
    // i użyte do stworzenia mapy neighbors_map
    map<point_t, vector<point_t>> neighbors_map;

    // czytanie inputu:
    int n, x, y;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> x;
        cin >> y;
        point_t island = make_pair(x,y);
        if (i == 0) start_island = island;
        else if (i == n - 1) end_island = island;
        islands.push_back(island);
        neighbors_map.insert(make_pair(island, vector<point_t>()));
    }

    sort(islands.begin(), islands.end());
    add_neighbors(islands, neighbors_map); // najpierw po 1-szej współrzędnej

    sort(islands.begin(), islands.end(),
         [](point_t v1, point_t v2){
             if(v1.second == v2.second) return v1.first < v2.first;
             return v1.second < v2.second;
         });
    add_neighbors(islands, neighbors_map); // teraz po 2-giej współrzędnej

    // dodajemy wierzcholki oraz ich wagi od startu do "kolejki priorytetowej" (set)
    priority_q.insert(make_pair(0, start_island));
    for (int i = 0; i < n; i++) {
        distances_map.insert(make_pair(islands[i], INT_MAX)); // klucz == współrzędne
    }
    distances_map.at(start_island) = 0;


    while(!priority_q.empty()) {
        pair<int, point_t> u_pair = *(priority_q.begin());
        priority_q.erase(priority_q.begin());
        point_t u = u_pair.second;
        // teraz updatujemy wagi sąsiadów v:
        vector<point_t> adj = neighbors_map.at(u);
        for (point_t w : adj) {
            int new_weight = weight(u, w) + u_pair.first;
            if (new_weight < distances_map.at(w)) {
                priority_q.erase(make_pair(distances_map.at(w), w)); // usuwamy sąsiada ze starym dist z prio Q
                priority_q.insert(make_pair(new_weight, w)); // wstawiamy sąsiada z nowym dist do prio Q
                distances_map.find(w)->second = new_weight; // zmieniamy dist w mapie dystansów
            }
        }
    }
    int min_dist_start_to_end = distances_map.at(end_island);
    cout << min_dist_start_to_end << '\n';

    return 0;
}