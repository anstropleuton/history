#include "glarens/model.hpp"
#include "internal/app-data.hpp"

[[nodiscard]] Rect Rect::screen_rect() {
    int width, height;
    SDL_GetWindowSize(appData.window, &width, &height);
    glm::vec2 extent = glm::vec2(width, height);

    return Rect{
        .center = extent / 2.0f,
        .extent = extent,
    };
}
