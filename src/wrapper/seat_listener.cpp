#include "./seat_listener.hpp"

#include "./util/log.h"

void WaylandClient::SeatListener::handleSeatCapabilities(
    wl_seat* seat,
    uint32_t capabilities) {
  VLOG << "handleSeatCapabilities: ";
  return;
}

void WaylandClient::SeatListener::handleSeatName(wl_seat* seat,
                                                 const char* name) {
  VLOG << "seatname: " << name;
  return;
}

const wl_seat_listener WaylandClient::SeatListener::registryListener_{
    recieveSeatCapabilities, recieveSeatName};
