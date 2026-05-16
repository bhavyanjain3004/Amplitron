#include "test_framework.h"
#include "gui/gui_presets.h"
#include "audio/audio_engine.h"
#include "gui/command_history.h"
#include "audio/effects/overdrive.h"
#include <memory>

using namespace Amplitron;

TEST(gui_presets_dirty_flag_input_gain) {
    AudioEngine engine;
    engine.initialize();

    CommandHistory history;
    GuiPresets gp(engine, history);

    gp.mark_clean();
    ASSERT_FALSE(gp.is_dirty());

    // 1. Change a simple top-level value to trigger dirty state
    engine.set_input_gain(engine.get_input_gain() + 0.123f);
    ASSERT_TRUE(gp.is_dirty());

    // 2. Validate baseline reset behavior (simulating Save/Load/New flows)
    gp.mark_clean();
    ASSERT_FALSE(gp.is_dirty());

    engine.shutdown();
}

TEST(gui_presets_dirty_on_add_effect) {
    AudioEngine engine;
    engine.initialize();

    CommandHistory history;
    GuiPresets gp(engine, history);
    
    gp.mark_clean();
    ASSERT_FALSE(gp.is_dirty());

    // 1. Add an effect to trigger dirty state
    auto od = std::make_shared<Overdrive>();
    engine.add_effect(od);
    ASSERT_TRUE(gp.is_dirty());

    // 2. Validate baseline reset behavior (simulating Save/Load/New flows)
    gp.mark_clean();
    ASSERT_FALSE(gp.is_dirty());

    engine.shutdown();
}