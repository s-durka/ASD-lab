// lab 07 ASD
#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <vector>
#include <climits>

using namespace std;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    int n, m, k;
    cin >> n;
    cin >> m;
    cin >> k;

    vector<int> wierzcholki[n+1];
    int programisci[n + 1];
    int odIluZalezy[n + 1]; // dopiero jak jest równe zero można zdjąc
    for (int i = 1; i <= n; i++)
        odIluZalezy[i] = 0;

    int potrzebniProgramisci;
    for (int i = 1; i <= n; i++) {
        cin >> potrzebniProgramisci;
        programisci[i] = potrzebniProgramisci;
    }
    // wczytywanie zależności:
    int projektKonieczny;
    int projektZalezny;
    for (int i = 1; i <= m; i++) {
        cin >> projektZalezny;
        cin >> projektKonieczny;
        wierzcholki[projektKonieczny].push_back(projektZalezny);
        odIluZalezy[projektZalezny]++;
    }
    /*
     * pierwszy el. pary to to, po czym sortuje najpierw, czyli to musi być liczba programistow
     * drugi el. to numer projektu
     */
    priority_queue<int, vector<pair<int, int>>, greater<pair<int, int>>> pQueueMin; // kolejka priorytetowa typu MAX
    int maxPotrzebnychProgramistow = 0;
    for (int i = 1; i <= n; i++) {
        if (odIluZalezy[i] == 0) {
            pair<int, int> p = make_pair(programisci[i], i);
            pQueueMin.push(p);
        }
    }
    int count = 0;
    while (count < k && !pQueueMin.empty()) {
        pair<int, int> projekt = pQueueMin.top();
        pQueueMin.pop();
        count++;
        int programisciPotrzebni = projekt.first;
        if (programisciPotrzebni > maxPotrzebnychProgramistow) maxPotrzebnychProgramistow = programisciPotrzebni;
        int projektNr = projekt.second;
        for (auto zalezny : wierzcholki[projektNr]) {
            --odIluZalezy[zalezny];
            if (odIluZalezy[zalezny] == 0) {
                pair <int, int> p = make_pair(programisci[zalezny], zalezny);
                pQueueMin.push(p);
            }
        }
    }
    cout << maxPotrzebnychProgramistow << "\n";

    return 0;
}