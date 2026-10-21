// Glarens - GUI Framework.
//
// Dummy example file.
//
// Copyright (c) 2026 Anstro Pleuton.
// This project is licensed under the terms of MIT license.
// See LICENSE.md file in the project root for license text.

#include "glarens/glarens.hpp"
#include "glarens/node.hpp"
#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_render.h>
#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL_main.h>
#include <iostream>

SDL_Window   *window   = nullptr;
SDL_Renderer *renderer = nullptr;

std::shared_ptr<Node> root;

SDL_AppResult SDL_AppInit(void **, int argc, char *argv[]) {
    SDL_SetAppMetadata("Dummy Application", "0.0.1", "user.anstropleuton.dummy_application");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Dummy Application", 1024, 768, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        std::cout << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    glarens_init(window, renderer);

    root = Node::create();

    Model rootModel;
    rootModel.scale = glm::vec2(1.0f);
    root->set_model(rootModel);

    auto  leftBar = root->create_child();
    Model leftBarModel;
    leftBarModel.anchor   = glm::vec2(-0.5f, 0.0f);
    leftBarModel.floating = glm::vec2(0.5f, 0.0f);
    leftBarModel.size     = glm::vec2(0.0f, 0.0f);
    leftBarModel.scale    = glm::vec2(0.25f, 1.0f);
    leftBar->set_model(leftBarModel);

    set_root(root);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *, SDL_Event *event) {
    switch (event->type) {
    case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
    case SDL_EVENT_WINDOW_RESIZED:
        root->refresh_model();
        break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    iterate_root();

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *, SDL_AppResult result) {
    glarens_term();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}
