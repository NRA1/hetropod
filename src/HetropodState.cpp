#include "HetropodState.h"
#include <fcitx-utils/utf8.h>
#include <fcitx/inputpanel.h>

#include <utility>

std::string hstringToUTF8(const std::list<HChar> &ustr) {
    std::string result;
    for (const auto& c : ustr) {
        result += c.toUTF8String();
    }
    return result;
}

std::string ustringToUTF8(const std::u32string &ustr) {
    std::string result;
    for (auto c : ustr) {
        result += utf8::UCS4ToUTF8(c);
    }
    return result;
}

HetropodState::HetropodState(InputContext *ic)
    : ic_(ic)
{
}

void HetropodState::keyEvent(KeyEvent &keyEvent)
{
    if(keyEvent.isRelease()) return;
    if(keyEvent.key().check(FcitxKey_Return))
    {
        if(!preedit_.empty())
        {
            commit();
            keyEvent.filterAndAccept();
        }
    }
    else if(keyEvent.key().check(FcitxKey_period) || keyEvent.key().check(FcitxKey_comma)
        || keyEvent.key().check(FcitxKey_greater) || keyEvent.key().check(FcitxKey_less)
        || keyEvent.key().check(FcitxKey_bracketleft) || keyEvent.key().check(FcitxKey_bracketright)
        || keyEvent.key().check(FcitxKey_space) || keyEvent.key().check(FcitxKey_KP_Space))
    {
        std::u32string str;
        if(keyEvent.key().check(FcitxKey_period)) str = U"。";
        else if(keyEvent.key().check(FcitxKey_comma)) str = U"、";
        else if(keyEvent.key().check(FcitxKey_greater)) str = U"〉";
        else if(keyEvent.key().check(FcitxKey_less)) str = U"〈";
        else if(keyEvent.key().check(FcitxKey_bracketleft)) str = U"「";
        else if(keyEvent.key().check(FcitxKey_bracketright)) str = U"」";
        else if(keyEvent.key().check(FcitxKey_space) || keyEvent.key().check(FcitxKey_KP_Space)) str = U" ";

        commit(str);
        keyEvent.filterAndAccept();
    }
    else if(keyEvent.key().check(FcitxKey_BackSpace))
    {
        if (!preedit_.empty())
        {
            HChar *ch = &preedit_.back();
            if(ch->final_.has_value())
            {
                auto parent = alphabet_.toParent(ch->final_.value(), JamoType::Final);
                if(parent.has_value())
                    ch->final_ = parent.value();
                else
                    ch->final_ = std::nullopt;
            } else if(ch->medial_.has_value())
            {
                auto parent = alphabet_.toParent(ch->medial_.value(), JamoType::Medial);
                if(parent.has_value())
                    ch->medial_ = parent.value();
                else
                {
                    if(ch->initial_.value().jamo == alphabet_.soundlessInitial().jamo)
                    {
                        parent = alphabet_.toParent(ch->medial_.value(), JamoType::Initial);
                        if(parent.has_value())
                            ch->initial_ = parent;
                    }
                    ch->medial_ = std::nullopt;
                }
            } else
            {
                auto parent = alphabet_.toParent(ch->initial_.value(), JamoType::Initial);
                if(parent.has_value())
                    ch->initial_ = parent.value();
                else
                    preedit_.pop_back();
            }
        }
        else return;
        keyEvent.filterAndAccept();
    }
    else if(keyEvent.key().isDigit())
    {
        int dg = keyEvent.key().digit();
        auto num = numerals_.toNumeral(dg);
        commit(num);
        keyEvent.filterAndAccept();
    }
    else if(keyEvent.key().isLAZ() || keyEvent.key().isUAZ() || keyEvent.key().check(FcitxKey_ccaron)
        || keyEvent.key().check(FcitxKey_scaron) || keyEvent.key().check(FcitxKey_zcaron)
        || keyEvent.key().check(FcitxKey_minus))
    {
        std::string ks = keyEvent.key().toString();
        std::optional<Char> key = std::nullopt;
        char16_t k;
        if(ks.length() == 1) k = ks[0];
        else if(keyEvent.key().check(FcitxKey_ccaron)) k = u'č';
        else if(keyEvent.key().check(FcitxKey_scaron)) k = u'š';
        else if(keyEvent.key().check(FcitxKey_zcaron)) k = u'ž';
        else if(keyEvent.key().check(FcitxKey_minus)) k = u'-';
        else
        {
            FCITX_WARN() << "Unexpected key of length greater than 1";
            return;
        }

        auto sound = keyboard_.findType(k);
        if(sound.has_value())
        {
            HChar *ch;

            if(!preedit_.empty())
            {
                ch = &preedit_.back();
            }
            else
            {
                ch = new HChar();
            }

            if(sound.value() == SoundType::Consonant)
            {
                if(keyboard_.isExclusiveInitial(k))
                {
                    auto c = alphabet_.toChar(k, JamoType::Initial);
                    if(c.has_value())
                    {
                        if(!ch->initial_.has_value())
                        {
                            ch->initial_ = c;

                            if(preedit_.empty())
                                preedit_.emplace_back(*ch);
                        } else
                        {
                            if(!ch->medial_.has_value())
                                ch->medial_ = alphabet_.soundlessVowel();
                            commitFirst();
                            preedit_.emplace_back(c.value());
                        }

                    } else FCITX_WARN() << "Entered a key specified in Keyboard but unspecified in Alphabet!";
                }
                else
                {
                    if(ch->final_.has_value())
                    {
                        auto cluster = alphabet_.toCharCluster(ch->final_.value().jamo, k, JamoType::Final);
                        if(cluster.has_value())
                        {
                            ch->final_ = cluster.value();
                        } else
                        {
                            commitFirst();
                            auto cons = alphabet_.toChar(k, JamoType::Initial);
                            if(cons.has_value())
                            {
                                preedit_.emplace_back(cons.value());
                            }
                            else
                                FCITX_WARN() << "Entered a key specified in Keyboard but unspecified in Alphabet!";
                        }
                    } else if(ch->initial_.has_value())
                    {
                        if(!ch->medial_.has_value())
                        {
                            auto cluster = alphabet_.toCharCluster(ch->initial_.value().jamo, k, JamoType::Initial);
                            if(cluster.has_value())
                            {
                                ch->initial_ = cluster.value();
                            }
                            else
                            {
                                auto cons = alphabet_.toChar(k, JamoType::Final);
                                if(cons.has_value())
                                {
                                    ch->medial_ = alphabet_.soundlessVowel();
                                    ch->final_ = cons.value();
                                }
                                else
                                    FCITX_WARN() << "Entered a key specified in Keyboard but unspecified in Alphabet!";

                            }
                        }
                        else
                        {
                            auto cons = alphabet_.toChar(k, JamoType::Final);
                            if(cons.has_value())
                            {
                                ch->final_ = cons.value();
                            }
                            else
                                FCITX_WARN() << "Entered a key specified in Keyboard but unspecified in Alphabet!";
                        }
                    } else
                    {
                        auto cons = alphabet_.toChar(k, JamoType::Initial);
                        if(cons.has_value())
                        {
                            preedit_.emplace_back(cons.value());
                        }
                        else
                            FCITX_WARN() << "Entered a key specified in Keyboard but unspecified in Alphabet!";
                    }
                }
            } else
            {
                if(ch->medial_.has_value())
                {
                    auto cluster = alphabet_.toCharCluster(ch->medial_.value().jamo, k, JamoType::Medial);
                    if(cluster.has_value())
                        ch->medial_ = cluster.value();
                    else
                    {
                        commitFirst();

                        auto cons = alphabet_.toChar(k, JamoType::Medial);
                        if(cons.has_value())
                        {
                            Char c = alphabet_.soundlessInitial();
                            if(!preedit_.empty() && preedit_.front().final_.has_value())
                            {
                                auto parent = alphabet_.toParent(preedit_.front().final_.value(), JamoType::Final);
                                c = alphabet_.toChar(preedit_.front().final_->key, JamoType::Initial).value();
                                if(parent.has_value())
                                {
                                    preedit_.front().final_ = parent.value();
                                }
                                else
                                {
                                    preedit_.front().final_ = std::nullopt;
                                }
                            }
                            preedit_.emplace_back(c, cons.value());
                        }
                        else
                            FCITX_WARN() << "Entered a key specified in Keyboard but unspecified in Alphabet!";
                    }
                } else
                {
                    if(ch->initial_.has_value())
                    {
                        auto cluster = alphabet_.toCharCluster(ch->initial_.value().jamo, k, JamoType::Medial);

                        if(cluster.has_value())
                        {
                            ch->initial_ = alphabet_.soundlessInitial();
                            ch->medial_ = cluster.value();
                        }
                        else
                        {
                            auto cons = alphabet_.toChar(k, JamoType::Medial);
                            if(cons.has_value())
                            {
                                ch->medial_ = cons;
                            }
                            else
                                FCITX_WARN() << "Entered a key specified in Keyboard but unspecified in Alphabet!";
                        }
                    } else
                    {
                        auto cons = alphabet_.toChar(k, JamoType::Medial);
                        if(cons.has_value())
                        {
                            if(!ch->initial_.has_value())
                                ch->initial_ = alphabet_.soundlessInitial();
                            ch->medial_ = cons;
                        }
                        else
                            FCITX_WARN() << "Entered a key specified in Keyboard but unspecified in Alphabet!";
                    }

                    if(preedit_.empty())
                        preedit_.emplace_back(*ch);
                }
            }
        } else
        {
            FCITX_WARN() << "Sound has no value";
        }

        keyEvent.filterAndAccept();
    } else if(keyEvent.key().isModifier())
    {
        return;
    }
    else
    {
        commit();
    }
    updateUI();
}

void HetropodState::updateUI()
{
    ic_->inputPanel().reset();

    if(!preedit_.empty()) {
        Text text;
        text.append(hstringToUTF8(preedit_), TextFormatFlag::HighLight);
        text.setCursor(preedit_.size());
        if(ic_->capabilityFlags().test(CapabilityFlag::Preedit))
            ic_->inputPanel().setClientPreedit(text);
        else
            ic_->inputPanel().setPreedit(text);
    }
    ic_->updatePreedit();

    ic_->updateUserInterface(UserInterfaceComponent::InputPanel);
}

void HetropodState::commitFirst()
{
    if(preedit_.size() > 1)
    {
        ic_->commitString(preedit_.front().toUTF8String());
        preedit_.pop_front();
    }
}

void HetropodState::commit()
{
    if(!preedit_.empty())
        validate(preedit_.back());
    ic_->commitString(hstringToUTF8(preedit_));
    reset();
}

void HetropodState::commit(const std::u32string &str)
{
    if(!preedit_.empty())
        validate(preedit_.back());
    ic_->commitString(hstringToUTF8(preedit_) + ustringToUTF8(str));
    reset();
}

void HetropodState::validate(HChar &chr) const
{
    if(!chr.medial_.has_value())
        chr.medial_ = alphabet_.soundlessVowel();
}

void HetropodState::reset()
{
    preedit_.clear();
}

HChar::HChar(Char initial, std::optional<Char> medial, std::optional<Char> final)
{
    initial_ = std::optional(initial);
    medial_ = std::move(medial);
    final_ = std::move(final);
}

std::string HChar::toUTF8String() const
{
    std::string result;
    if(initial_.has_value())
        result += ustringToUTF8(initial_.value().jamo);
    if(medial_.has_value())
        result += ustringToUTF8(medial_.value().jamo);
    if(final_.has_value())
        result += ustringToUTF8(final_.value().jamo);

    return result;
}

HChar::HChar()
{

}
