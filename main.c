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
  float ratio;
  int width, height;

  glfwGetFramebufferSize(window, &width, &height);
  ratio = width / (float)height;

  // Randomize the clear color
  if (glfwGetTime() - color_timer > 1.f) {
    random_color(&r, &g, &b);
    printf("New color: %f %f %f\n", r, g, b);
    color_timer = glfwGetTime();
  }

  glViewport(0, 0, width, height);
  glClearColor(r, g, b, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  glfwSwapBuffers(window);
  glfwPollEvents();
}

int main() {
  // Reading a file included with the app using `--preload-file assets/`
  char contents[512];
  FILE *fp = fopen("assets/file.txt", "r");
  fgets(contents, 512, fp);
  printf("File contents: %s", contents);

  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    printf("Initialization failed!\n");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  GLFWwindow *window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
  if (!window) {
    // Window or OpenGL context creation failed
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  printf("Starting render loop!\n");

  color_timer = glfwGetTime();

  emscripten_set_main_loop_arg(render_loop, window, 0, 1);

  printf("Exiting!\n");

  glfwTerminate();

  return 0;
}
