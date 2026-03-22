#pragma once

#include "common.h"
#include <fstream>
#include <chrono>

namespace GuitarAmp {

class AudioEngine;

class Recorder {
public:
    Recorder();
    ~Recorder();

    // Start recording to a WAV file
    bool start(const std::string& filepath, int sample_rate, int channels = 1);

    // Stop recording and finalize the WAV file
    void stop();

    // Write audio samples (called from audio callback)
    void write_samples(const float* buffer, int num_samples);

    // Write metadata JSON sidecar file
    void write_metadata(const std::string& wav_path, AudioEngine& engine);

    bool is_recording() const { return recording_.load(); }
    float get_duration() const;
    int64_t get_samples_written() const { return samples_written_.load(); }
    const std::string& filepath() const { return filepath_; }

    // Get default recordings directory
    static std::string get_recordings_dir();

    // Generate a timestamped filename
    static std::string generate_filename();

private:
    std::ofstream file_;
    std::atomic<bool> recording_{false};
    std::atomic<int64_t> samples_written_{0};
    int sample_rate_ = 48000;
    int channels_ = 1;
    std::string filepath_;

    std::chrono::steady_clock::time_point start_time_;

    // WAV header helpers
    void write_wav_header();
    void finalize_wav_header();
};

} // namespace GuitarAmp
