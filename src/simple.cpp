#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

// #include <stdexcept>
// #include <memory>
#include <bits/stdc++.h>

//

class WaylandClient {
  //
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
  class RegistryListener : public Accessor {
    wl_registry_listener registryListener_;

    void handle(wl_registry* registry,
                uint32_t name,
                const char* interface,
                uint32_t version);

   public:
    static void recieve(void* data,
                        wl_registry* registry,
                        uint32_t name,
                        const char* interface,
                        uint32_t version) {
      static_cast<RegistryListener*>(data)->handle(registry, name, interface,
                                                   version);
    }
    wl_registry_listener* getRegistryListener() { return &registryListener_; }

    RegistryListener(WaylandClient* wc)
        : Accessor(wc), registryListener_({recieve, nullptr}) {}
  };

  //
  class ShellSurfaceListener : public Accessor {
    wl_shell_surface_listener shellSurfaceListener_;
    void handle(wl_shell_surface* shell_surface, uint32_t serial);

   public:
    static void recieve(void* data,
                        wl_shell_surface* shell_surface,
                        uint32_t serial) {
      static_cast<ShellSurfaceListener*>(data)->handle(shell_surface, serial);
    }
    wl_shell_surface_listener* getShellSurfaceListener() {
      return &shellSurfaceListener_;
    }

    ShellSurfaceListener(WaylandClient* wc)
        : Accessor(wc), shellSurfaceListener_({recieve, nullptr, nullptr}) {}
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
  void* data_;
  int width_, height_;

  void createShmBuffer();

 public:
  void initialize();
  int displayDispatch();
  WaylandClient(int _width, int _height) : width_(_width), height_(_height) {}
};

//

void WaylandClient::RegistryListener::handle(struct wl_registry* registry,
                                             uint32_t name,
                                             const char* interface,
                                             uint32_t version) {
  printf("interface=%s name=%0x version=%d\n", interface, name, version);
  if (strcmp(interface, "wl_compositor") == 0)
    owner().compositor_ = static_cast<wl_compositor*>(
        wl_registry_bind(registry, name, &wl_compositor_interface, 1));
  else if (strcmp(interface, "wl_shell") == 0)
    owner().shell_ = static_cast<wl_shell*>(
        wl_registry_bind(registry, name, &wl_shell_interface, 1));
  else if (strcmp(interface, "wl_shm") == 0)
    owner().shm_ = static_cast<wl_shm*>(
        wl_registry_bind(registry, name, &wl_shm_interface, 1));
}

//

void WaylandClient::ShellSurfaceListener::handle(
    struct wl_shell_surface* shell_surface,
    uint32_t serial) {
  wl_shell_surface_pong(shell_surface, serial);
}

//

void WaylandClient::createShmBuffer() {
  const int stride = width_ * 4;
  const int size = stride * height_;

  const int fd = open("shm.tmp", O_RDWR | O_CREAT);

  {
    // tekitou
    void* dummy = malloc(size);
    write(fd, dummy, size);
    free(dummy);
  }

  data_ = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (data_ == MAP_FAILED) {
    fprintf(stderr, "%s: mmap failed: %m\n", __FUNCTION__);
    close(fd);
    exit(1);
  }

  wl_shm_pool* pool = wl_shm_create_pool(shm_, fd, size);
  buffer_ = wl_shm_pool_create_buffer(pool, 0, width_, height_, stride,
                                      WL_SHM_FORMAT_ARGB8888);
  wl_shm_pool_destroy(pool);

  close(fd);
}

void draw_argb8888(void* d,
                   uint8_t a,
                   uint8_t r,
                   uint8_t g,
                   uint8_t b,
                   size_t count) {
  while (count-- > 0)
    *((uint32_t*)d + count) = ((a << 24) | (r << 16) | (g << 8) | b);
}

//

void WaylandClient::initialize() {
  registryListener_ = std::make_unique<WaylandClient::RegistryListener>(this);
  shellSurfaceListener_ =
      std::make_unique<WaylandClient::ShellSurfaceListener>(this);

  // 1
  display_ = wl_display_connect(NULL);
  if (!display_)
    throw std::runtime_error("Cannot connect to Wayland display\n");

  // 2
  registry_ = wl_display_get_registry(display_);
  if (!registry_)
    throw std::runtime_error("Cannot get registry from Wayland display\n");

  wl_registry_add_listener(registry_, registryListener_->getRegistryListener(),
                           registryListener_.get());

  // 3
  wl_display_roundtrip(display_);
  wl_display_dispatch(display_);

  // 4
  surface_ = wl_compositor_create_surface(compositor_);
  shell_surface_ = wl_shell_get_shell_surface(shell_, surface_);

  createShmBuffer();

  // 5
  if (shell_surface_) {
    wl_shell_surface_add_listener(
        shell_surface_, shellSurfaceListener_->getShellSurfaceListener(),
        shellSurfaceListener_.get());
    wl_shell_surface_set_toplevel(shell_surface_);
  }

  // 6
  wl_surface_set_user_data(surface_, this);
  wl_shell_surface_set_title(shell_surface_, "wayland-client");

  // 7
  draw_argb8888(data_, 0x00, 0xf0, 0x80, 0x00, width_ * height_);

  // 8
  wl_surface_attach(surface_, buffer_, 0, 0);
  wl_surface_damage(surface_, 0, 0, width_, height_);
  wl_surface_commit(surface_);
}

//

int WaylandClient::displayDispatch() {
  return wl_display_dispatch(display_);
}

//

int main(int argc, char** argv) {
  WaylandClient wc(320, 240);
  wc.initialize();
  while (wc.displayDispatch() != -1)
    ;
  exit(EXIT_SUCCESS);
}
