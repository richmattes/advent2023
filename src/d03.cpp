// advent2023
// Copyright (C) 2023 Rich Mattes
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <advent2023/advent_data.hpp>

#include <fmt/format.h>

#include <fstream>
#include <iostream>
#include <ranges>
#include <tuple>
#include <vector>

namespace advent {

struct part_info {
    int value = 0;
    int start_col = -1;
    int end_col = -1;
    bool connected = false;
};

struct symbol_info {
    int col;
    char type;
};

using symbol_list = std::vector<symbol_info>;
using part_list = std::vector<part_info>;

using schematic_row = std::pair<part_list, symbol_list>;
using schematic = std::vector<schematic_row>;

bool is_adjacent(const part_info& part, const symbol_info& symbol){
    return part.start_col == symbol.col + 1 or part.end_col == symbol.col - 1 or (part.start_col <= symbol.col and part.end_col >= symbol.col);
}

schematic_row parse_row(const std::string_view& input_row)
{
    schematic_row row;

    part_info current_part;

    for (const auto [i,value] : std::views::enumerate(input_row))
    {
        if (value >= '0' and value <= '9')
        {
            current_part.start_col = current_part.start_col < 0 ? i : current_part.start_col;
            current_part.end_col = i;
            current_part.value = current_part.value * 10 + int(value - '0');
        } else {
            if(current_part.start_col >= 0) {
                row.first.push_back(current_part);
                current_part = part_info();
            }
            if (value != '.') {
                row.second.push_back({.col = (int)i, .type = value});
            }
        }
    }
    if(current_part.start_col >= 0) {
        row.first.push_back(current_part);
    }

    return row;
}

schematic parse_schematic(const std::string& input_file_name)
{
    schematic output;

    std::ifstream input_file(input_file_name);
    while(input_file.peek() != EOF) {
        std::string line;
        std::getline(input_file, line);
        auto row = parse_row(line);
        output.push_back(row);
    }

    return output;
}

void mark_connected_part_infos(schematic& schematic)
{
    schematic_row blank_row;
    for (size_t i = 0; i < schematic.size(); ++i) {
        const std::vector<schematic_row*> adjacent_rows{
            &schematic[i],
            (i+1) < schematic.size() ? &schematic[i+1] : &blank_row,
            i > 0 ? &schematic[i-1] : &blank_row
        };

        for (const auto & symbol : schematic[i].second) {
            for (const auto& adjacent_row : adjacent_rows){
                for(auto& part : adjacent_row->first) {
                    if (is_adjacent(part, symbol)) {
                        part.connected = true;
                    }
                }
            }
        }
    }
}

int total_connected_part_infos(const schematic& schematic)
{
    int total = 0;
    for (const auto& row : schematic){
        auto& parts = row.first;
        for (const auto& part : parts) {
            if (part.connected) {
                total += part.value * part.connected;
            }
        }
    }
    return total;
}

int total_gear_ratios(const schematic& schematic) 
{
    int total = 0;
    schematic_row blank_row;
    for (size_t i = 0; i < schematic.size(); ++i) {
        const std::vector<const schematic_row*> adjacent_rows{
            &schematic[i],
            (i+1) < schematic.size() ? &schematic[i+1] : &blank_row,
            i > 0 ? &schematic[i-1] : &blank_row
        };
        int count = 0;
        int product = 1;
        for (const auto & symbol : schematic[i].second) {
            if (symbol.type != '*') { continue; }
            for (const auto& adjacent_row : adjacent_rows){
                for(auto& part : adjacent_row->first) {
                    if (is_adjacent(part, symbol)) {
                        count++;
                        product *= part.value;
                    }
                }
            }
        
            if (count == 2) {
                total += product;
            }
            count = 0; product = 1;
        }
    }
    return total;
}

int part_1()
{
    const std::string input_file_name = fmt::format("{}/{}", get_data_path(), "d03.txt");
    auto schematic = parse_schematic(input_file_name);
    mark_connected_part_infos(schematic);
    return total_connected_part_infos(schematic);
}

int part_2() 
{
    const std::string input_file_name = fmt::format("{}/{}", get_data_path(), "d03.txt");
    auto schematic = parse_schematic(input_file_name);
    return total_gear_ratios(schematic);
}
}
int main (int, char**)
{
    std::cout << "Part 1: " << advent::part_1() << std::endl;
    std::cout << "Part 2: " << advent::part_2() << std::endl;
}