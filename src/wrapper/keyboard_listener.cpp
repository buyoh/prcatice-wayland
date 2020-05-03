#include "./keyboard_listener.hpp"

#include "./util/log.h"

void WaylandClient::KeyboardListener::handleKeymap(wl_keyboard* keyboard,
                                                   uint32_t format,
                                                   int32_t fd,
                                                   uint32_t size) {
  VLOG;
}

void WaylandClient::KeyboardListener::handleEnter(wl_keyboard* keyboard,
                                                  uint32_t serial,
                                                  wl_surface* surface,
                                                  wl_array* keys) {
  VLOG;
}

void WaylandClient::KeyboardListener::handleLeave(wl_keyboard* keyboard,
                                                  uint32_t serial,
                                                  wl_surface* surface) {
  VLOG;
}

void WaylandClient::KeyboardListener::handleKey(wl_keyboard* keyboard,
                                                uint32_t serial,
                                                uint32_t time,
                                                uint32_t key,
                                                uint32_t state) {
  VLOG;
}

void WaylandClient::KeyboardListener::handleModifiers(wl_keyboard* keyboard,
                                                      uint32_t serial,
                                                      uint32_t mods_depressed,
                                                      uint32_t mods_latched,
                                                      uint32_t mods_locked,
                                                      uint32_t group) {
  VLOG;
}

void WaylandClient::KeyboardListener::handleRepeatInfo(wl_keyboard* keyboard,
                                                       int32_t rate,
                                                       int32_t delay) {
  VLOG;
}
