//
// Created by Vlad Isan on 05/12/2023.
//

#ifndef PUZZLE_H
#define PUZZLE_H

#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "fmt/core.h"

using Direction = std::pair<int64_t, int64_t>;
using Tile = std::pair<int64_t, int64_t>;
using Row = std::vector<char>;
using Grid = std::vector<Row>;
using TileDeflections = std::map<char, std::map<Direction, std::vector<Direction>>>;

extern const TileDeflections tile_deflections;
extern std::map<std::pair<Tile, Direction>, uint64_t> cached_calls;

extern std::map<Tile, int64_t> visited_tiles;

uint64_t puzzle_sample_1(const std::string &base_file_path);

uint64_t puzzle_sample_2(const std::string &base_file_path);

uint64_t puzzle_1(const std::string &base_file_path);

uint64_t puzzle_2(const std::string &base_file_path);

uint64_t do_puzzle_1(std::ifstream &file);

uint64_t do_puzzle_2(std::ifstream &file);

uint64_t calculate_lit_tiles(const Grid &grid, const Tile &tile, const Direction &direction);

#endif //PUZZLE_H
