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
  //
  // wl_registry_listener wrapper
  class RegistryListener : public Accessor {
    wl_registry_listener registryListener_;

    void handleGlobal(wl_registry* registry,
                      uint32_t name,
                      const char* interface,
                      uint32_t version);
    void handleGlobalRemove(wl_registry* registry, uint32_t name);

    static void recieveGlobal(void* data,
                              wl_registry* registry,
                              uint32_t name,
                              const char* interface,
                              uint32_t version) {
      static_cast<RegistryListener*>(data)->handleGlobal(registry, name,
                                                         interface, version);
    }
    static void recieveGlobalRemove(void* data,
                                    wl_registry* registry,
                                    uint32_t name) {
      static_cast<RegistryListener*>(data)->handleGlobalRemove(registry, name);
    }

   public:
    wl_registry_listener* getRegistryListener() { return &registryListener_; }

    RegistryListener(WaylandClient* wc)
        : Accessor(wc),
          registryListener_({recieveGlobal, recieveGlobalRemove}) {}
  };

  //
  // wl_shell_surface_listener wrapper
  class ShellSurfaceListener : public Accessor {
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
      static_cast<ShellSurfaceListener*>(data)->handlePing(shell_surface,
                                                           serial);
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

 private:
  wl_display* display_;
  wl_registry* registry_;
  wl_compositor* compositor_;
  wl_buffer* buffer_;
  wl_surface* surface_;
  wl_shm* shm_;
  wl_shell* shell_;
  wl_shell_surface* shell_surface_;
  std::unique_ptr<RegistryListener> registryListener_;
  std::unique_ptr<ShellSurfaceListener> shellSurfaceListener_;
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
  ~WaylandClient();
};

#endif  // SRC_WRAPPER_WAYLAND_CLIENT_HPP__
