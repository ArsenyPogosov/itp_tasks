#pragma once

#include <string>
#include <vector>

class Minesweeper {
private:
    class GameCell {
    public:
        enum class GameCellType {
            MINED,
            CLEARED,
        };

        enum class GameCellStatus {
            CLOSED,
            OPENED,
            FLAGGED,
        };

        GameCellType type_;
        GameCellStatus status_;
        uint16_t neighbour_mine_count;

        GameCell();
        GameCell(GameCellType type, GameCellStatus status);

        void OpenCommand();
        void MarkCommand();
    };

    using GameData = std::vector<std::vector<GameCell>>;

    size_t width_, height_;
    GameData* data_ = nullptr;
    uint_fast64_t yet_to_open_;
    time_t game_start_time_;

public:
    struct Cell {
        size_t x = 0;
        size_t y = 0;
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    using RenderedField = std::vector<std::string>;

    Minesweeper(size_t width, size_t height, size_t mines_count);
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);
    ~Minesweeper();

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);

    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField() const;

private:
    GameStatus game_status_ = GameStatus::NOT_STARTED;
    void SetWin();
    void SetDefeat();
    GameCell& GetGameCell(const Cell& cell) const;
};
