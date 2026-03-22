#pragma once

#include "audio/effect.h"

namespace GuitarAmp {

class NoiseGate : public Effect {
public:
    NoiseGate();
    void process(float* buffer, int num_samples) override;
    void reset() override;
    const char* name() const override { return "Noise Gate"; }
    std::vector<EffectParam>& params() override { return params_; }

private:
    std::vector<EffectParam> params_;
    float envelope_ = 0.0f;
    float gain_ = 0.0f;
    float attack_coeff_ = 0.0f;
    float release_coeff_ = 0.0f;
    void update_coefficients();
};

} // namespace GuitarAmp
