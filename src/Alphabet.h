#ifndef HETROPOD_ALPHABET_H
#define HETROPOD_ALPHABET_H


#include <string>
#include <vector>
#include <optional>

enum JamoType
{
    Initial,
    Medial,
    Final
};

class Char
{
public:
    Char(char16_t key, std::u32string jamo);
    Char(std::u32string parent, char16_t key, std::u32string jamo);

    char16_t key;
    std::u32string jamo;
    std::optional<std::u32string> parent;
};

class Alphabet
{
    public:
    Alphabet();

    std::optional<Char> toChar(char16_t key, JamoType type) const;
    Char soundlessInitial() const;
    std::optional<Char> toCharCluster(std::u32string parent, char16_t key, JamoType type) const;
    Char soundlessVowel() const;
    std::optional<Char> toParent(const Char &ch, JamoType type) const;

private:
    std::vector<Char> initials_;
    std::vector<Char> medials_;
    std::vector<Char> finals_;

    std::vector<Char> initial_clusters_;
    std::vector<Char> medial_clusters_;
    std::vector<Char> final_clusters_;

    Char soundless_initial_;
    Char soundless_vowel_;
};


#endif //HETROPOD_ALPHABET_H
