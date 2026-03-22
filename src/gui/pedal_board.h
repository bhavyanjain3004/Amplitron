#pragma once

#include "common.h"
#include "audio/audio_engine.h"

namespace GuitarAmp {

class PedalWidget;

class PedalBoard {
public:
    PedalBoard(AudioEngine& engine);
    ~PedalBoard();

    void render();

    void rebuild_widgets();

private:
    void render_add_pedal_menu();
    void render_signal_chain();

    AudioEngine& engine_;
    std::vector<std::unique_ptr<PedalWidget>> widgets_;
    int dragging_index_ = -1;
};

} // namespace GuitarAmp
