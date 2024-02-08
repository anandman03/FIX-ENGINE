#pragma once

#include "book.h"

#include <iostream>
#include <string>
#include <tuple>

namespace parser {

class FixParser {
private:
    book::L1Book l1_book;

    const char* m_begin = nullptr;
    const char* m_stream = nullptr;

    int16_t field_tag = 0;
    bool equal_flag = false;
    std::string field_value {};
    const char delimiter = '|';

    struct OrderInfo {
        long double price;
        uint64_t volume;
        book::Side order_side;
    };

    enum MandatoryOrderFields {
        PRICE,
        VOLUME,
        ORDER_SIDE,
    };
    const uint32_t MANDATORY_FIELDS = (1 << MandatoryOrderFields::PRICE) | (1 << MandatoryOrderFields::VOLUME) | (1 << MandatoryOrderFields::ORDER_SIDE);

public:
    FixParser(const int& book_levels = 10);

    void set_message(const char* input_stream);

    void reset_variables();
    std::tuple<bool, std::string> find_field(const int& field, bool start_over = true);

    void advance();
    bool is_end();

    bool build_book();
};

}; // namespace parser