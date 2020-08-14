#ifndef SRC_WRAPPER_SEAT_LISTENER_HPP__
#define SRC_WRAPPER_SEAT_LISTENER_HPP__

#include "./wayland_client.hpp"

class WaylandClient::SeatListener : public Accessor {
  void handleSeatCapabilities(wl_seat* seat, uint32_t capabilities);
  void handleSeatName(wl_seat* seat, const char* name);

  static void recieveSeatCapabilities(void* data,
                                      wl_seat* seat,
                                      uint32_t capabilities) {
    static_cast<SeatListener*>(data)->handleSeatCapabilities(seat,
                                                             capabilities);
  }
  static void recieveSeatName(void* data, wl_seat* seat, const char* name) {
    static_cast<SeatListener*>(data)->handleSeatName(seat, name);
  }

  static const wl_seat_listener registryListener_;

 public:
  static const wl_seat_listener* getSeatListener() {
    return &registryListener_;
  }

  SeatListener(WaylandClient* wc) : Accessor(wc) {}
};

const wl_seat_listener WaylandClient::SeatListener::registryListener_{
    recieveSeatCapabilities, recieveSeatName};

#endif  // SRC_WRAPPER_SEAT_LISTENER_HPP__
