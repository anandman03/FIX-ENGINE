#include "src/fix_parser.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::filesystem::path file_path("messages.txt");
    if (!std::filesystem::exists(file_path)) {
        std::cout << "Cannot find file: " << file_path << std::endl;
        return 0;
    }

    std::fstream file_stream(file_path, std::ios::in);
    if (!file_stream.is_open()) {
        std::cout << "Cannot open file: " << file_path << std::endl;
        return 0;
    }

    std::string line;
    int book_levels = 5;
    parser::FixParser parser(book_levels);
    while (file_stream >> line) {
        parser.set_message(line.c_str());
        parser.build_book();
    }

    return 0;
}
