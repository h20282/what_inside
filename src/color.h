#pragma once

#include <string>

namespace color {

enum Color : uint16_t {
    kBlack = 0,
    kDarkBlue = 1,
    kDarkGreen = 2,
    kLightBlue = 3,
    kDarkRed = 4,
    kMagenta = 5,
    kOrange = 6,
    kLightGray = 7,
    kGray = 8,
    kBlue = 9,
    kGreen = 10,
    kCyan = 11,
    kRed = 12,
    kPink = 13,
    kYellow = 14,
    kWhite = 15,
    kNone = 16,
};

void ShowExample();

void Print(Color color, const std::string &s);

void Println(Color color, const std::string &s);

void Print(Color fg_color, const Color bg_color, const std::string &s);

void Println(Color fg_color, const Color bg_color, const std::string &s);

}  // namespace color
