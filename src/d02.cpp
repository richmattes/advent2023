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

#include <absl/strings/match.h>
#include <absl/strings/numbers.h>
#include <absl/strings/str_split.h>
#include <absl/strings/string_view.h>
#include <fmt/format.h>

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <tuple>
#include <vector>

namespace advent {

int get_game_id(const std::string_view& game_prefix)
{
    std::regex re("Game (\\d*)");
    auto result = std::regex_iterator(std::begin(game_prefix), std::end(game_prefix), re);
    return std::stoi((*result).str(1));
}

std::tuple<int, int, int> get_rgb(const std::string_view& set)
{
    int red=0, green=0, blue=0;
    std::vector<std::string_view> items = absl::StrSplit(set, ", ");
    for (const auto & item : items)
    {
        std::vector<absl::string_view> num_and_color = absl::StrSplit(item, ' ');
        if (absl::StrContains(num_and_color[1], "red")) {
            absl::SimpleAtoi(num_and_color[0], &red);
        }
        else if (absl::StrContains(num_and_color[1], "green")) {
            absl::SimpleAtoi(num_and_color[0], &green);
        } else {
            absl::SimpleAtoi(num_and_color[0], &blue);
        }
    }

    return std::make_tuple(red, green, blue);
}

int part_1(int red_limit, int green_limit, int blue_limit)
{
    const auto input_file_name = fmt::format("{}/{}", get_data_path(), "d02.txt");
    std::ifstream input_file(input_file_name);

    std::string line;
    int total = 0;
    while(input_file.peek() != EOF) {
        std::getline(input_file, line);

        std::vector<std::string_view> id_and_game = absl::StrSplit(line, ": ");
        auto game_id = get_game_id(id_and_game[0]);

        std::vector<std::string_view> set_list = absl::StrSplit(id_and_game[1], "; ");
        bool over = false;
        for(const auto & set : set_list) {
            int red, green, blue;
            std::tie(red, green, blue) = get_rgb(set);
            if (red > red_limit or green > green_limit or blue > blue_limit) {    
                over = true;
                break;
            }
        }
        if (not over) {
            total += game_id;
        }

    }
    return total;
}

int part_2()
{
    const auto input_file_name = fmt::format("{}/{}", get_data_path(), "d02.txt");
    std::ifstream input_file(input_file_name);

    std::string line;
    int total = 0;
    while(input_file.peek() != EOF) {
        std::getline(input_file, line);

        std::vector<std::string_view> id_and_game = absl::StrSplit(line, ": ");

        std::vector<std::string_view> set_list = absl::StrSplit(id_and_game[1], "; ");
        int min_red = 0, min_green = 0, min_blue = 0;
        for(const auto & set : set_list) {
            int red, green, blue;
            std::tie(red, green, blue) = get_rgb(set);

            min_red = std::max(min_red, red);
            min_green = std::max(min_green, green);
            min_blue = std::max(min_blue, blue);

        }
        auto power = min_red * min_green * min_blue;
        total += power;

    }
    return total;
}

}

int main (int, char**)
{
    std::cout << "Total: " << advent::part_1(12,13,14) << std::endl;
    std::cout << "Total power: " << advent::part_2() << std::endl;
    return 0;
}
