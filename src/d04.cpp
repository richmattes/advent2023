#include <advent2023/advent_data.hpp>

#include <absl/strings/numbers.h>
#include <absl/strings/str_split.h>
#include <fmt/format.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <list>
#include <numeric>
#include <set>
#include <string>
#include <ranges>
#include <vector>

namespace advent {

using game = std::pair<std::set<int>, std::set<int>>;
using game_list = std::vector<game>;

game parse_game(const std::string_view& line)
{
    std::set<int> winning_numbers, have_numbers;
    const std::vector<std::string_view> header_and_results = absl::StrSplit(line, ": ");

    const std::vector<std::string_view> winner_and_values = absl::StrSplit(header_and_results[1], " | ");

    const std::vector<std::string_view> winners = absl::StrSplit(winner_and_values[0], ' ', absl::SkipEmpty());
    for (const auto & winner : winners) {
        int winner_val = 0;
        if (absl::SimpleAtoi(winner, &winner_val)) {
            winning_numbers.emplace(winner_val);
        }
    }

    const std::vector<std::string_view> values = absl::StrSplit(winner_and_values[1], ' ' , absl::SkipEmpty());
    for (const auto & value : values) {
        int result = 0;
        if (absl::SimpleAtoi(value, &result)) {
            have_numbers.emplace(result);
        }
    }

    return std::make_pair(winning_numbers, have_numbers);

}

game_list parse_games(const std::string& input_file_name)
{
    std::ifstream input_file(input_file_name);
    game_list result;

    std::string line;
    while(input_file.peek() != EOF)
    {
        std::getline(input_file, line);
        result.push_back(parse_game(line));
    }

    return result;
}

std::vector<int> get_scores(const game_list& input, bool exponential = true)
{
    std::vector<int> scores;
    for (const auto& game : input) {
        std::vector<int> intersection;
        std::set_intersection(std::begin(game.first), std::end(game.first),
            std::begin(game.second), std::end(game.second), std::back_inserter(intersection));

        if (exponential) {
            if (intersection.size() > 0) {
                scores.push_back(std::pow(2, intersection.size() - 1));
            } else {
                scores.push_back(0);
            }
        } else {
            scores.push_back(intersection.size());
        }
    }
    return scores;
}

int part_1()
{
    auto input_file_name = fmt::format("{}/{}", get_data_path(), "d04.txt");
    auto game_list = parse_games(input_file_name);

    auto scores = get_scores(game_list);
    int total = std::accumulate(std::begin(scores), std::end(scores), 0);
    return total;
}

int part_2() 
{
    auto input_file_name = fmt::format("{}/{}", get_data_path(), "d04.txt");
    auto game_list = parse_games(input_file_name);

    auto scores = get_scores(game_list, false);
    std::vector<int> card_count(scores.size(), 1);

    for (const auto [i, value] : std::views::enumerate(card_count))
    {
        auto score = scores[i];
        for(int card_copy = 0; card_copy < value; ++card_copy) 
        {
            for(int j = 1; j <= score; ++j)
            {
                card_count[i + j]++;
            }
        }
    }
    return std::accumulate(std::begin(card_count), std::end(card_count), 0);
}
}

int main (int , char**)
{
    std::cout << "Part 1: " << advent::part_1() << std::endl;
    std::cout << "Part 2: " << advent::part_2() << std::endl;
    return 0;
}