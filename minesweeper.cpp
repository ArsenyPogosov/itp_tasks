#include <unordered_set>
#include <ctime>
#include <queue>
#include "minesweeper.h"

Minesweeper::GameCell::GameCell() {
    type_ = GameCellType::CLEARED;
    status_ = GameCellStatus::CLOSED;
    neighbour_mine_count = 0;
}

Minesweeper::GameCell::GameCell(Minesweeper::GameCell::GameCellType type,
                                Minesweeper::GameCell::GameCellStatus status) {
    type_ = type;
    status_ = status;
    neighbour_mine_count = 0;
}

void Minesweeper::GameCell::OpenCommand() {
    if (status_ == GameCellStatus::CLOSED) {
        status_ = GameCellStatus::OPENED;
    }
}

void Minesweeper::GameCell::MarkCommand() {
    if (status_ == GameCellStatus::CLOSED) {
        status_ = GameCellStatus::FLAGGED;
    } else if (status_ == GameCellStatus::FLAGGED) {
        status_ = GameCellStatus::CLOSED;
    }
}

Minesweeper::GameCell& Minesweeper::GetGameCell(const Minesweeper::Cell& cell) const {
    return (*data_)[cell.x][cell.y];
}

Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count) {
    NewGame(width, height, mines_count);
}

Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    NewGame(width, height, cells_with_mines);
}

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    std::vector<Minesweeper::Cell> cells_with_mines;
    cells_with_mines.reserve(mines_count);

    const auto get_cell_by_number = [=](uint_fast64_t n) {
        return Minesweeper::Cell({static_cast<size_t>(n / height), static_cast<size_t>(n % height)});
    };

    // Floyd's algorithm for generating m DIFFERENT numbers from 0 to n - 1
    uint_fast64_t n = static_cast<uint_fast64_t>(width) * height, m = mines_count;
    std::unordered_set<uint_fast64_t> chosen;
    for (uint_fast64_t i = n - m; i < n; ++i) {
        uint_fast64_t curr = rand() % (i + 1);
        if (chosen.find(curr) != chosen.end()) {
            curr = i;
        }
        chosen.insert(curr);

        cells_with_mines.push_back(get_cell_by_number(curr));
    }

    NewGame(width, height, cells_with_mines);
}

std::vector<Minesweeper::Cell> GetNeighbours(const Minesweeper::Cell& cell, size_t width, size_t height) {
    std::vector<Minesweeper::Cell> result;
    for (int_fast16_t x_offset = -1; x_offset <= 1; ++x_offset) {
        if ((cell.x == 0 && x_offset == -1) || (cell.x == width - 1 && x_offset == 1)) {
            continue;
        }
        for (int_fast16_t y_offset = -1; y_offset <= 1; ++y_offset) {
            if ((cell.y == 0 && y_offset == -1) || (cell.y == height - 1 && y_offset == 1)) {
                continue;
            }
            if (x_offset == 0 && y_offset == 0) {
                continue;
            }
            result.push_back({cell.x + x_offset, cell.y + y_offset});
        }
    }

    return result;
}

void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Minesweeper::Cell>& cells_with_mines) {
    width_ = width;
    height_ = height;
    yet_to_open_ = static_cast<uint_fast64_t>(width_) * height_ - cells_with_mines.size();
    if (data_) {
        delete data_;
    }
    data_ = new GameData(width, std::vector<GameCell>(height));

    for (const auto& cell : cells_with_mines) {
        GetGameCell(cell).type_ = GameCell::GameCellType::MINED;
    }

    for (size_t x = 0; x < width_; ++x) {
        for (size_t y = 0; y < height_; ++y) {
            for (const auto& neighbour : GetNeighbours({x, y}, width_, height_)) {
                if (GetGameCell(neighbour).type_ == GameCell::GameCellType::MINED) {
                    ++GetGameCell({x, y}).neighbour_mine_count;
                }
            }
        }
    }
    game_status_ = Minesweeper::GameStatus::IN_PROGRESS;

    game_start_time_ = std::time(nullptr);
}

Minesweeper::~Minesweeper() {
    delete data_;
}

void Minesweeper::OpenCell(const Minesweeper::Cell& cell) {
    const auto continue_spread = [&](GameCell& cell) {
        return (cell.status_ == GameCell::GameCellStatus::OPENED) && (cell.neighbour_mine_count == 0);
    };

    GameCell& game_cell = GetGameCell(cell);
    if (game_cell.type_ == GameCell::GameCellType::MINED) {
        SetDefeat();
        return;
    }
    if (game_cell.status_ != GameCell::GameCellStatus::CLOSED) {
        return;
    }

    std::queue<Minesweeper::Cell> to_open;
    to_open.push(cell);
    while (!to_open.empty()) {
        Minesweeper::Cell curr_cell = to_open.front();
        to_open.pop();

        GameCell& curr_game_cell = GetGameCell(curr_cell);
        if (curr_game_cell.status_ != GameCell::GameCellStatus::CLOSED) {
            continue;
        }
        curr_game_cell.OpenCommand();
        --yet_to_open_;
        if (continue_spread(curr_game_cell)) {
            for (const auto& neighbour : GetNeighbours(curr_cell, width_, height_)) {
                if (GetGameCell(neighbour).status_ == GameCell::GameCellStatus::CLOSED) {
                    to_open.push(neighbour);
                }
            }
        }
    }

    if (yet_to_open_ == 0) {
        SetWin();
    }
}

void Minesweeper::MarkCell(const Minesweeper::Cell& cell) {
    GetGameCell(cell).MarkCommand();
}

void Minesweeper::SetWin() {
    game_status_ = GameStatus::VICTORY;
}

void Minesweeper::SetDefeat() {
    for (auto& row : *data_) {
        for (auto& cell : row) {
            cell.status_ = GameCell::GameCellStatus::OPENED;
        }
    }

    game_status_ = GameStatus::DEFEAT;
}

Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    return game_status_;
}

time_t Minesweeper::GetGameTime() const {
    return game_start_time_;
}

Minesweeper::RenderedField Minesweeper::RenderField() const {
    RenderedField result(height_, std::string(width_, 0));

    const auto render_game_cell = [](GameCell game_cell) {
        if (game_cell.status_ == GameCell::GameCellStatus::CLOSED) {
            return '-';
        }
        if (game_cell.status_ == GameCell::GameCellStatus::FLAGGED) {
            return '?';
        }
        if (game_cell.type_ == GameCell::GameCellType::MINED) {
            return '*';
        }
        if (game_cell.neighbour_mine_count == 0) {
            return '.';
        }
        return static_cast<char>('0' + game_cell.neighbour_mine_count);
    };
    for (size_t x = 0; x < width_; ++x) {
        for (size_t y = 0; y < height_; ++y) {
            result[y][x] = render_game_cell(GetGameCell({x, y}));
        }
    }

    return result;
}