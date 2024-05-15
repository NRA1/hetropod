#ifndef HETROPOD_KEYBOARD_H
#define HETROPOD_KEYBOARD_H


#include <map>
#include <string>
#include <optional>
#include <vector>

enum SoundType
{
    Consonant,
    Vowel
};

class Keyboard
{
public:
    Keyboard();

    std::optional<SoundType> findType(char16_t key) const;
    bool isExclusiveInitial(char16_t key) const;

private:
    std::map<char16_t, SoundType> keys_;
    std::vector<char16_t> exclusive_initials_;
};


#endif //HETROPOD_KEYBOARD_H
