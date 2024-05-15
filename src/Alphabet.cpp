#include "Alphabet.h"

#include <utility>
#include <algorithm>

Char::Char(char16_t k, std::u32string j)
{
    key = k;
    jamo = std::move(j);
}

Char::Char(std::u32string p, char16_t k, std::u32string j)
{
    this->parent = std::optional( std::move(p));
    this->key = k;
    this->jamo = std::move(j);
}

Alphabet::Alphabet() : soundless_initial_(u'N', U"ᄋ"), soundless_vowel_(u'-', U"ᅳ")
{
    initials_.emplace_back(u'p', U"ᄑ"); // /p/
    initials_.emplace_back(u'b', U"ᄇ"); // /b/
    initials_.emplace_back(u'f', U"ᅗ"); // /f/
    initials_.emplace_back(u'v', U"ᄫ"); // /v/
    initials_.emplace_back(u'm', U"ᄆ"); // /m/
    initials_.emplace_back(u't', U"ᄐ"); // /t/
    initials_.emplace_back(u'T', U"ᅊ"); // /θ/
    initials_.emplace_back(u'd', U"ᄃ"); // /d/
    initials_.emplace_back(u'D', U"ᅂ"); // /ð/
    initials_.emplace_back(u'n', U"ᄂ"); // /n/
    initials_.emplace_back(soundless_initial_); // /ŋ/
    initials_.emplace_back(u'k', U"ᄏ"); // /k/
    initials_.emplace_back(u'g', U"ᄀ"); // /g/
    initials_.emplace_back(u's', U"ᄉ"); // /s/
    initials_.emplace_back(u'z', U"ᅀ"); // /z/
    initials_.emplace_back(u'š', U"ᄊ"); // /ʃ/
    initials_.emplace_back(u'ž', U"ᅈ"); // /ʒ/
    initials_.emplace_back(u'č', U"ᄎ"); // /t͡ʃ/
    initials_.emplace_back(u'j', U"ᄌ"); // /d͡ʒ/
    initials_.emplace_back(u'w', U"ᄝ"); // /w/
    initials_.emplace_back(u'r', U"ᄅ"); // /ɹ/
    initials_.emplace_back(u'l', U"ᄙ"); // /l/
    initials_.emplace_back(u'y', U"ᅇ"); // /j/
    initials_.emplace_back(u'h', U"ᄒ"); // /h/


    medials_.emplace_back(u'A', U"ᅡ"); // /a/~/ʌ/
    medials_.emplace_back(u'a', U"ힹ"); // /æ/
    medials_.emplace_back(u'E', U"ᅦ"); // /ɛ/
    medials_.emplace_back(u'e', U"ᆞ"); // /ə/~/ɜ/
    medials_.emplace_back(u'I', U"ᅵ"); // /ɪ/
    medials_.emplace_back(u'i', U"ᅴ"); // /iː/
    medials_.emplace_back(u'O', U"ᅥ"); // /ɔ/
    medials_.emplace_back(u'o', U"ힼ"); // /ɔː/
    medials_.emplace_back(u'U', U"ᅮ"); // /ʊ/
    medials_.emplace_back(u'u', U"ᆕ"); // /uː/
    medials_.emplace_back(soundless_vowel_); // vowel placeholder

    finals_.emplace_back(u'p', U"ᇁ"); // /p/
    finals_.emplace_back(u'b', U"ᆸ"); // /b/
    finals_.emplace_back(u'f', U"ᇴ"); // /f/
    finals_.emplace_back(u'v', U"ᇦ"); // /v/
    finals_.emplace_back(u'm', U"ᆷ"); // /m/
    finals_.emplace_back(u't', U"ᇀ"); // /t/
    finals_.emplace_back(u'd', U"ᆮ"); // /d/
    finals_.emplace_back(u'n', U"ᆫ"); // /n/
    finals_.emplace_back(u'N', U"ᆼ"); // /ŋ/
    finals_.emplace_back(u'k', U"ᆿ"); // /k/
    finals_.emplace_back(u'g', U"ᆨ"); // /g/
    finals_.emplace_back(u's', U"ᆺ"); // /s/
    finals_.emplace_back(u'z', U"ᇫ"); // /z/
    finals_.emplace_back(u'š', U"ᆻ"); // /ʃ/
    finals_.emplace_back(u'č', U"ᆾ"); // /t͡ʃ/
    finals_.emplace_back(u'j', U"ᆽ"); // /d͡ʒ/
    finals_.emplace_back(u'w', U"ᇢ"); // /w/
    finals_.emplace_back(u'r', U"ᆯ"); // /ɹ/
    finals_.emplace_back(u'l', U"ᇐ"); // /l/
    finals_.emplace_back(u'y', U"ᇮ"); // /j/
    finals_.emplace_back(u'h', U"ᇂ"); // /h/

    initial_clusters_.emplace_back(U"ᄃ", u'r', U"ᅞ"); // /dɹ/
    initial_clusters_.emplace_back(U"ᄉ", u'p', U"ᄺ"); // /sp/
    initial_clusters_.emplace_back(U"ᄉ", u'm', U"ᄱ"); // /sm/
    initial_clusters_.emplace_back(U"ᄉ", u't', U"ᄹ"); // /st/
    initial_clusters_.emplace_back(U"ᄉ", u'n', U"ᄮ"); // /sn/
    initial_clusters_.emplace_back(U"ᄉ", u'k', U"ᄸ"); // /sk/

    medial_clusters_.emplace_back(U"ᅡ", u'I', U"ᅢ"); // /aɪ/
    medial_clusters_.emplace_back(U"ᅡ", u'U', U"ᅶ"); // /aʊ/
    medial_clusters_.emplace_back(U"ᅥ", u'I', U"ᅬ"); // /ɔɪ/
    medial_clusters_.emplace_back(U"ᅵ", u'e', U"ᆝ"); // /ɪə/
    medial_clusters_.emplace_back(U"ᅮ", u'e', U"ᆠ"); // /ʊə/
    medial_clusters_.emplace_back(U"ᅥ", u'U', U"ᅩ"); // /oʊ/
    medial_clusters_.emplace_back(U"ᅇ", u'u', U"ᅲ"); // /ju/
    medial_clusters_.emplace_back(U"ᅇ", u'A', U"ᅣ"); // /ja/~/jʌ/
    medial_clusters_.emplace_back(U"ᅇ", u'E', U"ᅨ"); // /jɛ/
    medial_clusters_.emplace_back(U"ᅇ", u'O', U"ᅧ"); // /jɔ/
    medial_clusters_.emplace_back(U"ᅇ", u'e', U"ᆢ"); // /jə/~/jɜ/
    medial_clusters_.emplace_back(U"ᅣ", u'I', U"ᅤ"); // /jaɪ/
    medial_clusters_.emplace_back(U"ᅣ", u'U', U"ᅸ"); // /jaʊ/
    medial_clusters_.emplace_back(U"ᅧ", u'I', U"ᆈ"); // /jɔɪ/
    medial_clusters_.emplace_back(U"ᅧ", u'U', U"ᅭ"); // /joʊ/
    medial_clusters_.emplace_back(U"ᄝ", u'A', U"ᅪ"); // /wa/
    medial_clusters_.emplace_back(U"ᄝ", u'E', U"ᅰ"); // /wɛ/
    medial_clusters_.emplace_back(U"ᄝ", u'I', U"ᅱ"); // /wɪ/
    medial_clusters_.emplace_back(U"ᄝ", u'O', U"ᅯ"); // /wɔ/
    medial_clusters_.emplace_back(U"ᅪ", u'I', U"ᅫ"); // /waɪ/

    final_clusters_.emplace_back(U"ᆿ", u's', U"ᆪ"); // /ks/~/gz/
    final_clusters_.emplace_back(U"ᆨ", u's', U"ᆪ"); // /ks/~/gz/
    final_clusters_.emplace_back(U"ᆺ", u'p', U"ᇪ"); // /sp/
    final_clusters_.emplace_back(U"ᆺ", u't', U"ᇨ"); // /st/
    final_clusters_.emplace_back(U"ᆺ", u'k', U"ᇧ"); // /sk/

    final_clusters_.emplace_back(U"ᇁ", u's', U"ퟺ"); // /ps/
    final_clusters_.emplace_back(U"ᆸ", u's', U"ᆹ"); // /bs/
    final_clusters_.emplace_back(U"ᆷ", u'b', U"ᇜ"); // /mb/
    final_clusters_.emplace_back(U"ᇜ", u's', U"ퟡ"); // /mbs/
    final_clusters_.emplace_back(U"ᆷ", u'n', U"ퟞ"); // /mn/
    final_clusters_.emplace_back(U"ᆷ", u's', U"ᇝ"); // /ms/
    final_clusters_.emplace_back(U"ᆮ", u's', U"ퟐ"); // /ds/
    final_clusters_.emplace_back(U"ᆫ", u't', U"ᇉ"); // /nt/
    final_clusters_.emplace_back(U"ᆫ", u'd', U"ᇆ"); // /nd/
    final_clusters_.emplace_back(U"ᆫ", u's', U"ᇇ"); // /ns/
    final_clusters_.emplace_back(U"ᆫ", u'č', U"ퟌ"); // /nt͡ʃ/
    final_clusters_.emplace_back(U"ᆼ", u'k', U"ᇯ"); // /ŋk/
    final_clusters_.emplace_back(U"ᆼ", u's', U"ᇱ"); // /ŋs/
    final_clusters_.emplace_back(U"ᆺ", u't', U"ퟱ"); // /st/
    final_clusters_.emplace_back(U"ᆯ", u'p', U"ᆵ"); // /ɹp/
    final_clusters_.emplace_back(U"ᆯ", u'b', U"ᆲ"); // /ɹp/
    final_clusters_.emplace_back(U"ᆲ", u's', U"ᇓ"); // /ɹbs/
    final_clusters_.emplace_back(U"ᆯ", u'v', U"ᇕ"); // /ɹv/
    final_clusters_.emplace_back(U"ᆯ", u'm', U"ᆱ"); // /ɹm/
    final_clusters_.emplace_back(U"ᆱ", u's', U"ᇒ"); // /ɹms/
    final_clusters_.emplace_back(U"ᆯ", u't', U"ᆴ"); // /ɹt/
    final_clusters_.emplace_back(U"ᆯ", u'd', U"ᇎ"); // /ɹd/
    final_clusters_.emplace_back(U"ᆯ", u'n', U"ᇍ"); // /ɹn/
    final_clusters_.emplace_back(U"ᆯ", u'k', U"ᇘ"); // /ɹk/
    final_clusters_.emplace_back(U"ᆯ", u'g', U"ᆰ"); // /ɹg/
    final_clusters_.emplace_back(U"ᆰ", u's', U"ᇌ"); // /ɹgs/
    final_clusters_.emplace_back(U"ᆯ", u's', U"ᆳ"); // /ɹs/
    final_clusters_.emplace_back(U"ᆯ", u'š', U"ᇖ"); // /ɹʃ/
    final_clusters_.emplace_back(U"ᇐ", u'k', U"ퟗ"); // /lk/
}

std::optional<Char> Alphabet::toChar(char16_t key, JamoType type) const
{
    if(type == JamoType::Initial)
    {
        auto it = std::find_if(initials_.begin(), initials_.end(), [key](const Char &c)
        { return c.key == key; });
        if (it != initials_.end()) return std::optional(*it);
    } else if(type == JamoType::Medial)
    {
        auto it = std::find_if(medials_.begin(), medials_.end(), [key](const Char &c)
        { return c.key == key; });
        if (it != medials_.end()) return std::optional(*it);
    } else if(type == JamoType::Final)
    {
        auto it = std::find_if(finals_.begin(), finals_.end(), [key](const Char &c)
        { return c.key == key; });
        if (it != finals_.end()) return std::optional(*it);
    }
    return std::nullopt;
}

Char Alphabet::soundlessInitial() const
{
    return soundless_initial_;
}

std::optional<Char> Alphabet::toCharCluster(std::u32string parent, char16_t key, JamoType type) const
{
    if(type == JamoType::Initial)
    {
        auto it = std::find_if(initial_clusters_.begin(), initial_clusters_.end(), [parent, key](const Char &c)
        { return c.parent == parent && c.key == key; });
        if (it != initial_clusters_.end()) return std::optional(*it);
    } else if(type == JamoType::Medial)
    {
        auto it = std::find_if(medial_clusters_.begin(), medial_clusters_.end(), [parent, key](const Char &c)
        { return c.parent == parent && c.key == key; });
        if (it != medial_clusters_.end()) return std::optional(*it);
    } else if(type == JamoType::Final)
    {
        auto it = std::find_if(final_clusters_.begin(), final_clusters_.end(), [parent, key](const Char &c)
        { return c.parent == parent && c.key == key; });
        if (it != final_clusters_.end()) return std::optional(*it);
    }
    return std::nullopt;
}

Char Alphabet::soundlessVowel() const
{
    return soundless_vowel_;
}

std::optional<Char> Alphabet::toParent(const Char &ch, JamoType type) const
{
    if(type == JamoType::Initial)
    {
        auto it = std::find_if(initials_.begin(), initials_.end(), [ch](const Char &c)
        { return c.jamo == ch.parent; });
        if (it != initials_.end()) return std::optional(*it);
        it = std::find_if(initial_clusters_.begin(), initial_clusters_.end(), [ch](const Char &c)
        { return c.jamo == ch.parent; });
        if (it != initial_clusters_.end()) return std::optional(*it);
    } else if(type == JamoType::Medial)
    {
        auto it = std::find_if(medials_.begin(), medials_.end(), [ch](const Char &c)
        { return c.jamo == ch.parent; });
        if (it != medials_.end()) return std::optional(*it);
        it = std::find_if(medial_clusters_.begin(), medial_clusters_.end(), [ch](const Char &c)
        { return c.jamo == ch.parent; });
        if (it != medial_clusters_.end()) return std::optional(*it);
    } else if(type == JamoType::Final)
    {
        auto it = std::find_if(finals_.begin(), finals_.end(), [ch](const Char &c)
        { return c.jamo == ch.parent; });
        if (it != finals_.end()) return std::optional(*it);
        it = std::find_if(final_clusters_.begin(), final_clusters_.end(), [ch](const Char &c)
        { return c.jamo == ch.parent; });
        if (it != final_clusters_.end()) return std::optional(*it);
    }
    return std::nullopt;
}

