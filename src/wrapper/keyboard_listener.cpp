#include "./keyboard_listener.hpp"

#include <sys/mman.h>
#include <xkbcommon/xkbcommon.h>

#include <cassert>

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
  if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
    VLOG << "KEYMAP is not XKB_V1";
    close(fd);
    return;
  }

  if (xkb_context_ == nullptr) {
    xkb_context_ = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
  }

  char* map_shm = (char*)mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
  assert(map_shm != MAP_FAILED);
  xkb_keymap* xkb_keymap = xkb_keymap_new_from_string(
      xkb_context_, map_shm, XKB_KEYMAP_FORMAT_TEXT_V1,
      XKB_KEYMAP_COMPILE_NO_FLAGS);
  munmap(map_shm, size);
  close(fd);

  struct xkb_state* xkb_state = xkb_state_new(xkb_keymap);
  xkb_keymap_unref(xkb_keymap_);
  xkb_state_unref(xkb_state_);
  xkb_keymap_ = xkb_keymap;
  xkb_state_ = xkb_state;

  VLOG;
}

void WaylandClient::KeyboardListener::handleEnter(wl_keyboard* keyboard,
                                                  uint32_t serial,
                                                  wl_surface* surface,
                                                  wl_array* keys) {
  // on focus
  // ボタン押しながら当たった時、keysにキーがセットされる
  VLOG;
  // WTF
  // https://gitlab.freedesktop.org/wayland/wayland/-/issues/34
  // wl_array_for_each(itr, keys) { uint32_t key = *(uint32_t*)itr; }
  for (void* it = (keys)->data;
       (const char*)it < ((const char*)(keys)->data + (keys)->size);
       ((char*&)it) += (keys)->size) {
    uint32_t key = *(uint32_t*)it;
    xkb_keysym_t keysym = xkb_state_key_get_one_sym(xkb_state_, key + 8);
    VLOG << "keysym=" << keysym;
  }
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
  bool pressed = state == WL_KEYBOARD_KEY_STATE_PRESSED;
  bool released = state == WL_KEYBOARD_KEY_STATE_RELEASED;
  char buf[128];
  xkb_keysym_t keysym = xkb_state_key_get_one_sym(xkb_state_, key + 8);

  VLOG << "keysym=" << keysym << " pressed=" << pressed
       << " released=" << released << " time:" << time;

  // how to check key
  // VLOG << "Up? " << (keysym == XKB_KEY_Up) << "  sym:" << keysym
  //      << " == Up:" << XKB_KEY_Up;

  // https://wayland-book.com/seat/example.html
  // get keyname "Ctrl LEFT"
  // xkb_keysym_get_name(keysym, buf, sizeof(buf));
  // fprintf(stderr, "sym: %-12s (%d), ", buf, keysym);
  // get keychar 'a', '<'
  // xkb_state_key_get_utf8(xkb_state_, keycode, buf, sizeof(buf));
  // fprintf(stderr, "utf8: '%s'\n", buf);
}

void WaylandClient::KeyboardListener::handleModifiers(wl_keyboard* keyboard,
                                                      uint32_t serial,
                                                      uint32_t mods_depressed,
                                                      uint32_t mods_latched,
                                                      uint32_t mods_locked,
                                                      uint32_t group) {
  // capslock, shift など
  // xkb_state_ を更新しないと、xkb_state_key_get_utf8で大文字等が取れない。
  xkb_state_update_mask(xkb_state_, mods_depressed, mods_latched, mods_locked,
                        0, 0, group);
  VLOG;
}

void WaylandClient::KeyboardListener::handleRepeatInfo(wl_keyboard* keyboard,
                                                       int32_t rate,
                                                       int32_t delay) {
  VLOG;
  /* Left as an exercise for the reader */
  // https://wayland-book.com/seat/example.html
  // WTF
}

const wl_keyboard_listener
    WaylandClient::KeyboardListener::shellSurfaceListener_{
        recieveKeymap, recieveEnter,     recieveLeave,
        recieveKey,    recieveModifiers, recieveRepeatInfo};