#include "../shader.h"
#include "../io.h"
#include <glad/glad.h>
#include <stdio.h>

unsigned int shader_create(const char *pathVert, const char *pathFrag) {
  int success;
  char infoLog[512];

  // Vertex Shader opening
  File fileVertex = read_file(pathVert);
  if (!fileVertex.is_valid) {
    fprintf(stderr, "Error Reading vertex file : %s\n", pathVert);
  }
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, (const char *const *)&fileVertex.data, NULL);
  glCompileShader(vertexShader);
  // check for shader compile errors
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    fprintf(stderr, "Erro Vertex: %s\n", infoLog);
  }

  // Fragment Shader Opening
  File fileFragment = read_file(pathFrag);
  if (!fileFragment.is_valid) {
    fprintf(stderr, "Error Reading vertex file : %s\n", pathFrag);
  }
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, (const char *const *)&fileFragment.data,
                 NULL);
  glCompileShader(fragmentShader);
  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    fprintf(stderr, "Erro Fragment: %s\n", infoLog);
  }
  // Shader Linking
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    fprintf(stderr, "Erro Linking : %s\n", infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}
