/*
 * Cairo SDL clock. Shows how to use Cairo with SDL.
 * Made by Writser Cleveringa, based upon code from Eric Windisch.
 * Minor code clean up by Chris Nystrom (5/21/06) and converted to cairo-sdl
 * by Chris Wilson and converted to cairosdl by M Joonas Pihlaja.
 */

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <cairo/cairo.h>
#include "cairosdl.h"
#include <SDL2/SDL.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void test_draw_callback(cairo_t *cr) {
  printf("[test_draw_callback called]\n");
  int i;

  cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
      CAIRO_FONT_WEIGHT_BOLD);

  cairo_save (cr);
  cairo_set_font_size (cr, 40);
  cairo_move_to (cr, 40, 60);
  cairo_set_source_rgb (cr, 0,0,0);
  cairo_show_text (cr, "Hello World");
  cairo_restore (cr);

  cairo_set_source_rgb (cr, 1,0,0);
  cairo_set_font_size (cr, 20);
  cairo_move_to (cr, 50, 100);
  cairo_show_text (cr, "greetings from gtk and cairo");


  cairo_move_to (cr, 0, 150);
  for (i = 0; i < 800 / 10; i++) {
    cairo_rel_line_to (cr, 5,  10);
    cairo_rel_line_to (cr, 5, -10);
  }
  cairo_stroke (cr);
}

/* Draws a clock on a normalized Cairo context */
static void draw (cairo_t *cr) {
  time_t t;
  struct tm *tm;
  double seconds, minutes, hours;

  /* In newer versions of Visual Studio localtime(..) is deprecated. */
  /* Use localtime_s instead. See MSDN. */
  t = time (NULL);
  tm = localtime (&t);

  /* compute the angles for the indicators of our clock */
  seconds = tm->tm_sec * M_PI / 30;
  minutes = tm->tm_min * M_PI / 30;
  hours = tm->tm_hour * M_PI / 6;

  /* Fill the background with white. */
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);

  /* who doesn't want all those nice line settings :) */
  cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
  cairo_set_line_width (cr, 0.1);

  /* translate to the center of the rendering context and draw a black */
  /* clock outline */
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_translate (cr, 0.5, 0.5);
  cairo_arc (cr, 0, 0, 0.4, 0, M_PI * 2);
  cairo_stroke (cr);

  /* draw a white dot on the current second. */
  cairo_set_source_rgba (cr, 1, 1, 1, 0.6);
  cairo_arc (cr,
      sin (seconds) * 0.4, -cos (seconds) * 0.4,
      0.05, 0, M_PI * 2);
  cairo_fill (cr);

  /* draw the minutes indicator */
  cairo_set_source_rgba (cr, 0.2, 0.2, 1, 0.6);
  cairo_move_to (cr, 0, 0);
  cairo_line_to (cr, sin (minutes) * 0.4, -cos (minutes) * 0.4);
  cairo_stroke (cr);

  /* draw the hours indicator      */
  cairo_move_to (cr, 0, 0);
  cairo_line_to (cr, sin (hours) * 0.2, -cos (hours) * 0.2);
  cairo_stroke (cr);
}

cairo_t *get_cairo_context(SDL_Surface *surface) {
  cairo_t *cr;
  cairo_status_t status;

  SDL_LockSurface (surface);
  {
    cr = cairosdl_create (surface);
    cairo_scale (cr, surface->w, surface->h);
    status = cairo_status (cr);

    if (status != CAIRO_STATUS_SUCCESS) {
      fprintf (stderr, "%s\n", cairo_status_to_string (status));
      goto fail;
    }
  }
  SDL_UnlockSurface (surface);

  return cr;

fail:
  cairosdl_destroy (cr);
  return NULL;
}

void test_sdl_init(void) {
  SDL_Window *window;
  SDL_Surface *surface;
  SDL_Renderer *render = NULL;
  SDL_Texture *texture = NULL;
  SDL_Event event;
  cairo_t *cr;

  int width = 640;
  int height = 480;
  int depth = 32;

  if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return;
  }

  window = SDL_CreateWindow("Demo Cairo with SDL.", 0, 0, width, height,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  if(window == NULL) {
    SDL_Log("Unable to create SDL window: %s", SDL_GetError());
    goto done;
  }

  // render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  // if (render == NULL) {
  //   SDL_Log("Unable to create SDL render: %s", SDL_GetError());
  //   goto done;
  // }

  surface = SDL_CreateRGBSurface(0, width, height, depth, 0, 0, 0, 0);
  if (surface == NULL) {
    SDL_Log("Unable to create SDL surface: %s", SDL_GetError());
    goto done;
  }
  cr = get_cairo_context(surface);

  // texture = SDL_CreateTextureFromSurface(render, surface);
  // if (texture == NULL) {
  //   SDL_Log("Unable to create SDL texture: %s", SDL_GetError());
  //   goto done;
  // }

  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 255));
  // SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
  // SDL_RenderClear(render);
  // SDL_RenderCopy(render, texture, NULL, NULL);
  test_draw_callback(cr);
  // SDL_RenderPresent(render);

  while(1) {
    while (SDL_PollEvent (&event)) {
      switch (event.type) {
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_q) {
            goto done;
          }

          if (event.key.keysym.sym == SDLK_u) {
            test_draw_callback(cr);
          }

          break;
        case SDL_QUIT:
          goto done;
        default:
          break;
      }
    }
  }

done:
  if (cr != NULL)            { cairo_destroy(cr);     }
  if (render != NULL)        { SDL_DestroyRenderer(render); }
  if (surface != NULL)       { SDL_FreeSurface(surface);    }
  if (texture != NULL)       { SDL_DestroyTexture(texture); }
  if (window != NULL)        { SDL_DestroyWindow(window);   }

  SDL_Quit ();
} 
