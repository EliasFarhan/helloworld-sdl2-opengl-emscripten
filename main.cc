// Copyright 2015 Red Blob Games <redblobgames@gmail.com>
// License: Apache v2.0 <http://www.apache.org/licenses/LICENSE-2.0.html>

#include "common.h"
#include "glwrappers.h"
#include "window.h"
#include "render-sprites.h"
#include "render-surface.h"
#include "render-imgui.h"
#include "font.h"

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


std::unique_ptr<Window> window;
std::unique_ptr<RenderSprites> sprite_layer;
static bool main_loop_running = true;

void main_loop() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT: {
      main_loop_running = false;
      break;
    }
    case SDL_KEYUP: {
      int sym = event.key.keysym.sym;
      if (sym == SDLK_ESCAPE) { main_loop_running = false; }
      break;
    }
    }

    window->ProcessEvent(&event);
  }

  if (window->visible) {
    static float t = 0.0;
    t += 0.01;

    std::vector<Sprite> s;
    int SIDE = 4;
    int NUM = SIDE * SIDE;
    for (int j = 0; j < NUM; j++) {
      s.emplace_back();
      s[j].image_id = 0;
      s[j].x = (0.5 + j % SIDE - 0.5*SIDE + ((j/SIDE)%2) * 0.5 - 0.25) * 2.0 / SIDE;
      s[j].y = (0.5 + j / SIDE - 0.5*SIDE) * 2.0 / SIDE;
      s[j].scale = 2.0 / SIDE;
      s[j].rotation_degrees = 180/M_PI * (j * 0.03 * t);
    }
    
    sprite_layer->SetSprites(s);
    
    window->Render();
  }
}


int main(int, char**) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) { FAIL("SDL_Init"); }
  SDL_GL_SetSwapInterval(1);

  window = std::unique_ptr<Window>(new Window(800, 600));

  Font font("assets/DroidSans.ttf", 32);

  SDL_Surface* overlay_surface = CreateRGBASurface(window->width, window->height);
  SDL_Rect fillarea;
  fillarea.x = 0;
  fillarea.y = 0;
  fillarea.w = 2 + font.Width("Hello jello");
  fillarea.h = font.Height();
  SDL_FillRect(overlay_surface, &fillarea, SDL_MapRGBA(overlay_surface->format, 64, 32, 0, 192));

  font.Draw(overlay_surface, 1, font.Baseline(), "Hello jello");

  sprite_layer = std::unique_ptr<RenderSprites>(new RenderSprites);
  std::unique_ptr<RenderSurface> overlay_layer(new RenderSurface(overlay_surface));
  std::unique_ptr<RenderImGui> ui_layer(new RenderImGui());
  window->AddLayer(sprite_layer.get());
  window->AddLayer(overlay_layer.get());
  // window->AddLayer(ui_layer.get());

#ifdef __EMSCRIPTEN__
  // 0 fps means to use requestAnimationFrame; non-0 means to use setTimeout.
  emscripten_set_main_loop(main_loop, 0, 1);
#else
  while (main_loop_running) {
    main_loop();
    SDL_Delay(16);
  }
#endif

  overlay_layer = nullptr;
  sprite_layer = nullptr;
  window = nullptr;
  SDL_Quit();
}
