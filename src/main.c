#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <glad/glad.h>
#include <stdbool.h>
#include <stdio.h>
#define SDL_MAIN_HANDLED
#include "../src/engine/shader.h"
#include <SDL2/SDL.h>

void CalculateFrameRate();

int main(int argc, char *argv[]) {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Could not init SDL: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_Window *window =
      SDL_CreateWindow("MyGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (!window) {
    printf("Failed to init window: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_GL_CreateContext(window);
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    printf("Failed to load GL: %s\n", SDL_GetError());
    exit(1);
  }

  puts("OpenGL Loaded");
  printf("Vendor:   %s\n", glGetString(GL_VENDOR));
  printf("Renderer: %s\n", glGetString(GL_RENDERER));
  printf("Version:  %s\n", glGetString(GL_VERSION));

  puts("Hello there!");

  bool should_quit = false;

  unsigned int shader =
      shader_create("shader/default.vert", "shader/default.frag");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
      // positions // colors
      0.25f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
      0.75f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
      0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
  };
  float vertices2[] = {
      -0.25f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // left
      -0.75f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // right
      -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f  // top
  };
  // unsigned int indices[] = {
  //     // note that we start from 0!
  //     0, 1, 3, // first triangle
  //     1, 2, 3  // second triangle
  // };

  unsigned int VBO[2], VAO[2];
  glGenVertexArrays(2, VAO);
  glGenBuffers(2, VBO);
  // glGenBuffers(1, &EBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO[0]);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  //              GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(VAO[1]);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the vertex attribute's bound vertex buffer object so afterwards we can
  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally
  // modify this VAO, but this rarely happens. Modifying other VAOs requires a
  // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
  // VBOs) when it's not directly necessary.
  glBindVertexArray(0);

  // uncomment this call to draw in wireframe polygons.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!should_quit) {
    SDL_Event event;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangle
    glUseProgram(shader);
    // seeing as we only have a single VAO there's no need to bind it
    // every time, but we'll do so to keep things a bit more organized
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // seeing as we only have a single VAO there's no need to bind it
    // every time, but we'll do so to keep things a bit more organized
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        should_quit = true;
        break;
      case SDL_WINDOWEVENT: {
        switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED:
          glViewport(0, 0, event.window.data1, event.window.data2);
          break;
        default:
          break;
        }
      }
      default:
        break;
      }
    }
    SDL_GL_SwapWindow(window);
    // CalculateFrameRate();
  }

  glDeleteVertexArrays(2, VAO);
  glDeleteBuffers(2, VBO);
  // glDeleteBuffers(1, &EBO);
  // glDeleteProgram(shader);

  SDL_Quit();
  return 0;
}

void CalculateFrameRate() {
  static float framesPerSecond = 0.0f; // This will store our fps
  static float lastTime = 0.0f; // This will hold the time from the last frame
  float currentTime = SDL_GetTicks64() * 0.001f;
  ++framesPerSecond;
  if (currentTime - lastTime > 1.0f) {
    lastTime = currentTime;
    fprintf(stderr, "\nCurrent Frames Per Second: %d\n\n",
            (int)framesPerSecond);
    framesPerSecond = 0;
  }
}
