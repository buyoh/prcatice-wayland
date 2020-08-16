#include "./keyboard_listener.hpp"

#include "./util/log.h"

void WaylandClient::KeyboardListener::handleKeymap(wl_keyboard* keyboard,
                                                   uint32_t format,
                                                   int32_t fd,
                                                   uint32_t size) {
  // This event provides a file descriptor to the client which can
  // be memory-mapped to provide a keyboard mapping description.
  // キーコードとどのキーが押されたかの対応関係の取得
  // 英字配列とか
  // ref: xkb
  // handleKeyで実験してみると、24->Q 25->W 26->E ... なので、何らかの変換は必要
  VLOG;
}

void WaylandClient::KeyboardListener::handleEnter(wl_keyboard* keyboard,
                                                  uint32_t serial,
                                                  wl_surface* surface,
                                                  wl_array* keys) {
  // on focus
  VLOG;
}

void WaylandClient::KeyboardListener::handleLeave(wl_keyboard* keyboard,
                                                  uint32_t serial,
                                                  wl_surface* surface) {
  // off focus
  VLOG;
}

void WaylandClient::KeyboardListener::handleKey(wl_keyboard* keyboard,
                                                uint32_t serial,
                                                uint32_t time,
                                                uint32_t key,
                                                uint32_t state) {
  uint32_t keycode = key + 8;
  bool pressed = state == WL_KEYBOARD_KEY_STATE_PRESSED;
  bool released = state == WL_KEYBOARD_KEY_STATE_RELEASED;
  VLOG << "keycode=" << keycode << " pressed=" << pressed
       << " released=" << released << " time:" << time;
}

void WaylandClient::KeyboardListener::handleModifiers(wl_keyboard* keyboard,
                                                      uint32_t serial,
                                                      uint32_t mods_depressed,
                                                      uint32_t mods_latched,
                                                      uint32_t mods_locked,
                                                      uint32_t group) {
  // capslock, shift など
  VLOG;
}

void WaylandClient::KeyboardListener::handleRepeatInfo(wl_keyboard* keyboard,
                                                       int32_t rate,
                                                       int32_t delay) {
  VLOG;
}

const wl_keyboard_listener
    WaylandClient::KeyboardListener::shellSurfaceListener_{
        recieveKeymap, recieveEnter,     recieveLeave,
        recieveKey,    recieveModifiers, recieveRepeatInfo};