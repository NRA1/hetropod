#ifndef HETROPOD_HETROPODSTATE_H
#define HETROPOD_HETROPODSTATE_H

#include <fcitx/inputcontext.h>
#include <queue>
#include "Alphabet.h"
#include "Keyboard.h"
#include "Numerals.h"

using namespace fcitx;

class HChar
{
public:
    HChar();

    HChar(Char initial, std::optional<Char> medial = std::nullopt, std::optional<Char> final = std::nullopt);

    std::string toUTF8String() const;

    std::optional<Char> initial_;
    std::optional<Char> medial_;
    std::optional<Char> final_;
};

class HetropodEngine;

class HetropodState : public InputContextProperty
{
public:
    HetropodState(InputContext *ic);

    void keyEvent(KeyEvent &keyEvent);

    void updateUI();

    void commit();
    void reset();

private:
    void commitFirst();
    void commit(const std::u32string &str);
    void validate(HChar &chr) const;

    InputContext *ic_;
    std::list<HChar> preedit_;
    Alphabet alphabet_;
    Keyboard keyboard_;
    Numerals numerals_;
};


#endif //HETROPOD_HETROPODSTATE_H
