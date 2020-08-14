#ifndef SRC_WRAPPER_TOUCH_LISTENER_HPP__
#define SRC_WRAPPER_TOUCH_LISTENER_HPP__

#include "./wayland_client.hpp"

class WaylandClient::TouchListener : public Accessor {
  void handleDown(wl_touch* wl_touch,
                  uint32_t serial,
                  uint32_t time,
                  wl_surface* surface,
                  int32_t id,
                  wl_fixed_t x,
                  wl_fixed_t y);
  void handleUp(wl_touch* wl_touch, uint32_t serial, uint32_t time, int32_t id);
  void handleMotion(wl_touch* wl_touch,
                    uint32_t time,
                    int32_t id,
                    wl_fixed_t x,
                    wl_fixed_t y);
  void handleFrame(wl_touch* wl_touch);
  void handleCancel(wl_touch* wl_touch);
  void handleShape(wl_touch* wl_touch,
                   int32_t id,
                   wl_fixed_t major,
                   wl_fixed_t minor);
  void handleOrientation(wl_touch* wl_touch,
                         int32_t id,
                         wl_fixed_t orientation);

  static void recieveDown(void* data,
                          wl_touch* wl_touch,
                          uint32_t serial,
                          uint32_t time,
                          wl_surface* surface,
                          int32_t id,
                          wl_fixed_t x,
                          wl_fixed_t y) {
    static_cast<TouchListener*>(data)->handleDown(wl_touch, serial, time,
                                                  surface, id, x, y);
  }
  static void recieveUp(void* data,
                        wl_touch* wl_touch,
                        uint32_t serial,
                        uint32_t time,
                        int32_t id) {
    static_cast<TouchListener*>(data)->handleUp(wl_touch, serial, time, id);
  }
  static void recieveMotion(void* data,
                            wl_touch* wl_touch,
                            uint32_t time,
                            int32_t id,
                            wl_fixed_t x,
                            wl_fixed_t y) {
    static_cast<TouchListener*>(data)->handleMotion(wl_touch, time, id, x, y);
  }
  static void recieveFrame(void* data, wl_touch* wl_touch) {
    static_cast<TouchListener*>(data)->handleFrame(wl_touch);
  }
  static void recieveCancel(void* data, wl_touch* wl_touch) {
    static_cast<TouchListener*>(data)->handleCancel(wl_touch);
  }
  static void recieveShape(void* data,
                           wl_touch* wl_touch,
                           int32_t id,
                           wl_fixed_t major,
                           wl_fixed_t minor) {
    static_cast<TouchListener*>(data)->handleShape(wl_touch, id, major, minor);
  }
  static void recieveOrientation(void* data,
                                 wl_touch* wl_touch,
                                 int32_t id,
                                 wl_fixed_t orientation) {
    static_cast<TouchListener*>(data)->handleOrientation(wl_touch, id,
                                                         orientation);
  }

  static const wl_touch_listener touchListener_;

 public:
  static const wl_touch_listener* shellSurfaceListener() {
    return &touchListener_;
  }

  TouchListener(WaylandClient* wc) : Accessor(wc) {}
};

#endif  // SRC_WRAPPER_TOUCH_LISTENER_HPP__
