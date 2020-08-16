#ifndef SRC_WRAPPER_POINTER_LISTENER_HPP__
#define SRC_WRAPPER_POINTER_LISTENER_HPP__

#include "wayland_client.hpp"

class WaylandClient::PointerListener : public Accessor {
  void handleEnter(wl_pointer* wl_pointer,
                   uint32_t serial,
                   wl_surface* surface,
                   wl_fixed_t surface_x,
                   wl_fixed_t surface_y);
  void handleLeave(wl_pointer* wl_pointer,
                   uint32_t serial,
                   wl_surface* surface);
  void handleMotion(wl_pointer* wl_pointer,
                    uint32_t time,
                    wl_fixed_t surface_x,
                    wl_fixed_t surface_y);
  void handleButton(wl_pointer* wl_pointer,
                    uint32_t serial,
                    uint32_t time,
                    uint32_t button,
                    uint32_t state);
  void handleAxis(wl_pointer* wl_pointer,
                  uint32_t time,
                  uint32_t axis,
                  wl_fixed_t value);
  void handleFrame(wl_pointer* wl_pointer);
  void handleAxisSource(wl_pointer* wl_pointer, uint32_t axis_source);
  void handleAxisStop(wl_pointer* wl_pointer, uint32_t time, uint32_t axis);
  void handleAxisDiscrete(wl_pointer* wl_pointer,
                          uint32_t axis,
                          int32_t discrete);

  static void recieveEnter(void* data,
                           wl_pointer* wl_pointer,
                           uint32_t serial,
                           wl_surface* surface,
                           wl_fixed_t surface_x,
                           wl_fixed_t surface_y) {
    static_cast<PointerListener*>(data)->handleEnter(
        wl_pointer, serial, surface, surface_x, surface_y);
  }
  static void recieveLeave(void* data,
                           wl_pointer* wl_pointer,
                           uint32_t serial,
                           wl_surface* surface) {
    static_cast<PointerListener*>(data)->handleLeave(wl_pointer, serial,
                                                     surface);
  }
  static void recieveMotion(void* data,
                            wl_pointer* wl_pointer,
                            uint32_t time,
                            wl_fixed_t surface_x,
                            wl_fixed_t surface_y) {
    static_cast<PointerListener*>(data)->handleMotion(wl_pointer, time,
                                                      surface_x, surface_y);
  }
  static void recieveButton(void* data,
                            wl_pointer* wl_pointer,
                            uint32_t serial,
                            uint32_t time,
                            uint32_t button,
                            uint32_t state) {
    static_cast<PointerListener*>(data)->handleButton(wl_pointer, serial, time,
                                                      button, state);
  }
  static void recieveAxis(void* data,
                          wl_pointer* wl_pointer,
                          uint32_t time,
                          uint32_t axis,
                          wl_fixed_t value) {
    static_cast<PointerListener*>(data)->handleAxis(wl_pointer, time, axis,
                                                    value);
  }
  static void recieveFrame(void* data, wl_pointer* wl_pointer) {
    static_cast<PointerListener*>(data)->handleFrame(wl_pointer);
  }
  static void recieveAxisSource(void* data,
                                wl_pointer* wl_pointer,
                                uint32_t axis_source) {
    static_cast<PointerListener*>(data)->handleAxisSource(wl_pointer,
                                                          axis_source);
  }
  static void recieveAxisStop(void* data,
                              wl_pointer* wl_pointer,
                              uint32_t time,
                              uint32_t axis) {
    static_cast<PointerListener*>(data)->handleAxisStop(wl_pointer, time, axis);
  }
  static void recieveAxisDiscrete(void* data,
                                  wl_pointer* wl_pointer,
                                  uint32_t axis,
                                  int32_t discrete) {
    static_cast<PointerListener*>(data)->handleAxisDiscrete(wl_pointer, axis,
                                                            discrete);
  }

  static const wl_pointer_listener pointerListener_;

 public:
  static const wl_pointer_listener* pointerListener() {
    return &pointerListener_;
  }

  PointerListener(WaylandClient* wc) : Accessor(wc) {}
};

#endif  // SRC_WRAPPER_POINTER_LISTENER_HPP__
