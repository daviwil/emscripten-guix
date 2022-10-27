#include <GLFW/glfw3.h>
#include <emscripten.h>
#include <stdio.h>
#include <stdlib.h>

void error_callback(int error, const char *description) {
  printf("GLFW Error: %s\n", description);
}

double color_timer = -1;
float r = 1.0, g = 0.0, b = 0.0;

void random_color(float *r, float *g, float *b) {
  *r = (float)(rand() % 255) / 255.f;
  *g = (float)(rand() % 255) / 255.f;
  *b = (float)(rand() % 255) / 255.f;
}

void render_loop(void *arg) {
  GLFWwindow *window = (GLFWwindow *)arg;

  // Set the viewport to the current framebuffer size
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  // Randomize the clear color
  if (glfwGetTime() - color_timer > 1.f) {
    random_color(&r, &g, &b);
    printf("New color: %f %f %f\n", r, g, b);
    color_timer = glfwGetTime();
  }

  // Clear the screen with the current color
  glClearColor(r, g, b, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  // Flip the render buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
}

int main() {
  // Reading a file included with the app using `--preload-file assets/`
  char contents[512];
  FILE *fp = fopen("assets/file.txt", "r");
  fgets(contents, 512, fp);
  printf("File contents: %s", contents);

  // Initialize the version of GLFW provided by Emscripten
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    printf("Initialization failed!\n");
  }

  // Create the "window" which is ultimately just a canvas on the page
  GLFWwindow *window =
      glfwCreateWindow(640, 480, "Emscripten Example", NULL, NULL);
  if (!window) {
    // Window or OpenGL context creation failed
  }

  // Set up the OpenGL rendering context
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  // Initialize the color timer before we use it in `render_loop`
  color_timer = glfwGetTime();

  // Set the main loop function which will be invoked by JavaScript each frame.
  // This is needed because JavaScript is a single-threaded event loop so we
  // have to let it drive the invocation of our render function, otherwise we'd
  // block the browser tab's main thread.
  printf("Starting render loop!\n");
  emscripten_set_main_loop_arg(render_loop, window, 0, 1);

  // We never really hit this point :)

  glfwTerminate();

  return 0;
}
