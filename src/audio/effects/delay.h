#pragma once

#include "audio/effect.h"

namespace GuitarAmp {

class Delay : public Effect {
public:
    Delay();
    void process(float* buffer, int num_samples) override;
    void set_sample_rate(int sample_rate) override;
    void reset() override;
    const char* name() const override { return "Delay"; }
    std::vector<EffectParam>& params() override { return params_; }

private:
    std::vector<EffectParam> params_;
    std::vector<float> delay_buffer_;
    int write_pos_ = 0;
    int max_delay_samples_ = 0;
    float lp_state_ = 0.0f;
};

} // namespace GuitarAmp
