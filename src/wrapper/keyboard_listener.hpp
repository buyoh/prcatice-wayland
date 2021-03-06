#ifndef SRC_WRAPPER_KEYBOARD_LISTENER_HPP__
#define SRC_WRAPPER_KEYBOARD_LISTENER_HPP__

#include <xkbcommon/xkbcommon.h>

#include "./wayland_client.hpp"

class WaylandClient::KeyboardListener : public Accessor {
  void handleKeymap(wl_keyboard* keyboard,
                    uint32_t format,
                    int32_t fd,
                    uint32_t size);
  void handleEnter(wl_keyboard* keyboard,
                   uint32_t serial,
                   wl_surface* surface,
                   wl_array* keys);
  void handleLeave(wl_keyboard* keyboard, uint32_t serial, wl_surface* surface);
  void handleKey(wl_keyboard* keyboard,
                 uint32_t serial,
                 uint32_t time,
                 uint32_t key,
                 uint32_t state);
  void handleModifiers(wl_keyboard* keyboard,
                       uint32_t serial,
                       uint32_t mods_depressed,
                       uint32_t mods_latched,
                       uint32_t mods_locked,
                       uint32_t group);
  void handleRepeatInfo(wl_keyboard* keyboard, int32_t rate, int32_t delay);

  static void recieveKeymap(void* data,
                            wl_keyboard* keyboard,
                            uint32_t format,
                            int32_t fd,
                            uint32_t size) {
    static_cast<KeyboardListener*>(data)->handleKeymap(keyboard, format, fd,
                                                       size);
  }
  static void recieveEnter(void* data,
                           wl_keyboard* keyboard,
                           uint32_t serial,
                           wl_surface* surface,
                           wl_array* keys) {
    static_cast<KeyboardListener*>(data)->handleEnter(keyboard, serial, surface,
                                                      keys);
  }
  static void recieveLeave(void* data,
                           wl_keyboard* keyboard,
                           uint32_t serial,
                           wl_surface* surface) {
    static_cast<KeyboardListener*>(data)->handleLeave(keyboard, serial,
                                                      surface);
  }
  static void recieveKey(void* data,
                         wl_keyboard* keyboard,
                         uint32_t serial,
                         uint32_t time,
                         uint32_t key,
                         uint32_t state) {
    static_cast<KeyboardListener*>(data)->handleKey(keyboard, serial, time, key,
                                                    state);
  }
  static void recieveModifiers(void* data,
                               wl_keyboard* keyboard,
                               uint32_t serial,
                               uint32_t mods_depressed,
                               uint32_t mods_latched,
                               uint32_t mods_locked,
                               uint32_t group) {
    static_cast<KeyboardListener*>(data)->handleModifiers(
        keyboard, serial, mods_depressed, mods_latched, mods_locked, group);
  }
  static void recieveRepeatInfo(void* data,
                                wl_keyboard* keyboard,
                                int32_t rate,
                                int32_t delay) {
    static_cast<KeyboardListener*>(data)->handleRepeatInfo(keyboard, rate,
                                                           delay);
  }

  static const wl_keyboard_listener shellSurfaceListener_;

  xkb_context* xkb_context_ = nullptr;
  xkb_state* xkb_state_ = nullptr;
  xkb_keymap* xkb_keymap_ = nullptr;

 public:
  static const wl_keyboard_listener* keyboardListener() {
    return &shellSurfaceListener_;
  }

  KeyboardListener(WaylandClient* wc) : Accessor(wc) {}
};

#endif  // SRC_WRAPPER_KEYBOARD_LISTENER_HPP__
