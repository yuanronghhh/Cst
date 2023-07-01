/*
 * Cairo SDL clock. Shows how to use Cairo with SDL.
 * Made by Writser Cleveringa, based upon code from Eric Windisch.
 * Minor code clean up by Chris Nystrom (5/21/06) and converted to cairo-sdl
 * by Chris Wilson and converted to cairosdl by M Joonas Pihlaja.
 */

#include <CstDemo/TestSDL.h>


static void test_sdl_basic(void) {
  SDL_Window *window;

  int width = 640;
  int height = 480;
  int running = 1;
  SDL_Event event;

  if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return;
  }

  window = SDL_CreateWindow("Demo Cairo with SDL.", 0, 0, width, height,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  if(window == NULL) {
    SDL_Log("Unable to create SDL window: %s", SDL_GetError());
    return;
  }

  while(running) {
    SDL_WaitEvent (&event);

    switch(event.type) {
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        switch(event.key.keysym.sym) {
          case SDLK_q:
            running = 0;
            break;
        }
        break;
      case SDL_QUIT:
        running = 0;
        break;
    }
  }

  printf("%s\n", "done");
  SDL_DestroyWindow(window);

  SDL_Quit ();
}


void test_sdl_init(int argc, char *argv[]) {
  UNITY_BEGIN();
  {
    RUN_TEST(test_sdl_basic);
  }
  UNITY_END();
}
