#pragma once

#include "common.h"
#include "audio/effect.h"
#include <imgui.h>

namespace GuitarAmp {

class PedalWidget {
public:
    PedalWidget(std::shared_ptr<Effect> effect, int index);

    // Returns true if the pedal should be removed
    bool render();

    int get_index() const { return index_; }
    void set_index(int idx) { index_ = idx; }
    std::shared_ptr<Effect> get_effect() const { return effect_; }

private:
    void render_knob(const char* label, float* value, float min_val, float max_val,
                     float default_val, const char* format = "%.1f");
    void render_toggle(const char* label, bool* value);

    std::shared_ptr<Effect> effect_;
    int index_;

    // Pedal color scheme based on effect type
    ImVec4 pedal_color_;
    ImVec4 led_color_;

    void assign_colors();
};

} // namespace GuitarAmp
