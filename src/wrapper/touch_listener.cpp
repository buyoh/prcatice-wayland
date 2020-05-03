#include "./touch_listener.hpp"

#include "./util/log.h"

void WaylandClient::TouchListener ::handleDown(wl_touch* wl_touch,
                                               uint32_t serial,
                                               uint32_t time,
                                               wl_surface* surface,
                                               int32_t id,
                                               wl_fixed_t x,
                                               wl_fixed_t y) {
  VLOG;
}

void WaylandClient::TouchListener ::handleUp(wl_touch* wl_touch,
                                             uint32_t serial,
                                             uint32_t time,
                                             int32_t id) {
  VLOG;
}

void WaylandClient::TouchListener ::handleMotion(wl_touch* wl_touch,
                                                 uint32_t time,
                                                 int32_t id,
                                                 wl_fixed_t x,
                                                 wl_fixed_t y) {
  VLOG;
}

void WaylandClient::TouchListener ::handleFrame(wl_touch* wl_touch) {
  VLOG;
}

void WaylandClient::TouchListener ::handleCancel(wl_touch* wl_touch) {
  VLOG;
}

void WaylandClient::TouchListener ::handleShape(wl_touch* wl_touch,
                                                int32_t id,
                                                wl_fixed_t major,
                                                wl_fixed_t minor) {
  VLOG;
}

void WaylandClient::TouchListener ::handleOrientation(wl_touch* wl_touch,
                                                      int32_t id,
                                                      wl_fixed_t orientation) {
  VLOG;
}
