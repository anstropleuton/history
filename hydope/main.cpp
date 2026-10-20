/// @file
///
/// @author    Anstro Pleuton <https://github.com/anstropleuton>
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Hydope - Hypnotizing Double Pendulum.
///
/// Hydope main source file.
///
/// This project is licensed under the terms of MIT license.

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <execution>
#include <vector>

#include "pendulum.hpp"
#include "raylib.h"
#include "utilities.hpp"

FILE *ffmpegPipe = nullptr;

int main()
{
    SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(1080, 1080, "Hydope");

    // Open a pipe to ffmpeg
    ffmpegPipe = popen("ffmpeg -y -f rawvideo -pixel_format rgba -video_size 1080x1080 -framerate 60 -i - -c:v libx264 -pix_fmt yuv420p output.mp4", "w");

    if (!ffmpegPipe)
    {
        CloseWindow();
        fprintf(stderr, "Failed to open ffmpeg pipe\n");
        return 1;
    }

    const std::size_t items_count = 1'000'000;

    // Single buffer
    std::vector<WeldedPendulum> pendulums_buffer(items_count);

    // Initialize pendulums
    for (std::size_t i = 0; i < items_count; i++)
    {
        double angle = PI + 0.125 + (double) i / items_count * 0.0001 + std::fmod(0.0 * 0.5 + PI / 8.0, (double) PI / 4) - PI / 8.0;

        pendulums_buffer[i].pendulums.resize(2);
        for (Pendulum &pendulum : pendulums_buffer[i].pendulums)
        {
            pendulum.length = 150.0;
            pendulum.mass   = 10.0;
            pendulum.angle  = angle;
        }
        pendulums_buffer[i].pendulums.back().angle = PI;
        pendulums_buffer[i].color                  = ColorFromHSV(i * 360.0f / items_count + GetTime() * 5.0f, 0.5f, 1.0f);
    }

    // Camera
    WorldCamera2D camera;

    camera.actualZoom = log2(PI / 2.0f);

    RenderTexture2D target = LoadRenderTexture(1080, 1080);

    for (std::size_t frame_count = 0; frame_count < 4000 && !WindowShouldClose(); frame_count++)
    {
        // Update all pendulums sequentially
        std::for_each(std::execution::par_unseq, pendulums_buffer.begin(), pendulums_buffer.end(), [](WeldedPendulum &wp) {
            wp.Update();
        });

        camera.Update();

        // Draw
        BeginTextureMode(target);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(BLACK, 64.0f / 255.0f));
        camera.BeginMode2D();

        std::for_each(std::execution::par_unseq, pendulums_buffer.begin(), pendulums_buffer.end(), [](WeldedPendulum &wp) {
            wp.Draw();
        });

        camera.EndMode2D();
        EndTextureMode();

        BeginDrawing();
        DrawTexture(target.texture, 0, 0, WHITE); // optional screen preview
        EndDrawing();

        // Capture frame and send to ffmpeg
        Image frame = LoadImageFromTexture(target.texture);
        fwrite(frame.data, 1, frame.width * frame.height * 4, ffmpegPipe);
        UnloadImage(frame);
    }

    if (ffmpegPipe) pclose(ffmpegPipe);

    CloseWindow();
}
