#ifndef HETROPOD_NUMERALS_H
#define HETROPOD_NUMERALS_H


#include <map>
#include <string>
#include <optional>

class Numerals
{
public:
    Numerals();

    std::u32string toNumeral(int dg) const;

private:
    std::map<int, std::u32string> numerals_;
};


#endif //HETROPOD_NUMERALS_H
