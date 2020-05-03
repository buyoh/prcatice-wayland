#ifndef SRC_WRAPPER_SHELL_SURFACE_LISTENER_HPP__
#define SRC_WRAPPER_SHELL_SURFACE_LISTENER_HPP__

#include "./wayland_client.hpp"

class WaylandClient::ShellSurfaceListener : public Accessor {
  wl_shell_surface_listener shellSurfaceListener_;

  void handlePing(wl_shell_surface* shell_surface, uint32_t serial);
  void handleConfigure(wl_shell_surface* shell_surface,
                       uint32_t edges,
                       int32_t width,
                       int32_t height);
  void handlePopupDone(wl_shell_surface* shell_surface);

  static void recievePing(void* data,
                          wl_shell_surface* shell_surface,
                          uint32_t serial) {
    static_cast<ShellSurfaceListener*>(data)->handlePing(shell_surface, serial);
  }
  static void recieveConfigure(void* data,
                               wl_shell_surface* shell_surface,
                               uint32_t edges,
                               int32_t width,
                               int32_t height) {
    static_cast<ShellSurfaceListener*>(data)->handleConfigure(
        shell_surface, edges, width, height);
  }
  static void recievePopupDone(void* data, wl_shell_surface* shell_surface) {
    static_cast<ShellSurfaceListener*>(data)->handlePopupDone(shell_surface);
  }

 public:
  wl_shell_surface_listener* getShellSurfaceListener() {
    return &shellSurfaceListener_;
  }

  ShellSurfaceListener(WaylandClient* wc)
      : Accessor(wc),
        shellSurfaceListener_(
            {recievePing, recieveConfigure, recievePopupDone}) {}
};

#endif  // SRC_WRAPPER_SHELL_SURFACE_LISTENER_HPP__
