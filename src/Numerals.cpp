#include <fcitx-utils/log.h>
#include "Numerals.h"

Numerals::Numerals()
{
    numerals_[0] = U"〇";
    numerals_[1] = U"一";
    numerals_[2] = U"二";
    numerals_[3] = U"三";
    numerals_[4] = U"四";
    numerals_[5] = U"五";
    numerals_[6] = U"六";
    numerals_[7] = U"七";
    numerals_[8] = U"八";
    numerals_[9] = U"九";
}

std::u32string Numerals::toNumeral(int dg) const
{
    auto it = numerals_.find(dg);
    if(it == numerals_.end())
    {
        FCITX_WARN() << "Invalid digit: " << dg;
        return U"";
    }
    return it->second;
}

