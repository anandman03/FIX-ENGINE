#pragma once

#include <iostream>
#include <map>
#include <sstream>

namespace book {

enum class Side {
    Bid = 0,
    Ask = 1,
};

struct BestEntry {
    long double price;
    uint64_t volume;

    std::string to_string();
};

class L1Book {
private:
    const int m_depth;

    BestEntry best_bid; // highest
    std::multimap<long double, int> bids;

    std::multimap<long double, int> asks;
    BestEntry best_ask; // lowest

    bool add_bid(const long double& price_level, const uint64_t& vol_level);
    bool add_ask(const long double& price_level, const uint64_t& vol_level);

public:
    L1Book(const int& depth = 10);

    std::string top_of_book();
    bool update_from_quote(const Side& quote_side, const long double& price_level, const uint64_t& vol_level);

    std::string to_string();
};

};