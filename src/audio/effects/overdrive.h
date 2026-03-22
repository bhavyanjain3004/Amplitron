#pragma once

#include "audio/effect.h"

namespace GuitarAmp {

class Overdrive : public Effect {
public:
    Overdrive();
    void process(float* buffer, int num_samples) override;
    void reset() override;
    const char* name() const override { return "Overdrive"; }
    std::vector<EffectParam>& params() override { return params_; }

private:
    std::vector<EffectParam> params_;
    float lp_state_ = 0.0f;
    float hp_state_ = 0.0f;
};

} // namespace GuitarAmp
