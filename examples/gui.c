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
//! [code]

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

static int cursor_xpos, cursor_ypos;
int window_xpos, window_ypos;

static vec3 color = {1.0f, 0.0f, 0.0f};

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

GLFWGUImenu *menu;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT) {
        glfwGuiPopUpMenu(window, menu);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
//    window_xpos += (xoffset * 4.0);
//    window_ypos += (yoffset * 4.0);
//    glfwSetWindowPos(window, window_xpos, window_ypos);
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
//    cursor_xpos = (int)xpos;
//    cursor_ypos = (int)ypos;
//    printf("x=%d y=%d\n", cursor_xpos, cursor_ypos);
//    glfwSetWindowPos(window, cursor_xpos, cursor_ypos);
}

void menu_callback(GLFWGUImenuitem *item, void *callback_data)
{
    printf("menu_callback [%s]\n", (char *)callback_data);
}

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
//    glfwWindowHint( GLFW_DECORATED, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwGetWindowPos(window, &window_xpos, &window_ypos);

////////////////

    menu = glfwGuiNewMenu("popmenu");

    glfwGuiAppendMenuItem(menu, "1111", GLFW_MOD_SHIFT, 'a', menu_callback, "1111");
    glfwGuiAppendMenuItem(menu, "2222", GLFW_MOD_CONTROL, 'b', menu_callback, "2222");

    GLFWGUImenu *submenu = glfwGuiAppendSubMenu(menu, "submenu");
    glfwGuiAppendMenuItem(submenu, "3333", GLFW_MOD_ALT, 'c', menu_callback, "3333");
    glfwGuiAppendMenuItem(submenu, "4444", GLFW_MOD_SUPER, 'd', menu_callback, "4444");

////////////////

    GLFWGUImenu *appmenu = glfwGuiGetApplicationMenu(window);

    GLFWGUImenu *menu2 = glfwGuiAppendSubMenu(appmenu, "qwerty");

    glfwGuiAppendMenuItem(menu2, "5555", GLFW_MOD_SHIFT, 'e', menu_callback, "5555");
    glfwGuiAppendMenuItem(menu2, "6666", GLFW_MOD_CONTROL, 'f', menu_callback, "6666");

    GLFWGUImenu *submenu2 = glfwGuiAppendSubMenu(menu2, "submenu");
    glfwGuiAppendMenuItem(submenu2, "7777", GLFW_MOD_SHIFT|GLFW_MOD_CONTROL, 'g', menu_callback, "7777");
    glfwGuiAppendMenuItem(submenu2, "8888", GLFW_MOD_SHIFT|GLFW_MOD_CONTROL|GLFW_MOD_ALT, 'h', menu_callback, "8888");
    glfwGuiAppendMenuItem(submenu2, "9999", GLFW_MOD_SHIFT|GLFW_MOD_CONTROL|GLFW_MOD_ALT|GLFW_MOD_SUPER, 'i', menu_callback, "9999");

////////////////

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

//glEnable(GL_BLEND);
//glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

    // NOTE: OpenGL error checks have been omitted for brevity

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
    const GLint ucol_location = glGetUniformLocation(program, "uCol");

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          0, 0);

    glUseProgram(program);

    glUniform3fv(ucol_location, 1, (const GLfloat*) &color);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetScrollCallback(window, scroll_callback);

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

//! [code]
