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
#include <regex>
#include <string>
#include <unordered_map>

namespace advent {

std::unordered_map<std::string, int> str_to_int{
    {"one", 1}, {"two", 2}, {"three", 3},
    {"four", 4}, {"five", 5}, {"six", 6},
    {"seven", 7}, {"eight", 8}, {"nine",9}};

int string_to_value(const std::string& input)
{
    if (input.size() == 1) {
        return std::stoi(input);
    } else {
        return str_to_int[input];
    }
}

int get_calibration_value(const std::string& input, bool find_alpha_numbers = false) {
    const std::string match_string = find_alpha_numbers ? "(?=(one|two|three|four|five|six|seven|eight|nine|\\d))" : "(\\d)"; 
    const std::regex re(match_string, std::regex_constants::ECMAScript);
    const auto begin = std::sregex_iterator(std::begin(input), std::end(input), re);
    auto end = begin;
    while (std::next(end) != std::sregex_iterator()) {
        ++end;
    }

    const int first_digit = string_to_value((*begin).str(1));
    const int last_digit = string_to_value((*end).str(1));

    return first_digit * 10 + last_digit;
}

int compute_total(bool find_alpha_numbers = false)
{
    const auto input_file_path = fmt::format("{}/{}", get_data_path(), "d01.txt");

    auto input_file = std::ifstream(input_file_path);

    std::string line;
    int total = 0;
    while (input_file.peek() != EOF) {
        std::getline(input_file, line);
        total += get_calibration_value(line, find_alpha_numbers);
    }

    return total;
}
};

int main (int /*argc*/, char** /*argv*/)
{
    std::cout << "Total: " << advent::compute_total() << std::endl;
    std::cout << "Total 2: " << advent::compute_total(true) << std::endl;
    return 0;
}
