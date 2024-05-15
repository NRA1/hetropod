#include <algorithm>
#include "Keyboard.h"

Keyboard::Keyboard()
{
    keys_[u'p'] = SoundType::Consonant; // /p/
    keys_[u'b'] = SoundType::Consonant; // /b/
    keys_[u'f'] = SoundType::Consonant; // /f/
    keys_[u'v'] = SoundType::Consonant; // /v/
    keys_[u'm'] = SoundType::Consonant; // /m/
    keys_[u't'] = SoundType::Consonant; // /t/
    keys_[u'T'] = SoundType::Consonant; // /θ/
    keys_[u'd'] = SoundType::Consonant; // /d/
    keys_[u'D'] = SoundType::Consonant; // /ð/
    keys_[u'n'] = SoundType::Consonant; // /n/
    keys_[u'N'] = SoundType::Consonant; // /ŋ/
    keys_[u'k'] = SoundType::Consonant; // /k/
    keys_[u'g'] = SoundType::Consonant; // /g/
    keys_[u's'] = SoundType::Consonant; // /s/
    keys_[u'z'] = SoundType::Consonant; // /z/
    keys_[u'š'] = SoundType::Consonant; // /ʃ/
    keys_[u'ž'] = SoundType::Consonant; // /ʒ/
    keys_[u'č'] = SoundType::Consonant; // /t͡ʃ/
    keys_[u'j'] = SoundType::Consonant; // /d͡ʒ/
    keys_[u'w'] = SoundType::Consonant; // /w/
    keys_[u'r'] = SoundType::Consonant; // /ɹ/
    keys_[u'l'] = SoundType::Consonant; // /l/
    keys_[u'y'] = SoundType::Consonant; // /j/
    keys_[u'h'] = SoundType::Consonant; // /h/

    keys_[u'A'] = SoundType::Vowel; // /a/~/ʌ/
    keys_[u'a'] = SoundType::Vowel; // /æ/
    keys_[u'E'] = SoundType::Vowel; // /ɛ/
    keys_[u'e'] = SoundType::Vowel; // /ə/~/ɜ/
    keys_[u'I'] = SoundType::Vowel; // /ɪ/
    keys_[u'i'] = SoundType::Vowel; // /iː/
    keys_[u'O'] = SoundType::Vowel; // /ɔ/
    keys_[u'o'] = SoundType::Vowel; // /ɔː/
    keys_[u'U'] = SoundType::Vowel; // /ʊ/
    keys_[u'u'] = SoundType::Vowel; // /uː/
    keys_[u'-'] = SoundType::Vowel; // vowel placeholder

    exclusive_initials_.emplace_back(u'T');
    exclusive_initials_.emplace_back(u'D');
    exclusive_initials_.emplace_back(u'ž');
}

std::optional<SoundType> Keyboard::findType(char16_t key) const
{
    auto it = keys_.find(key);
    if(it != keys_.end()) return std::optional(it->second);
    else return std::nullopt;
}

bool Keyboard::isExclusiveInitial(char16_t key) const
{
    auto it = std::find(exclusive_initials_.begin(), exclusive_initials_.end(), key);
    return it != exclusive_initials_.end();
}
