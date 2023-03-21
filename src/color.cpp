// 参考
// https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c
#include "color.h"

#include <iostream>
#include <iterator>
#include <map>

namespace color {
namespace {

std::string ToString(Color color) {
    // clang-format off
    std::map<Color, std::string> name_mapper {
        {Color::kBlack, "Black"},
        {Color::kDarkBlue, "DarkBlue"},
        {Color::kDarkGreen, "DarkGreen"},
        {Color::kLightBlue, "LightBlue"},
        {Color::kDarkRed, "DarkRed"},
        {Color::kMagenta, "Magenta"},
        {Color::kOrange, "Orange"},
        {Color::kLightGray, "LightGray"},
        {Color::kGray, "Gray"},
        {Color::kBlue, "Blue"},
        {Color::kGreen, "Green"},
        {Color::kCyan, "Cyan"},
        {Color::kRed, "Red"},
        {Color::kPink, "Pink"},
        {Color::kYellow, "Yellow"},
        {Color::kWhite, "White"},
        {Color::kNone, "None"},
    };
    // clang-format on
    return name_mapper[color];
}
}  // namespace

void ShowExample() {
    auto colors = {Color::kBlack,   Color::kDarkBlue, Color::kDarkGreen, Color::kLightBlue, Color::kDarkRed,
                   Color::kMagenta, Color::kOrange,   Color::kLightGray, Color::kGray,      Color::kBlue,
                   Color::kGreen,   Color::kCyan,     Color::kRed,       Color::kPink,      Color::kYellow,
                   Color::kWhite,   Color::kNone};
    putchar('\n');
    for (auto color : colors) { printf("%-12s", ToString(color).c_str()); }
    putchar('\n');
    for (auto fg : colors) {
        for (auto bg : colors) {
            Print(fg, bg, "hello,world");
            std::putchar(' ');
        }
        std::cout << std::endl;
    }
}

#ifdef _WIN32
#include <windows.h>

void Print(Color color, const std::string &line) {
    Print(color, Color::kBlack, line);
}

void Println(Color color, const std::string &line) {
    Print(color, line);
    std::cout << std::endl;
}

void Print(Color fg_color, Color bg_color, const std::string &line) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD color = (static_cast<WORD>(fg_color) & 0xf) | (static_cast<WORD>(bg_color) & 0xf) << 4;
    SetConsoleTextAttribute(hConsole, color);
    std::cout << line;
    SetConsoleTextAttribute(hConsole, static_cast<WORD>(Color::kLightGray));
}

void Println(Color fg_color, Color bg_color, const std::string &line) {
    Print(fg_color, bg_color, line);
    std::cout << std::endl;
}

#else

void Print(Color color, const std::string &s) {
    Print(color, Color::kNone, s);
}

void Println(Color color, const std::string &s) {
    Print(color, s);
    std::cout << std::endl;
}

void Print(Color fg_color, const Color bg_color, const std::string &s) {
    // clang-format off
    std::map<Color, std::string> bg_color_mapper{
            {Color::kBlack, "40"},
            {Color::kDarkBlue, "44"},
            {Color::kDarkGreen, "42"},
            {Color::kLightBlue, "46"},
            {Color::kDarkRed, "41"},
            {Color::kMagenta, "45"},
            {Color::kOrange, "43"},
            {Color::kLightGray, "47"},
            {Color::kGray, "100"},    
            {Color::kBlue, "104"}, 
            {Color::kGreen, "102"}, 
            {Color::kCyan, "106"},
            {Color::kRed, "101"}, 
            {Color::kPink, "105"},
            {Color::kYellow, "103"}, 
            {Color::kWhite, "107"},
            {Color::kNone, "37"},
    };
    std::map<Color, std::string> fg_color_mapper{
        {Color::kBlack, "30"},
        {Color::kDarkBlue, "34"},
        {Color::kDarkGreen, "32"},
        {Color::kLightBlue, "36"},
        {Color::kDarkRed, "31"},
        {Color::kMagenta, "35"},
        {Color::kOrange, "33"},
        {Color::kLightGray, "37"},
        {Color::kGray, "90"},
        {Color::kBlue, "94"},
        {Color::kGreen, "92"},
        {Color::kCyan, "96"},
        {Color::kRed, "91"},
        {Color::kPink, "95"},
        {Color::kYellow, "93"},
        {Color::kWhite, "97"},
        {Color::kNone, "30"},
    };
    // clang-format on

    auto col = bg_color == Color::kNone ? "\033[" + fg_color_mapper[fg_color] + "m"
                                        : "\033[" + fg_color_mapper[fg_color] + ";" + bg_color_mapper[bg_color] + "m";
    std::cout << col << s << "\033[0m";
}

void Println(Color fg_color, const Color bg_color, const std::string &s) {
    Print(fg_color, bg_color, s);
    std::cout << std::endl;
}

#endif
}  // namespace color
