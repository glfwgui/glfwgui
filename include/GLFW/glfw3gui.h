/*************************************************************************
 * GLFWGUI - github.com/glfwgui/glfwgui
 *------------------------------------------------------------------------
 * Copyright (c) 2025 Paweł Płóciennik (github.com/pawplo)
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would
 *    be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not
 *    be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 *
 *************************************************************************/

#ifndef __GLFW3GUI_H
#define __GLFW3GUI_H

#include <GLFW/glfw3.h>

/*************************************************************************
 * GLFWGUI MENU
 *************************************************************************/

typedef void GLFWGUImenu;
typedef void GLFWGUImenuitem;

GLFWGUImenu *glfwGuiGetApplicationMenu(GLFWwindow *window);

GLFWGUImenu *glfwGuiNewMenu(const char *name);
GLFWGUImenuitem *glfwGuiAppendMenuItem(GLFWGUImenu *menu, const char *name, int key_mods, char key_shortcut,
                                    void (*callback)(GLFWGUImenuitem *, void *), void *callback_data);

GLFWGUImenu *glfwGuiAppendSubMenu(GLFWGUImenu *menu, const char *name);

void glfwGuiPopUpMenu(GLFWwindow *w, GLFWGUImenu *menu);

void glfwGuiMenuItemKeyShortcutProcess(int key_mods, char key_shortcut);

/*************************************************************************
 * GLFWGUI MESAGEBOX DIALOG
 *************************************************************************/

/*************************************************************************
 * GLFWGUI OPEN/SAVE DIALOG
 *************************************************************************/

/*************************************************************************
 * GLFWGUI COLOR PICKER DIALOG
 *************************************************************************/

void glfwGuiColorPicker(void (*color_picker_callback)(float rgba[4]));

#endif /* __GLFW3GUI_H */
