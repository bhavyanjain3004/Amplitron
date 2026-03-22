#pragma once

#include "audio/effect.h"

namespace GuitarAmp {

class CabinetSim : public Effect {
public:
    CabinetSim();
    void process(float* buffer, int num_samples) override;
    void reset() override;
    const char* name() const override { return "Cabinet"; }
    std::vector<EffectParam>& params() override { return params_; }

private:
    std::vector<EffectParam> params_;

    // Simple IIR cabinet simulation using cascaded biquads
    struct BiquadState {
        float x1 = 0, x2 = 0, y1 = 0, y2 = 0;
        float b0 = 1, b1 = 0, b2 = 0, a1 = 0, a2 = 0;
        float process(float x) {
            float y = b0 * x + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
            x2 = x1; x1 = x;
            y2 = y1; y1 = y;
            return y;
        }
        void reset() { x1 = x2 = y1 = y2 = 0; }
    };

    BiquadState lp_;   // speaker rolloff
    BiquadState hp_;   // low cut
    BiquadState peak_; // resonance bump
};

} // namespace GuitarAmp
