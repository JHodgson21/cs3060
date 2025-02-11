#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void display_menu();
void add_player(vector<string>& jersey_numbers, vector<string>& ratings);
void delete_player(vector<string>& jersey_numbers, vector<string>& ratings);
void update_player_rating(vector<string>& jersey_numbers, vector<string>& ratings);
void output_player_rating(const vector<string>& jersey_numbers, const vector<string>& ratings);
void return_roster(const vector<string>& jersey_numbers, const vector<string>& ratings);

int main() {
    vector<string> jersey_numbers;
    vector<string> ratings;

    for (int i = 0; i < 5; ++i) {
        string jersey, rating;
        cout << "Enter player " << i + 1 << "'s jersey number: ";
        cin >> jersey;
        jersey_numbers.push_back(jersey);

        cout << "Enter player " << i + 1 << "'s rating: ";
        cin >> rating;
        ratings.push_back(rating);
    }

    while (true) {
        display_menu();

        char user_choice;
        cout << "Choose an option: ";
        cin >> user_choice;

        switch (user_choice) {
            case 'a':
                add_player(jersey_numbers, ratings);
                break;
            case 'd':
                delete_player(jersey_numbers, ratings);
                break;
            case 'u':
                update_player_rating(jersey_numbers, ratings);
                break;
            case 'r':
                output_player_rating(jersey_numbers, ratings);
                break;
            case 'o':
                return_roster(jersey_numbers, ratings);
                break;
            case 'q':
                cout << "Quitting the program. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
        }
    }

    return 0;
}

void display_menu() {
    cout << "------------------------------------\n"
         << "MENU\n"
         << "a - Add player\n"
         << "d - Remove player rating\n"
         << "u - Update player rating\n"
         << "r - Output players above a rating\n"
         << "o - Output roster\n"
         << "q - Quit\n";
}

void add_player(vector<string>& jersey_numbers, vector<string>& ratings) {
    string new_player_jersey, new_player_rating;
    cout << "Enter a new player's jersey number: ";
    cin >> new_player_jersey;

    cout << "Enter the player's rating: ";
    cin >> new_player_rating;

    jersey_numbers.push_back(new_player_jersey);
    ratings.push_back(new_player_rating);
}

void delete_player(vector<string>& jersey_numbers, vector<string>& ratings) {
    string delete_a_player;
    cout << "Enter a jersey number: ";
    cin >> delete_a_player;

    auto it = find(jersey_numbers.begin(), jersey_numbers.end(), delete_a_player);
    if (it != jersey_numbers.end()) {
        size_t index = distance(jersey_numbers.begin(), it);
        jersey_numbers.erase(jersey_numbers.begin() + index);
        ratings.erase(ratings.begin() + index);
    } else {
        cout << delete_a_player << " not found in data\n";
    }
}

void update_player_rating(vector<string>& jersey_numbers, vector<string>& ratings) {
    string updates;
    cout << "Enter a jersey number: ";
    cin >> updates;

    auto it = find(jersey_numbers.begin(), jersey_numbers.end(), updates);
    if (it != jersey_numbers.end()) {
        size_t index = distance(jersey_numbers.begin(), it);
        string new_rating;
        cout << "Enter a new rating for player: ";
        cin >> new_rating;
        ratings[index] = new_rating;
    } else {
        cout << updates << " is not a saved jersey number.\n";
    }
}

void output_player_rating(const vector<string>& jersey_numbers, const vector<string>& ratings) {
    int the_rating;
    cout << "Enter a rating: ";
    cin >> the_rating;

    cout << "------------------------------------\n"
         << "ABOVE " << the_rating << "\n";

    for (size_t i = 0; i < ratings.size(); ++i) {
        if (stoi(ratings[i]) > the_rating) {
            cout << "Player " << i + 1 << " -- Jersey number: " << jersey_numbers[i] << ", Rating: " << ratings[i] << "\n";
        }
    }
}

void return_roster(const vector<string>& jersey_numbers, const vector<string>& ratings) {
    cout << "Roster\n";
    for (size_t j = 0; j < jersey_numbers.size(); ++j) {
        cout << "Player " << j + 1 << " -- Jersey number: " << jersey_numbers[j] << ", Rating: " << ratings[j] << "\n";
    }
}
