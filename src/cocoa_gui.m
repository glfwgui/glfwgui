//========================================================================
// GLFWGUI - github.com/glfwgui/glfwgui
//------------------------------------------------------------------------
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

#include "internal.h"

#include <Carbon/Carbon.h>

#if defined(_GLFW_COCOA)

struct menuitem {
    NSMenuItem *item;
    int key_mods;
    char key_shortcut;
    void (*callback)(GLFWGUImenuitem *, void *);
    void *callback_data;
};

static struct menuitem maps[256];
int maps_index = 0;

@interface GLFWGUI : NSObject
@end

@implementation GLFWGUI

- (void)menu_action:(id)sender
{
//    NSLog(@"menu_action");
    for (int i = 0; i < maps_index; i++) {
        if (maps[i].item == (NSMenuItem *)sender) {
            if (maps[i].callback) {
                maps[i].callback(maps[i].item ,maps[i].callback_data);
            }
        }
    }
}

- (NSMenuItem *)glfwGuiAppendMenuItem:(NSMenu *)menu NSString:name NSString:key_shortcut
{
    NSMenuItem *item = [(NSMenu *)menu addItemWithTitle:name action:@selector(menu_action:) keyEquivalent:key_shortcut];
    [item setTarget:self];
    return item;
}

@end // GLFWGUI

GLFWGUI *GLFWGUINSTANCE;

void _glfwGuiInit(void)
{
    GLFWGUINSTANCE = [[GLFWGUI alloc] init];
}

GLFWGUImenu *glfwGuiGetApplicationMenu(GLFWwindow *window)
{
    return (GLFWGUImenu *)[NSApp menu];
}

GLFWGUImenu *glfwGuiNewMenu(const char *name)
{
    return (GLFWGUImenu *)[[NSMenu alloc] initWithTitle:[NSString stringWithUTF8String:(name)]];
}

GLFWGUImenuitem *glfwGuiAppendMenuItem(GLFWGUImenu *menu, const char *name, int key_mods, const char key_shortcut,
                                    void (*callback)(GLFWGUImenuitem *, void *), void *callback_data)
{
    NSMenuItem *item = [GLFWGUINSTANCE glfwGuiAppendMenuItem:(NSMenu *)menu
                        NSString:[NSString stringWithUTF8String:(name)]
                        NSString:[NSString stringWithUTF8String:(key_shortcut == 0 ? "" : &key_shortcut)]
    ];

    NSUInteger flags = 0;

    if (key_mods & GLFW_MOD_SHIFT)
        flags |= NSEventModifierFlagShift;
    if (key_mods & GLFW_MOD_CONTROL)
        flags |= NSEventModifierFlagControl;
    if (key_mods & GLFW_MOD_ALT)
        flags |= NSEventModifierFlagOption;
    if (key_mods & GLFW_MOD_SUPER)
        flags |= NSEventModifierFlagCommand;

    [item setKeyEquivalentModifierMask:flags];

    if (maps_index < 255) {
        maps[maps_index].item = item;
        maps[maps_index].key_mods = key_mods;
        maps[maps_index].key_shortcut = key_shortcut;
        maps[maps_index].callback = callback;
        maps[maps_index].callback_data = callback_data;
        maps_index++;
    }

    return item;
}

GLFWGUImenu *glfwGuiAppendSubMenu(GLFWGUImenu *menu, const char *name)
{
    GLFWGUImenu *submenu = glfwGuiNewMenu(name);

    GLFWGUImenuitem *item = glfwGuiAppendMenuItem(menu, name, 0, 0, NULL, NULL);
    [(NSMenu *)menu setSubmenu:(NSMenu *)submenu forItem:((NSMenuItem *)item)];

    return submenu;
}

void glfwGuiPopUpMenu(GLFWwindow *window, GLFWGUImenu *menu)
{
    NSWindow *ns_window = glfwGetCocoaWindow(window);
    [(NSMenu *)menu popUpMenuPositioningItem:nil atLocation:[ns_window convertPointFromScreen:[NSEvent mouseLocation]] inView:[ns_window contentView]];
}

#endif
