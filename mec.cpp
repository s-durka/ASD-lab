#include <iostream>
#include <unordered_set>


unsigned long long int record_new_game(unsigned long long int games_seq, unsigned long long int multiplier, int team) {
    // jeśli w drużynie na prawo to + 1 do ciagu binarnego
    // jesli na lewo to 0
    // team = 0 lub 1
    return games_seq + team * multiplier;
}

int main() {
    std::ios::sync_with_stdio(false);
    
    int n; // liczba zawodników -- zał. n % 2 == 0
    int m; // liczba meczów <= 50
    std::cin >> n;
    std::cin >> m;

    std::unordered_set<unsigned long long int> players_set;
    unsigned long long int players_arr[n];
    for (int c = 0; c < n; c++)
        players_arr[c] = 0;

    unsigned long long int multiplier = 1;
    int player_nr;
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n/2; i++) {
            std::cin >> player_nr;
            //players_arr[i] = record_new_game(players_arr[i], multiplier, 0); // to nic nie zmienia więc po prostu multiplier *= 2
        }
        for (int i = n/2; i < n; i++) {
            std::cin >> player_nr;
            players_arr[player_nr - 1] = record_new_game(players_arr[player_nr - 1], multiplier, 1);
        }
        multiplier *= 2;
    }

    bool two_same_sequences = false;
    for (int i = 0; i < n && !two_same_sequences; i++) {
        if (players_set.count(players_arr[i]) > 0)
            two_same_sequences = true;
        players_set.insert(players_arr[i]);
    }


    if (two_same_sequences)
        std::cout << "NIE";
    else
        std::cout << "TAK";

    return 0;
}