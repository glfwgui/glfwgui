//========================================================================
// OpenGL gui example
// Copyright (c) Camilla Löwy <elmindreda@glfw.org>
// Copyright (c) 2025 Paweł Płóciennik (github.com/pawplo)
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3gui.h>

#include "linmath.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

static const vec2 vertices[3] =
{
    { -0.6f, -0.4f },
    {  0.6f, -0.4f },
    {  0.0f,  0.6f }
};

static vec3 color = {1.0f, 0.0f, 0.0f};

enum color {
    RED,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    MAGENTA,
    COLOR_MAX
};

static const vec3 colors[COLOR_MAX] = {
    {1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 1.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 1.0f},
};

static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec2 vPos;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
"uniform vec3 uCol;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(uCol, 0.3);\n"
"}\n";

GLint ucol_location;

GLFWGUImenu *menu;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (action == GLFW_PRESS) {
        glfwGuiMenuItemKeyShortcutProcess(mods, key);
    }

}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT) {
        glfwGuiPopUpMenu(window, menu);
    }
}

void menu_callback(GLFWGUImenuitem *item, void *callback_data)
{
    printf("menu_callback [%s]\n", (char *)callback_data);

    if (strcmp(callback_data, "red") == 0) {
        glUniform3fv(ucol_location, 1, (const GLfloat*)colors[RED]);
    } else if (strcmp(callback_data, "green") == 0) {
        glUniform3fv(ucol_location, 1, (const GLfloat*)colors[GREEN]);
    } else if (strcmp(callback_data, "blue") == 0) {
        glUniform3fv(ucol_location, 1, (const GLfloat*)colors[BLUE]);
    } else if (strcmp(callback_data, "yellow") == 0) {
        glUniform3fv(ucol_location, 1, (const GLfloat*)colors[YELLOW]);
    } else if (strcmp(callback_data, "cyan") == 0) {
        glUniform3fv(ucol_location, 1, (const GLfloat*)colors[CYAN]);
    } else if (strcmp(callback_data, "magenta") == 0) {
        glUniform3fv(ucol_location, 1, (const GLfloat*)colors[MAGENTA]);
    }
}

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    menu = glfwGuiNewMenu("colors");

    glfwGuiAppendMenuItem(menu, "red", GLFW_MOD_SHIFT|GLFW_MOD_CONTROL|GLFW_MOD_ALT|GLFW_MOD_SUPER, 'r', menu_callback, "red");
    glfwGuiAppendMenuItem(menu, "green", GLFW_MOD_SHIFT|GLFW_MOD_CONTROL|GLFW_MOD_ALT|GLFW_MOD_SUPER, 'g', menu_callback, "green");

    GLFWGUImenu *submenu = glfwGuiAppendSubMenu(menu, "submenu");
    glfwGuiAppendMenuItem(submenu, "blue", GLFW_MOD_SHIFT|GLFW_MOD_CONTROL|GLFW_MOD_ALT|GLFW_MOD_SUPER, 'b', menu_callback, "blue");


    GLFWGUImenu *appmenu = glfwGuiGetApplicationMenu(window);

    GLFWGUImenu *menu2 = glfwGuiAppendSubMenu(appmenu, "colors");

    glfwGuiAppendMenuItem(menu2, "yellow", GLFW_MOD_SHIFT|GLFW_MOD_CONTROL|GLFW_MOD_ALT|GLFW_MOD_SUPER, 'y', menu_callback, "yellow");
    glfwGuiAppendMenuItem(menu2, "cyan", GLFW_MOD_SHIFT|GLFW_MOD_CONTROL|GLFW_MOD_ALT|GLFW_MOD_SUPER, 'c', menu_callback, "cyan");

    GLFWGUImenu *submenu2 = glfwGuiAppendSubMenu(menu2, "submenu");
    glfwGuiAppendMenuItem(submenu2, "magenta", GLFW_MOD_SHIFT|GLFW_MOD_CONTROL|GLFW_MOD_ALT|GLFW_MOD_SUPER, 'm', menu_callback, "magenta");

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    const GLint mvp_location = glGetUniformLocation(program, "MVP");
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    ucol_location = glGetUniformLocation(program, "uCol");

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          0, 0);

    glUseProgram(program);
    glUniform3fv(ucol_location, 1, (const GLfloat*) &color);

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    float angle = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float) height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4 m, p, mvp;
        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, angle);
        angle += 0.02f;
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &mvp);
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
