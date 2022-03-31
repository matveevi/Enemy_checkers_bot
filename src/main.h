#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <string>
#include <vector>

enum { MAX_SIZE_BOARD = 8 };

using Board = std::vector<std::vector<int>>;

void print_description_easy_lvl() {
    std::cout << "Ваш противник будет атаковать случайно, независимо от вашего "
                 "хода\n";
}

void print_description_hard_lvl() {
    std::cout << "Ваш противник будет расчитывать возможные исходы в "
                 "зависимости от вашего хода\n";
}

class Color {
  public:
    Color(int color) : color_(color) {
    }

    int EnemyColor() {
        if (color_ == 1) {
            return 2;
        } else if (color_ == 2) {
            return 1;
        }
        return 0;
    }

    int color_;
};

class State;

class Checker {
  public:
    Checker() : x_(0), y_(0), is_queen_(false), color_(0) {
    }

    Checker(int x, int y, int color)
        : x_(x), y_(y), is_queen_(false), color_(color) {
    }

    Checker(int x, int y, int color, bool is_queen)
        : x_(x), y_(y), is_queen_(is_queen), color_(color) {
    }

    std::vector<std::pair<std::pair<Board, bool>, int>>
    NextTurns(Board board, int priority) {
        assert(color_.color_ == 1);
        std::vector<std::pair<std::pair<Board, bool>, int>>
            turns; // bool это продолжить ход или нет
        int prior = priority;
        if (board[y_][x_] != -1) {
            if (x_ + 1 < 8 && y_ + 1 < 8) {
                if (board[y_ + 1][x_ + 1] == 0) {
                    prior = priority;
                    Board copy(board);
                    copy[y_][x_] = 0;
                    if (y_ + 1 == 7) {
                        copy[y_ + 1][x_ + 1] = -1;
                        prior++;
                    } else {
                        copy[y_ + 1][x_ + 1] = color_.color_;
                    }
                    auto make_state = std::make_pair(copy, false);
                    turns.emplace_back(make_state, prior);
                } else if (board[y_ + 1][x_ + 1] == color_.EnemyColor() &&
                           (y_ + 2 < 8) && (x_ + 2 < 8) &&
                           board[y_ + 2][x_ + 2] == 0) {
                    prior = priority;
                    Board copy(board);
                    copy[y_][x_] = 0;
                    copy[y_ + 1][x_ + 1] = 0;
                    if (y_ + 2 == 7) {
                        copy[y_ + 2][x_ + 2] = -1;
                        prior += 2;
                    } else {
                        copy[y_ + 2][x_ + 2] = color_.color_;
                        prior++;
                    }
                    auto make_state = std::make_pair(copy, true);
                    turns.emplace_back(make_state, prior);
                }
            }
            if (x_ - 1 >= 0 && x_ - 1 < 8 && y_ + 1 < 8) {
                if (board[y_ + 1][x_ - 1] == 0) {
                    prior = priority;
                    Board copy(board);
                    copy[y_][x_] = 0;
                    if (y_ + 1 == 7) {
                        copy[y_ + 1][x_ - 1] = -1;
                        prior++;
                    } else {
                        copy[y_ + 1][x_ - 1] = color_.color_;
                    }
                    auto make_state = std::make_pair(copy, false);
                    turns.emplace_back(make_state, prior);
                } else if (board[y_ + 1][x_ - 1] == color_.EnemyColor() &&
                           (y_ + 2 < 8) && (x_ - 2 < 8) && (x_ - 2 >= 0) &&
                           board[y_ + 2][x_ - 2] == 0) {
                    prior = priority;
                    Board copy(board);
                    copy[y_][x_] = 0;
                    copy[y_ + 1][x_ - 1] = 0;
                    if (y_ + 2 == 7) {
                        copy[y_ + 1][x_ - 2] = -1;
                        prior += 2;
                    } else {
                        copy[y_ + 2][x_ - 2] = color_.color_;
                        prior++;
                    }
                    auto make_state = std::make_pair(copy, true);
                    turns.emplace_back(make_state, prior);
                }
            }

            // рубка ходом назад
            if (x_ - 1 >= 0 && x_ - 1 < 8 && y_ - 1 < 8 && y_ - 1 >= 0) {
                if (board[y_ - 1][x_ - 1] == color_.EnemyColor() &&
                    (y_ - 2 < 8) && (y_ - 2 >= 0) && (x_ - 2 < 8) &&
                    (x_ - 2 >= 0) && board[y_ - 2][x_ - 2] == 0) {
                    Board copy(board);
                    copy[y_][x_] = 0;
                    copy[y_ - 1][x_ - 1] = 0;
                    copy[y_ - 2][x_ - 2] = color_.color_;
                    auto make_state = std::make_pair(copy, true);
                    turns.emplace_back(make_state, priority + 1);
                }
            }

            if (x_ + 1 < 8 && y_ - 1 < 8 && y_ - 1 >= 0) {
                if (board[y_ - 1][x_ + 1] == color_.EnemyColor() &&
                    (x_ + 2 < 8) && (y_ - 2 < 8) && (y_ - 2 >= 0) &&
                    board[y_ - 2][x_ + 2] == 0) {
                    Board copy(board);
                    copy[y_][x_] = 0;
                    copy[y_ - 1][x_ + 1] = 0;
                    copy[y_ - 2][x_ + 2] = color_.color_;
                    auto make_state = std::make_pair(copy, true);
                    turns.emplace_back(make_state, priority + 1);
                }
            }
        } else if (board[y_][x_] == -1) {
            // Queen turn
            int tmpX, tmpY;
            int tmpBeatX, tmpBeatY;
            // ++
            tmpX = x_;
            tmpY = y_;
            while (true) {
                tmpBeatX = 1;
                tmpBeatY = 1;
                tmpX++;
                tmpY++;
                if ((tmpX >= 0) && (tmpX < 8) && (tmpY >= 0) && (tmpY < 8)) {
                    if (board[tmpY][tmpX] == 0) {
                        Board copy(board);
                        copy[y_][x_] = 0;
                        copy[tmpY][tmpX] = color_.color_;
                        auto make_state = std::make_pair(copy, false);
                        turns.emplace_back(make_state, priority);
                    } else if (board[tmpY][tmpX] == color_.EnemyColor()) {
                        while (
                            (tmpX + tmpBeatX >= 0) && (tmpX + tmpBeatX < 8) &&
                            (tmpY + tmpBeatY >= 0) && (tmpY + tmpBeatY < 8)) {
                            if (board[tmpY + tmpBeatY][tmpX + tmpBeatX] == 0) {
                                Board copy(board);
                                copy[y_][x_] = 0;
                                copy[tmpY][tmpX] = 0;
                                copy[tmpY + tmpBeatY][tmpX + tmpBeatX] =
                                    color_.color_;
                                auto make_state = std::make_pair(copy, true);
                                turns.emplace_back(make_state, priority + 1);
                            }
                            tmpBeatX++;
                            tmpBeatY++;
                        }
                    }
                } else {
                    break;
                }
            }
            // +-
            tmpX = x_;
            tmpY = y_;
            while (true) {
                tmpBeatX = 1;
                tmpBeatY = 1;
                tmpX++;
                tmpY--;
                if ((tmpX >= 0) && (tmpX < 8) && (tmpY >= 0) && (tmpY < 8)) {
                    if (board[tmpY][tmpX] == 0) {
                        Board copy(board);
                        copy[y_][x_] = 0;
                        copy[tmpY][tmpX] = color_.color_;
                        auto make_state = std::make_pair(copy, false);
                        turns.emplace_back(make_state, priority);
                    } else if (board[tmpY][tmpX] == color_.EnemyColor()) {
                        while (
                            (tmpX + tmpBeatX >= 0) && (tmpX + tmpBeatX < 8) &&
                            (tmpY + tmpBeatY >= 0) && (tmpY + tmpBeatY < 8)) {
                            if (board[tmpY + tmpBeatY][tmpX + tmpBeatX] == 0) {
                                Board copy(board);
                                copy[y_][x_] = 0;
                                copy[tmpY + tmpBeatY][tmpX + tmpBeatX] =
                                    color_.color_;
                                auto make_state = std::make_pair(copy, true);
                                turns.emplace_back(make_state, priority + 1);
                            } else {
                                break;
                            }
                            tmpBeatX++;
                            tmpBeatY--;
                        }
                    }
                } else {
                    break;
                }
            }
            // -+
            tmpX = x_;
            tmpY = y_;
            while (true) {
                tmpBeatX = 1;
                tmpBeatY = 1;
                tmpX--;
                tmpY++;
                if ((tmpX >= 0) && (tmpX < 8) && (tmpY >= 0) && (tmpY < 8)) {
                    if (board[tmpY][tmpX] == 0) {
                        Board copy(board);
                        copy[y_][x_] = 0;
                        copy[tmpY][tmpX] = color_.color_;
                        auto make_state = std::make_pair(copy, false);
                        turns.emplace_back(make_state, priority);
                    } else if (board[tmpY][tmpX] == color_.EnemyColor()) {
                        while (
                            (tmpX + tmpBeatX >= 0) && (tmpX + tmpBeatX < 8) &&
                            (tmpY + tmpBeatY >= 0) && (tmpY + tmpBeatY < 8)) {
                            if (board[tmpY + tmpBeatY][tmpX + tmpBeatX] == 0) {
                                Board copy(board);
                                copy[y_][x_] = 0;
                                copy[tmpY + tmpBeatY][tmpX + tmpBeatX] =
                                    color_.color_;
                                auto make_state = std::make_pair(copy, true);
                                turns.emplace_back(make_state, priority + 1);
                            } else {
                                break;
                            }
                            tmpBeatX--;
                            tmpBeatY++;
                        }
                    }
                } else {
                    break;
                }
            }
            // --
            tmpX = x_;
            tmpY = y_;
            while (true) {
                tmpBeatX = 1;
                tmpBeatY = 1;
                tmpX--;
                tmpY--;
                if ((tmpX >= 0) && (tmpX < 8) && (tmpY >= 0) && (tmpY < 8)) {
                    if (board[tmpY][tmpX] == 0) {
                        Board copy(board);
                        copy[y_][x_] = 0;
                        copy[tmpY][tmpX] = color_.color_;
                        auto make_state = std::make_pair(copy, false);
                        turns.emplace_back(make_state, priority);
                    } else if (board[tmpY][tmpX] == color_.EnemyColor()) {
                        while (
                            (tmpX + tmpBeatX >= 0) && (tmpX + tmpBeatX < 8) &&
                            (tmpY + tmpBeatY >= 0) && (tmpY + tmpBeatY < 8)) {
                            if (board[tmpX + tmpBeatX][tmpY + tmpBeatY] == 0) {
                                Board copy(board);
                                copy[y_][x_] = 0;
                                copy[tmpY + tmpBeatY][tmpX + tmpBeatX] =
                                    color_.color_;
                                auto make_state = std::make_pair(copy, true);
                                turns.emplace_back(make_state, priority + 1);
                            } else {
                                break;
                            }
                            tmpBeatX--;
                            tmpBeatY--;
                        }
                    }
                } else {
                    break;
                }
            }

        }

        return turns;
    }

    bool operator<(const Checker& b_ch) const { // for std::set
        if (x_ < b_ch.x_) {
            return true;
        } else if (x_ > b_ch.x_) {
            return false;
        } else if (y_ < b_ch.y_) {
            return true;
        }
        return false;
    }

    int x_;
    int y_;
    bool is_queen_;
    Color color_;
};

bool BadInputCoord(const int& x, const int& y) {
    if (x < 0 || x >= 8) {
        return true;
    }
    if (y < 0 || y >= 8) {
        return true;
    }

    return false;
}

class State {
    friend class Checker;

  public:
    int IsFinish() {
        int count_black = 0;
        int count_white = 0;

        for (auto it : board_) {
            for (auto it2 : it) {
                if (it2 == 1) {
                    count_black++;
                } else if (it2 == 2) {
                    count_white++;
                }
            }
        }
        if (count_white == 0 && count_black != 0) {
            return 1;
        } else if (count_white != 0 && count_black == 0) {
            return 2;
        }
        return 0;
    }

    State() : board_(8) {
        for (auto& it : board_) {
            it.resize(8);
            std::fill(it.begin(), it.end(), 0);
        }
        int now_color = 1;
        for (int i = 0; i < 8; i++) {
            if ((now_color == 1 && (i == 0 || i == 2)) ||
                (now_color == 2 && (i == 6))) {
                for (int j = 1; j < 8; j += 2) {
                    board_[i][j] = now_color;
                    if (now_color == 1) {
                        checkers_.emplace(j, i, now_color);
                    }
                }
            } else {
                for (int j = 0; j < 8; j += 2) {
                    board_[i][j] = now_color;
                    if (now_color == 1) {
                        checkers_.emplace(j, i, now_color);
                    }
                }
            }
            if (i == 2) {
                i = 4;
                now_color = 2;
            }
        }
    }

    State(Board board) : board_(board) {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                if (board_[y][x] == 1) {
                    checkers_.emplace(x, y, 1);
                }
            }
        }
    }

    State(std::pair<Board, bool> pair)
        : board_(pair.first), can_continue_turn_(pair.second) {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                if (board_[y][x] == 1) {
                    checkers_.emplace(x, y, 1);
                } else if (board_[y][x] == 4) {
                    checkers_.emplace(x, y, true, 1);
                }
            }
        }
    }

    std::vector<std::pair<std::pair<State, bool>, int>> NextStates() {
        // std::cout << "Next_state";
        std::vector<std::pair<std::pair<State, bool>, int>> states;
        for (auto checker : checkers_) {
            for (auto now_pair : checker.NextTurns(board_, 0)) {
                states.push_back(now_pair);
            }
        }
        return states;
    }

    int ClientTurn(int x_from, int y_from, int x_to,
                   int y_to) { // 0 - простой ход, 1 - ход с рубкой, 2 - ошибка
        if (BadInputCoord(x_to, y_to) || BadInputCoord(x_from, y_from)) {
            return 2;
        }
        if (board_[y_from][x_from] == 2) {
            if (board_[y_to][x_to] == 1 &&
                !BadInputCoord(x_from + (x_to - x_from) * 2,
                               y_from + (y_to - y_from) * 2) &&
                board_[y_from + (y_to - y_from) * 2]
                      [x_from + (x_to - x_from) * 2] == 0) { // рубка
                board_[y_from][x_from] = 0;
                board_[y_to][x_to] = 0;
                board_[y_from + (y_to - y_from) * 2]
                      [x_from + (x_to - x_from) * 2] = 2;
                if (y_from + (y_to - y_from) * 2 == 0) {
                    std::cout << "У вас появилась королева\n";
                    queens_.emplace(y_from + (y_to - y_from) * 2,
                                    x_from + (x_to - x_from) * 2);
                    board_[y_from + (y_to - y_from) * 2]
                          [x_from + (x_to - x_from) * 2] = 3;
                }
                return 1;
            }
            if (board_[y_to][x_to] == 0 && y_from > y_to) {
                board_[y_from][x_from] = 0;
                if (y_to == 0) {
                    std::cout << "У вас появилась королева\n";
                    queens_.emplace(y_to, x_to);
                    board_[y_to][x_to] = 3;
                } else {
                    board_[y_to][x_to] = 2;
                }
                return 0;
            }
        }

        if ((board_[y_from][x_from] == 3) &&
            (((y_to - y_from) / (x_to - x_from) == 1) ||
             ((y_to - y_from) / (x_to - x_from) == -1))) {

            std::set<std::pair<int, int>> enemies;
            if (x_to > x_from) {
                if (y_to > y_from) {
                    for (int offset = 1; (offset + y_from) != y_to; offset++) {
                        if (board_[y_from + offset][x_from + offset] == 1) {
                            if (board_[y_from + offset - 1]
                                      [x_from + offset - 1] == 1) {
                                return 2;
                            }
                            enemies.emplace(y_from + offset, x_from + offset);
                        }
                    }
                } else {
                    for (int offset = 1; (-offset + y_from) != y_to; offset++) {
                        if (board_[y_from - offset][x_from + offset] == 1) {
                            if ((y_from - offset + 1) >= 0 &&
                                (x_from + offset - 1) >= 0 &&
                                board_[y_from - offset + 1]
                                      [x_from + offset - 1] == 1) {
                                return 2;
                            }
                            enemies.emplace(y_from - offset, x_from + offset);
                        }
                    }
                }
            } else {
                if (y_to > y_from) {
                    for (int offset = 1; (offset + y_from) != y_to; offset++) {
                        if (board_[y_from + offset][x_from - offset] == 1) {
                            if ((x_from - offset + 1) >= 0 &&
                                board_[y_from + offset - 1]
                                      [x_from - offset + 1] == 1) {
                                return 2;
                            }
                            enemies.emplace(y_from + offset, x_from - offset);
                        }
                    }
                } else {
                    for (int offset = 1; (-offset + y_from) != y_to; offset++) {
                        if (board_[y_from - offset][x_from - offset] == 1) {
                            if (y_from - offset + 1 >= 0 &&
                                x_from - offset + 1 >= 0 &&
                                board_[y_from - offset + 1]
                                      [x_from - offset + 1] == 1) {
                                return 2;
                            }
                            enemies.emplace(y_from - offset, x_from - offset);
                        }
                    }
                }
            }
            if (!enemies.empty()) {
                for (auto del_point : enemies) {
                    board_[del_point.first][del_point.second] = 0;
                }
                return 1;
            } else {
                return 0;
            }

            board_[y_to][x_to] = 3;
        }

        return 2;
    }

    void PrintState() {
        int id_coord_y = 0;

        for (int i = 0; i < 8; i++) {
            std::cout << i << ' ';
        }

        std::cout << ":X Y: \n";

        for (auto it : board_) {
            int id_coord_x = 0;
            for (auto it2 : it) {
                if (it2 == 2) {
                    std::cout << 2 << " ";
                } else if (it2 == 3) {
                    std::cout << 3 << " ";
                } else if (it2 == 1) {
                    std::cout << 'X' << " ";
                } else if (it2 == -1) {
                    std::cout << 'Q' << " ";
                } else {
                    std::cout << "  ";
                }
                id_coord_x++;
            }
            std::cout << "   " << id_coord_y << '\n';
            id_coord_y++;
        }

        if (!queens_.empty()) {
            // std::cout << "You have queens: \n";
            std::cout << "У вас есть королевы: \n";
            for (auto it : queens_) {
                std::cout << "x: " << it.second << " y: " << it.first << '\n';
            }
        }
    }

    void PrintCheckers() {
        std::cout << "CHECKERS:\n";
        for (auto it : checkers_) {
            std::cout << it.x_ << " " << it.y_ << '\n';
        }
        std::cout << "END_CHECKERS:\n";
    }

    void Update() {
        for (auto it : checkers_) {
            if (!it.is_queen_ && board_[it.y_][it.x_] != 1) {
                checkers_.erase(it);
            }
            if (it.is_queen_ && board_[it.y_][it.x_] != -1) {
                checkers_.erase(it);
            }
        }
    }

    void operator=(const std::pair<State, bool>& pair) {
        *this = pair.first;
        can_continue_turn_ = pair.second;
    }

    Board board_;

    // auto cmp = [](const Checker& a, const Checker& b) {
    //     return true;
    // };
    std::set<Checker> checkers_;
    std::set<std::pair<int, int>> queens_;
    bool can_continue_turn_;
};

void HardGame(State& st) {
    std::cout << "!HARD GAME!\n";
    while (st.IsFinish() == 0) {
        // std::cout <<  "Your turn: from x, y to x, y\nUse spaces between
        // numbers:\n";
        std::cout
            << "Ваш ход\nВведите координаты шашки, которую хотите сдвинуть, и "
               "координаты куда поставить ее: шашку: x, y передвинуть на x, "
               "y\nИспользуйте пробелы между числами:\n";
        int x_from, y_from, x_to, y_to = 0;
        std::cin >> x_from >> y_from >> x_to >> y_to;
        if (BadInputCoord(x_from, y_from) || BadInputCoord(x_to, y_to)) {
            // std::cout << "1_bad input\n";
            std::cout << "Неверный ввод\n";
            continue;
        }

        int result_turn = 2;
        if ((result_turn = st.ClientTurn(x_from, y_from, x_to, y_to)) != 2) {
            st.PrintState();
            st.Update();
            // st.PrintCheckers();
            if (result_turn == 1) {
                continue;
            }
        } else {
            std::cout << "Неверный ввод\n";
            continue;
        }

        while (st.IsFinish() == 0) {
            auto next_states = st.NextStates();

            // Find max prior
            int max_prior = -1;
            std::pair<State, bool> max_prior_state;
            for (auto it : next_states) {
                if (it.second > max_prior) {
                    max_prior_state = it.first;
                    max_prior = it.second;
                }
            }

            st = max_prior_state;
            st.PrintState();
            if (!st.can_continue_turn_) {
                break;
            }
        }
    }

    if (st.IsFinish() == 1) {
        // std::cout << "You lose\n";
        std::cout << "***/////Вы победили!/////***";
    } else if (st.IsFinish() == 2) {
        std::cout << "/ / / Вы проиграли / / /";
        // std::cout << "You win!\n";
    }
}

void EasyGame(State& st) {
    std::cout << "!EASY GAME!\n";
    while (st.IsFinish() == 0) {
        // std::cout
        //     << "Your turn: from x, y to x, y\nUse spaces between numbers:\n";
        std::cout
            << "Ваш ход\nВведите координаты шашки, которую хотите сдвинуть, и "
               "координаты куда поставить ее: шашку: x, y передвинуть на x, "
               "y\nИспользуйте пробелы между числами:\n";
        int x_from, y_from, x_to, y_to = 0;
        std::cin >> x_from >> y_from >> x_to >> y_to;
        if (BadInputCoord(x_from, y_from) || BadInputCoord(x_to, y_to)) {
            std::cout << "Неверный ввод\n";
            continue;
        }

        int result_turn = 2;
        if ((result_turn = st.ClientTurn(x_from, y_from, x_to, y_to)) != 2) {
            st.PrintState();
            st.Update();
            // st.PrintCheckers();
            if (result_turn == 1) {
                continue;
            }
        } else {
            std::cout << "Неверный ввод\n";
            continue;
        }

        while (st.IsFinish() == 0) {
            auto next_states = st.NextStates();
            int rand_turn = rand() % next_states.size();
            st = next_states[rand_turn].first;
            st.PrintState();
            if (!st.can_continue_turn_) {
                break;
            }
        }
    }

    if (st.IsFinish() == 1) {
        // std::cout << "You lose\n";
        std::cout << "***/////Вы победили!/////***";
    } else if (st.IsFinish() == 2) {
        std::cout << "/ / / Вы проиграли / / /";
        // std::cout << "You win!\n";
    }
}
