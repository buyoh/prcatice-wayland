#include "./shellsurface_listener.hpp"

#include "./util/log.h"

//

void WaylandClient::ShellSurfaceListener::handlePing(
    wl_shell_surface* shell_surface,
    uint32_t serial) {
  VLOG << "ping: serial=" << serial;
  wl_shell_surface_pong(shell_surface, serial);
}

//

void WaylandClient::ShellSurfaceListener::handleConfigure(
    wl_shell_surface* shell_surface,
    uint32_t edges,
    int32_t width,
    int32_t height) {
  VLOG << "configure: edges=" << edges << " width=" << width
       << " height=" << height;
}

//

void WaylandClient::ShellSurfaceListener::handlePopupDone(
    wl_shell_surface* shell_surface) {
  VLOG << "popup_done: ";
}

const wl_shell_surface_listener
    WaylandClient::ShellSurfaceListener::shellSurfaceListener_{
        recievePing, recieveConfigure, recievePopupDone};
