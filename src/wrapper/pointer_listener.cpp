#include "./pointer_listener.hpp"

#include "./util/log.h"

void WaylandClient::PointerListener::handleEnter(wl_pointer* wl_pointer,
                                                 uint32_t serial,
                                                 wl_surface* surface,
                                                 wl_fixed_t surface_x,
                                                 wl_fixed_t surface_y) {
  VLOG;
}

void WaylandClient::PointerListener::handleLeave(wl_pointer* wl_pointer,
                                                 uint32_t serial,
                                                 wl_surface* surface) {
  VLOG;
}

void WaylandClient::PointerListener::handleMotion(wl_pointer* wl_pointer,
                                                  uint32_t time,
                                                  wl_fixed_t surface_x,
                                                  wl_fixed_t surface_y) {
  VLOG;
}

void WaylandClient::PointerListener::handleButton(wl_pointer* wl_pointer,
                                                  uint32_t serial,
                                                  uint32_t time,
                                                  uint32_t button,
                                                  uint32_t state) {
  VLOG;
}

void WaylandClient::PointerListener::handleAxis(wl_pointer* wl_pointer,
                                                uint32_t time,
                                                uint32_t axis,
                                                wl_fixed_t value) {
  VLOG;
}

void WaylandClient::PointerListener::handleFrame(wl_pointer* wl_pointer) {
  // NOTE:
  // 他のハンドラで情報を受け取りどこかに退避してから、
  // このハンドラでログを出したり反映したりするらしい
  // wl_pointer_listenerの定義元を参照。
  VLOG;
}

void WaylandClient::PointerListener::handleAxisSource(wl_pointer* wl_pointer,
                                                      uint32_t axis_source) {
  VLOG;
}

void WaylandClient::PointerListener::handleAxisStop(wl_pointer* wl_pointer,
                                                    uint32_t time,
                                                    uint32_t axis) {
  VLOG;
}

void WaylandClient::PointerListener::handleAxisDiscrete(wl_pointer* wl_pointer,
                                                        uint32_t axis,
                                                        int32_t discrete) {
  VLOG;
}

const wl_pointer_listener WaylandClient::PointerListener::pointerListener_{
    recieveEnter,      recieveLeave,    recieveMotion,
    recieveButton,     recieveAxis,     recieveFrame,
    recieveAxisSource, recieveAxisStop, recieveAxisDiscrete};
