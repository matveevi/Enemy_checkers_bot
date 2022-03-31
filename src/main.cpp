#include "main.h"

int main() {
    State st;
    st.PrintState();

    /// input data

    std::cout << "Выберете цифру для определения сложности игры\n";
    std::cout << "1 - легкий\n2 - сложный\n";
    int game_category = 0;
    std::cin >> game_category;

    if (game_category == 1) {
        print_description_easy_lvl();
        EasyGame(st);
    } else if (game_category == 2) {
        print_description_hard_lvl();
        HardGame(st);
    }

    return 0;
}