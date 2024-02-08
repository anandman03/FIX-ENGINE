#include "book.h"

namespace book {

L1Book::L1Book(const int& depth) : m_depth(depth), best_bid({.price = INT_MIN, .volume = 0}), best_ask({.price = INT_MAX, .volume = 0}) {}

std::string BestEntry::to_string() {
    std::stringstream ss;
    ss << "price [" << price << "] volume [" << volume << "]\n";
    return ss.str();
}

bool L1Book::add_bid(const long double& price_level, const uint64_t& vol_level) {
    if (vol_level > 0 && (!best_bid.volume || best_bid.price < price_level)) {
        best_bid.volume = vol_level;
        best_bid.price = price_level;
    }

    bids.insert({price_level, vol_level});
    if (bids.size() > m_depth) {
        bids.erase(bids.begin()->first);
    }
    
    return true;
}

bool L1Book::add_ask(const long double& price_level, const uint64_t& vol_level) {
    if (vol_level > 0 && (!best_ask.volume || best_ask.price > price_level)) {
        best_ask.volume = vol_level;
        best_ask.price = price_level;
    }

    asks.insert({price_level, vol_level});
    if (asks.size() > m_depth) {
        asks.erase(asks.rbegin()->first);
    }

    return true;
}

bool L1Book::update_from_quote(const Side& quote_side, const long double& price_level, const uint64_t& vol_level) {
    switch (quote_side) {
        case Side::Ask: add_ask(price_level, vol_level); break;
        case Side::Bid: add_bid(price_level, vol_level); break;
        default: return false;
    }
    
    return true;
}

std::string L1Book::top_of_book() {
    std::stringstream ss;
    ss << "BEST BID" << '\n' << best_bid.to_string();
    ss << "BEST ASK" << '\n' << best_ask.to_string();
    return ss.str();
}

std::string L1Book::to_string() {
    std::stringstream ss;
    ss << "Ask Side" << std::endl;
    for (auto itr : asks) {
        ss << "price [" << itr.first << " ] volume [" << itr.second << "]\n";
    }
    ss << "Bid Side" << std::endl;
    for (auto itr = bids.rbegin() ; itr != bids.rend() ; ++itr) {
        ss << "price [" << itr->first << " ] volume [" << itr->second << "]\n";
    }
    return ss.str();
}

} // namespace book