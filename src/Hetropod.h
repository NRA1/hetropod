#ifndef HETROPOD_HETROPOD_H
#define HETROPOD_HETROPOD_H

#include <fcitx/inputmethodengine.h>
#include <fcitx/inputcontextproperty.h>
#include <fcitx/addonfactory.h>
#include <fcitx/instance.h>
#include <fcitx/addonmanager.h>
#include "HetropodState.h"

using namespace fcitx;

class HetropodEngine : public fcitx::InputMethodEngineV2 {
public:
    HetropodEngine(Instance *instance);

    void keyEvent(const fcitx::InputMethodEntry & entry, fcitx::KeyEvent & keyEvent) override;
    void deactivate(const InputMethodEntry &entry, InputContextEvent &event) override;
    void reset(const InputMethodEntry &, InputContextEvent &event) override;


private:
    FactoryFor<HetropodState> factory_;
    Instance *instance_;
};

class HetropodEngineFactory : public fcitx::AddonFactory {
    fcitx::AddonInstance * create(fcitx::AddonManager * manager) override {
        FCITX_UNUSED(manager);
        return new HetropodEngine(manager->instance());
    }
};

#endif //HETROPOD_HETROPOD_H
