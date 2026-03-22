#pragma once

#include "audio/effect.h"

namespace GuitarAmp {

class Equalizer : public Effect {
public:
    Equalizer();
    void process(float* buffer, int num_samples) override;
    void set_sample_rate(int sample_rate) override;
    void reset() override;
    const char* name() const override { return "Equalizer"; }
    std::vector<EffectParam>& params() override { return params_; }

private:
    std::vector<EffectParam> params_;

    // 3-band EQ using biquad filters
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

    BiquadState low_shelf_;
    BiquadState mid_peak_;
    BiquadState high_shelf_;

    void compute_low_shelf(float freq, float gain_db, float q);
    void compute_peaking(float freq, float gain_db, float q);
    void compute_high_shelf(float freq, float gain_db, float q);
};

} // namespace GuitarAmp
