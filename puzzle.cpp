//
// Created by Vlad Isan on 05/12/2023.
//

#include "puzzle.h"

uint64_t puzzle_sample_1(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-sample-1.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_1(file);
}

uint64_t puzzle_sample_2(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-sample-2.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_2(file);
}

uint64_t puzzle_1(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-1.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_1(file);
}

uint64_t puzzle_2(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-2.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_2(file);
}

uint64_t do_puzzle_1(std::ifstream &file) {
    Grid grid{};
    std::string line;

    while (std::getline(file, line)) {
        Row row{};

        for (auto &c: line) {
            row.emplace_back(c);
        }

        grid.emplace_back(row);
    }

    visited_tiles.clear();
    cached_calls.clear();

    uint64_t result = calculate_lit_tiles(grid, {0, 0}, {0, 1});

    uint64_t sum = 0;

    for (auto &col: grid) {
        for (auto &c: col) {
            char ch = c;

            if (visited_tiles.contains({&col - &grid[0], &c - &col[0]})) {
                ch = 'X';
                sum++;
            }
        }
    }

    return sum;
}

uint64_t do_puzzle_2(std::ifstream &file) {
    Grid grid{};
    std::string line;

    while (std::getline(file, line)) {
        Row row{};

        for (auto &c: line) {
            row.emplace_back(c);
        }

        grid.emplace_back(row);
    }

    visited_tiles.clear();
    cached_calls.clear();

    uint64_t sum = 0;

    std::vector<std::pair<Tile, Direction>> tiles{};

    size_t width = grid.at(0).size();
    size_t height = grid.size();

    for (size_t i = 0; i < width; i++) {
        tiles.emplace_back(std::make_pair<Tile, Direction>({0, i}, {1, 0}));
        tiles.emplace_back(std::make_pair<Tile, Direction>({height - 1, i}, {-1, 0}));
    }

    for (size_t i = 0; i < height; i++) {
        tiles.emplace_back(std::make_pair<Tile, Direction>({i, 0}, {0, 1}));
        tiles.emplace_back(std::make_pair<Tile, Direction>({i, width - 1}, {0, -1}));
    }

    for (auto &tile: tiles) {
        uint64_t result = 0;

        visited_tiles.clear();
        cached_calls.clear();

        calculate_lit_tiles(grid, tile.first, tile.second);

        for (auto &col: grid) {
            for (auto &c: col) {
                if (visited_tiles.contains({&col - &grid[0], &c - &col[0]})) {
                    result++;
                }
            }
        }

        sum = std::max(sum, result);
    }

    return sum;
}

const TileDeflections tile_deflections = {
        {
                '/',
                {
                        {{0, 1}, {{-1, 0}}}, // right -> up
                        {{0,  -1}, {{1,  0}}}, // left -> down
                        {{1, 0},  {{0, -1}}}, // down -> right
                        {{-1, 0}, {{0,  1}}} // up -> right
                }
        },
        {
                '\\',
                {
                        {{0, 1}, {{1,  0}}}, // right -> down
                        {{0,  -1}, {{-1, 0}}}, // left -> up
                        {{1, 0},  {{0, 1}}}, // down -> right
                        {{-1, 0}, {{0,  -1}}} // up -> left
                }
        },
        {
                '|',
                {
                        {{0, 1}, {{-1, 0}, {1, 0}}}, // right -> up & down
                        {{0,  -1}, {{-1, 0}, {1, 0}}}, // left -> up & down
                        {{1, 0},  {{1, 0}}}, // down -> down
                        {{-1, 0}, {{-1, 0}}} // up -> up
                }
        },
        {
                '-',
                {
                        {{1, 0}, {{0,  1}, {0, -1}}}, // down -> left & right
                        {{-1, 0},  {{0,  1}, {0, -1}}}, // up -> left & right
                        {{0, -1}, {{0, -1}}}, // left -> left
                        {{0,  1}, {{0,  1}}} // right -> right
                }
        },
};

std::map<Tile, int64_t> visited_tiles{};
std::map<std::pair<Tile, Direction>, uint64_t> cached_calls{};

uint64_t calculate_lit_tiles(const Grid &grid, const Tile &tile, const Direction &direction) {
    uint64_t sum = 0;

    auto start_col = grid.begin() + tile.first;
    auto start_row = start_col->begin() + tile.second;

    while (start_col <= grid.end() && start_col >= grid.begin() && start_row <= start_col->end() && start_row >= start_col->begin()) {
        if (!visited_tiles.contains({start_col - grid.begin(), start_row - start_col->begin()})) {
            sum++;
            visited_tiles.insert({{start_col - grid.begin(), start_row - start_col->begin()}, 1});
        } else {
            visited_tiles[{start_col - grid.begin(), start_row - start_col->begin()}]++;
        }

        if (*start_row != '.') {
            if (tile_deflections.contains(*start_row)) {
                auto &deflections = tile_deflections.at(*start_row).at(direction);

                for (auto &deflection: deflections) {
                    Tile new_tile{start_col - grid.begin() + deflection.first,
                                  start_row - start_col->begin() + deflection.second};

                    if (cached_calls.contains({new_tile, deflection})) {
//                        sum += cached_calls[{new_tile, deflection}];
                        continue;
                    } else {
                        cached_calls.insert({{new_tile, deflection}, 0});
                        uint64_t result = calculate_lit_tiles(grid, new_tile, deflection);

                        cached_calls[{new_tile, deflection}] = result;

                        sum += result;
                    }
                }

                break;
            }
        }

        Tile new_tile = {start_col - grid.begin() + direction.first, start_row - start_col->begin() + direction.second};

        if (new_tile.first >= grid.size()) {
            break;
        }

        start_col = grid.begin() + new_tile.first;

        if (new_tile.second >= start_col->size()) {
            break;
        }

        start_row = start_col->begin() + new_tile.second;
    }

    return sum;
}
