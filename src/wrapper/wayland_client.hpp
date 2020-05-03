#ifndef SRC_WRAPPER_WAYLAND_CLIENT_HPP__
#define SRC_WRAPPER_WAYLAND_CLIENT_HPP__

#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

#include <cstdint>
#include <memory>

//

// WaylandClient
// ============================================================================

class WaylandClient {
  //
  // for access WaylandClient member variables
  class Accessor {
    WaylandClient* owner_ = nullptr;

   protected:
    WaylandClient& owner() { return *owner_; }
    const WaylandClient& owner() const { return *owner_; }

   public:
    void detach() { owner_ = nullptr; }
    bool detached() const { return owner_ != nullptr; }
    Accessor(WaylandClient* _owner) : owner_(_owner) {}
    ~Accessor() { detach(); }
  };

  //

 public:
  class RegistryListener;
  class ShellSurfaceListener;
  class KeyboardListener;
  class PointerListener;
  class TouchListener;

  ~WaylandClient();

 private:
  wl_display* display_;
  wl_registry* registry_;
  wl_compositor* compositor_;
  wl_buffer* buffer_;
  wl_surface* surface_;
  wl_shm* shm_;
  wl_shell* shell_;
  wl_shell_surface* shell_surface_;
  wl_seat* seat_;
  std::unique_ptr<RegistryListener> registryListener_;
  std::unique_ptr<ShellSurfaceListener> shellSurfaceListener_;
  std::unique_ptr<KeyboardListener> keyboardListener_;
  std::unique_ptr<PointerListener> pointerListener_;
  std::unique_ptr<TouchListener> touchListener_;
  uint32_t* data_;  // for shm buffer
  int width_, height_;

  void createShmBuffer();

 public:
  void initialize();
  void destroy();

  const uint32_t* data() const { return data_; }
  uint32_t* data() { return data_; }

  int displayDispatch();
  void redraw(int _x, int _y, int _width, int _height);
  void redraw() { redraw(0, 0, width_, height_); }

  WaylandClient(int _width, int _height) : width_(_width), height_(_height) {}
};

// TODO: bad workaround
#include "./keyboard_listener.hpp"
#include "./pointer_listener.hpp"
#include "./registry_listener.hpp"
#include "./shellsurface_listener.hpp"
#include "./touch_listener.hpp"

#endif  // SRC_WRAPPER_WAYLAND_CLIENT_HPP__
