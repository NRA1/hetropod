#include "Hetropod.h"
#include <fcitx/userinterfacemanager.h>

void HetropodEngine::keyEvent(const fcitx::InputMethodEntry &, fcitx::KeyEvent &keyEvent)
{
    if (keyEvent.isRelease()) return;

    auto state = keyEvent.inputContext()->propertyFor(&factory_);
    state->keyEvent(keyEvent);
}

HetropodEngine::HetropodEngine(Instance *instance)
    : instance_(instance),
      factory_([](InputContext &ic) { return new HetropodState(&ic); })
{
    instance_->inputContextManager().registerProperty("hetropodState", &factory_);
}

void HetropodEngine::deactivate(const InputMethodEntry &, InputContextEvent &event)
{
    if (event.type() == EventType::InputContextSwitchInputMethod) {
        auto state = event.inputContext()->propertyFor(&factory_);
        state->commit();
    }
}

void HetropodEngine::reset(const InputMethodEntry &, InputContextEvent &event)
{
    auto state = event.inputContext()->propertyFor(&factory_);
    state->reset();
}

FCITX_ADDON_FACTORY(HetropodEngineFactory)