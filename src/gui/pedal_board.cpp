#include "gui/pedal_board.h"
#include "gui/pedal_widget.h"

#include "audio/effects/noise_gate.h"
#include "audio/effects/compressor.h"
#include "audio/effects/overdrive.h"
#include "audio/effects/distortion.h"
#include "audio/effects/equalizer.h"
#include "audio/effects/chorus.h"
#include "audio/effects/delay.h"
#include "audio/effects/reverb.h"
#include "audio/effects/cabinet_sim.h"

#include <imgui.h>
#include <algorithm>

namespace GuitarAmp {

PedalBoard::PedalBoard(AudioEngine& engine)
    : engine_(engine) {
    rebuild_widgets();
}

PedalBoard::~PedalBoard() = default;

void PedalBoard::rebuild_widgets() {
    widgets_.clear();
    auto& effects = engine_.effects();
    for (int i = 0; i < static_cast<int>(effects.size()); ++i) {
        widgets_.push_back(std::make_unique<PedalWidget>(effects[i], i));
    }
}

void PedalBoard::render() {
    // Toolbar
    ImGui::BeginChild("PedalToolbar", ImVec2(0, 35), true);
    render_add_pedal_menu();
    ImGui::SameLine();

    if (ImGui::Button("Reset All")) {
        for (auto& fx : engine_.effects()) {
            fx->reset();
            auto& p = fx->params();
            for (auto& param : p) {
                param.value = param.default_val;
            }
        }
    }
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.6f, 1.0f),
        "  Signal Chain: %d pedals | Drag knobs to adjust | Double-click to reset knob",
        static_cast<int>(engine_.effects().size()));

    ImGui::EndChild();

    // Pedal board area with horizontal scroll
    ImGui::BeginChild("PedalArea", ImVec2(0, 0), true,
        ImGuiWindowFlags_HorizontalScrollbar);

    render_signal_chain();

    ImGui::EndChild();
}

void PedalBoard::render_add_pedal_menu() {
    if (ImGui::Button("+ Add Pedal")) {
        ImGui::OpenPopup("AddPedalPopup");
    }

    if (ImGui::BeginPopup("AddPedalPopup")) {
        ImGui::TextColored(ImVec4(0.8f, 0.6f, 0.2f, 1.0f), "DRIVE");
        if (ImGui::MenuItem("Overdrive")) {
            engine_.add_effect(std::make_shared<Overdrive>());
            rebuild_widgets();
        }
        if (ImGui::MenuItem("Distortion")) {
            engine_.add_effect(std::make_shared<Distortion>());
            rebuild_widgets();
        }

        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.4f, 1.0f), "DYNAMICS");
        if (ImGui::MenuItem("Noise Gate")) {
            engine_.add_effect(std::make_shared<NoiseGate>());
            rebuild_widgets();
        }
        if (ImGui::MenuItem("Compressor")) {
            engine_.add_effect(std::make_shared<Compressor>());
            rebuild_widgets();
        }

        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.2f, 0.6f, 1.0f, 1.0f), "MODULATION");
        if (ImGui::MenuItem("Chorus")) {
            engine_.add_effect(std::make_shared<Chorus>());
            rebuild_widgets();
        }

        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.6f, 0.3f, 1.0f, 1.0f), "TIME");
        if (ImGui::MenuItem("Delay")) {
            engine_.add_effect(std::make_shared<Delay>());
            rebuild_widgets();
        }
        if (ImGui::MenuItem("Reverb")) {
            engine_.add_effect(std::make_shared<Reverb>());
            rebuild_widgets();
        }

        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.2f, 1.0f), "TONE");
        if (ImGui::MenuItem("Equalizer")) {
            engine_.add_effect(std::make_shared<Equalizer>());
            rebuild_widgets();
        }
        if (ImGui::MenuItem("Cabinet Sim")) {
            engine_.add_effect(std::make_shared<CabinetSim>());
            rebuild_widgets();
        }

        ImGui::EndPopup();
    }
}

void PedalBoard::render_signal_chain() {
    if (widgets_.empty()) {
        ImGui::SetCursorPos(ImVec2(
            ImGui::GetWindowWidth() / 2 - 150,
            ImGui::GetWindowHeight() / 2 - 30
        ));
        ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.5f, 1.0f),
            "No pedals in chain.\nClick '+ Add Pedal' to get started.");
        return;
    }

    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 origin = ImGui::GetCursorScreenPos();

    // Draw signal flow line
    float line_y = origin.y + 160;
    float total_width = widgets_.size() * 195.0f + 40;
    dl->AddLine(
        ImVec2(origin.x, line_y),
        ImVec2(origin.x + total_width, line_y),
        IM_COL32(60, 60, 70, 150), 3.0f
    );

    // Input jack
    dl->AddCircleFilled(ImVec2(origin.x + 5, line_y), 6, IM_COL32(180, 180, 190, 255));
    dl->AddCircle(ImVec2(origin.x + 5, line_y), 6, IM_COL32(100, 100, 110, 255), 0, 1.5f);

    // Render each pedal
    float pedal_x = origin.x + 20;
    int remove_idx = -1;

    for (int i = 0; i < static_cast<int>(widgets_.size()); ++i) {
        ImGui::SetCursorScreenPos(ImVec2(pedal_x, origin.y + 5));

        if (widgets_[i]->render()) {
            remove_idx = i;
        }

        // Connection dot between pedals
        if (i < static_cast<int>(widgets_.size()) - 1) {
            float dot_x = pedal_x + 190;
            dl->AddCircleFilled(ImVec2(dot_x, line_y), 4, IM_COL32(120, 120, 140, 200));
        }

        pedal_x += 195;
    }

    // Output jack
    dl->AddCircleFilled(ImVec2(pedal_x, line_y), 6, IM_COL32(180, 180, 190, 255));
    dl->AddCircle(ImVec2(pedal_x, line_y), 6, IM_COL32(100, 100, 110, 255), 0, 1.5f);

    // Handle removal
    if (remove_idx >= 0) {
        engine_.remove_effect(remove_idx);
        rebuild_widgets();
    }

    // Reserve space for horizontal scrolling
    ImGui::Dummy(ImVec2(total_width + 20, 340));
}

} // namespace GuitarAmp
