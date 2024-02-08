#include "fix_parser.h"

namespace parser {

FixParser::FixParser(const int& book_levels) : l1_book(book_levels) {}

void FixParser::set_message(const char* input_stream) {
    m_stream = input_stream;
    m_begin = input_stream;
}

void FixParser::reset_variables() {
    field_tag = 0;
    field_value = "";
    equal_flag = false;
}

std::tuple<bool, std::string> FixParser::find_field(const int& field, bool start_over) {
    if (start_over) {
        m_begin = m_stream;
    }

    while (!is_end()) {
        advance();
        if (field_tag == field) {
            return std::tuple(true, field_value);
        }
    }

    return std::tuple(false, "");
}

void FixParser::advance() {
    reset_variables();

    while (*m_begin != delimiter) {
        if (*m_begin == '=') {
            equal_flag = !equal_flag;
        } else if (equal_flag) {
            field_value += *m_begin;
        } else {
            field_tag = (field_tag * 10) + (*m_begin - '0');
        }

        ++m_begin;
    }
    // to pass the delimiter '|'
    if (!is_end()) ++m_begin;
}

bool FixParser::is_end() {
    return (*m_begin == '\0');
}

bool FixParser::build_book() {
    auto [res, value] = find_field(262);
    if (!res) {
        std::cout << "cannot parse FIX tag: 262" << std::endl;
        return false;
    }
    std::cout << "FIX tag: 262 [" << value << "]" << std::endl;

    std::tie(res, value) = find_field(268, false);
    if (!res) {
        std::cout << "cannot parse FIX tag: 268" << std::endl;
        return false;
    }
    std::cout << "FIX tag: 268 [" << value << "]" << std::endl;
    const uint32_t total_entries = std::stoul(value);

    for (int entry = 0 ; entry < total_entries ; ++entry) {

        OrderInfo order_info;
        uint32_t current_group = 0;
        while (!is_end() && current_group != MANDATORY_FIELDS) {
            advance();

            switch (field_tag) {
                case 270: {
                    if ((current_group & (1 << MandatoryOrderFields::PRICE)) == (1 << MandatoryOrderFields::PRICE)) {
                        std::cout << "Duplicate fields: 269" << std::endl;
                        return false;
                    }

                    order_info.price = std::stold(field_value);
                    current_group |= (1 << MandatoryOrderFields::PRICE);
                } break;
                case 271: {
                    if ((current_group & (1 << MandatoryOrderFields::VOLUME)) == (1 << MandatoryOrderFields::VOLUME)) {
                        std::cout << "Duplicate fields: 271" << std::endl;
                        return false;
                    }

                    order_info.volume = std::stoull(field_value);
                    current_group |= (1 << MandatoryOrderFields::VOLUME);
                } break;
                case 269: {
                    if ((current_group & (1 << MandatoryOrderFields::ORDER_SIDE)) == (1 << MandatoryOrderFields::ORDER_SIDE)) {
                        std::cout << "Duplicate fields: 269" << std::endl;
                        return false;
                    }

                    order_info.order_side = static_cast<book::Side>(std::stod(field_value));
                    current_group |= (1 << MandatoryOrderFields::ORDER_SIDE);
                } break;
                default: break;
            }
        }

        if (current_group == MANDATORY_FIELDS) {
            l1_book.update_from_quote(order_info.order_side, order_info.price, order_info.volume);
        }
    }

    std::cout << l1_book.top_of_book();
    std::cout << l1_book.to_string() << '\n';

    return true;
}

}; // namespace parser