#include "./seat_listener.hpp"

#include "./keyboard_listener.hpp"
#include "./pointer_listener.hpp"
#include "./util/log.h"

void applyPointerListener(
    bool have_pointer,
    wl_pointer*& pointer,
    std::unique_ptr<WaylandClient::PointerListener>& listener,
    wl_seat* seat) {
  if (have_pointer && pointer == nullptr) {
    pointer = wl_seat_get_pointer(seat);
    wl_pointer_add_listener(pointer,
                            WaylandClient::PointerListener::pointerListener(),
                            listener.get());
  } else if (!have_pointer && pointer != nullptr) {
    wl_pointer_release(pointer);
    pointer = nullptr;
  }
}

void applyKeyboardListener(
    bool have_pointer,
    wl_keyboard*& keyboard,
    std::unique_ptr<WaylandClient::KeyboardListener>& listener,
    wl_seat* seat) {
  if (have_pointer && keyboard == nullptr) {
    keyboard = wl_seat_get_keyboard(seat);
    wl_keyboard_add_listener(
        keyboard, WaylandClient::KeyboardListener::keyboardListener(),
        listener.get());
  } else if (!have_pointer && keyboard != nullptr) {
    wl_keyboard_release(keyboard);
    keyboard = nullptr;
  }
}

void WaylandClient::SeatListener::handleSeatCapabilities(
    wl_seat* seat,
    uint32_t capabilities) {
  VLOG << "handleSeatCapabilities: ";

  bool have_pointer = capabilities & WL_SEAT_CAPABILITY_POINTER;
  applyPointerListener(have_pointer, owner().pointer_, owner().pointerListener_,
                       seat);
  bool have_keyboard = capabilities & WL_SEAT_CAPABILITY_KEYBOARD;
  applyKeyboardListener(have_keyboard, owner().keyboard_,
                        owner().keyboardListener_, seat);

  // wl_touch_add_listener

  return;
}

void WaylandClient::SeatListener::handleSeatName(wl_seat* seat,
                                                 const char* name) {
  VLOG << "seatname: " << name;
  return;
}

const wl_seat_listener WaylandClient::SeatListener::registryListener_{
    recieveSeatCapabilities, recieveSeatName};
