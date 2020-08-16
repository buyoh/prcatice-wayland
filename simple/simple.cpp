#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

// #include <stdexcept>
// #include <memory>
#include <bits/stdc++.h>

// 以下のサイトを参考に作成。
// https://devm33.hatenadiary.org/entry/20140422/1398182440

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
  void* data_;
  int width_, height_;

  void createShmBuffer();

 public:
  void initialize();
  void destroy();
  int displayDispatch();
  WaylandClient(int _width, int _height) : width_(_width), height_(_height) {}
  ~WaylandClient();
};

//

void WaylandClient::RegistryListener::handleGlobal(struct wl_registry* registry,
                                                   uint32_t name,
                                                   const char* interface,
                                                   uint32_t version) {
  printf("RegistryListener: global: interface=%s name=%0x version=%d\n",
         interface, name, version);
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

void WaylandClient::RegistryListener::handleGlobalRemove(wl_registry* registry,
                                                         uint32_t name) {
  printf("RegistryListener: global_remove: name=%0x\n", name);
}

//

void WaylandClient::ShellSurfaceListener::handlePing(
    wl_shell_surface* shell_surface,
    uint32_t serial) {
  printf("ShellSurfaceListener: ping: serial=%d\n", serial);
  wl_shell_surface_pong(shell_surface, serial);
}
void WaylandClient::ShellSurfaceListener::handleConfigure(
    wl_shell_surface* shell_surface,
    uint32_t edges,
    int32_t width,
    int32_t height) {
  printf("ShellSurfaceListener: configure: edges=%0x, width=%d, height=%d\n",
         edges, width, height);
}

void WaylandClient::ShellSurfaceListener::handlePopupDone(
    wl_shell_surface* shell_surface) {
  printf("ShellSurfaceListener: popup_done: \n");
}

//

WaylandClient::~WaylandClient() {
  // ????
  // if (shell_surface_ != nullptr)
  //   wl_shell_surface_destroy(shell_surface_), shell_surface_ = nullptr;
  // if (surface_ != nullptr)
  //   wl_surface_destroy(surface_), surface_ = nullptr;
  // if (shell_ != nullptr)
  //   wl_shell_destroy(shell_), shell_ = nullptr;
  // if (compositor_ != nullptr)
  //   wl_compositor_destroy(compositor_), compositor_ = nullptr;
  // if (registry_ != nullptr)
  //   wl_registry_destroy(registry_), registry_ != nullptr;
  // const int stride = width_ * 4;
  // const int size = stride * height_;
  // if (data_ != nullptr)
  //   munmap(data_, size);  // ???
  // ?????
  // :thinking_face:
}

//

void WaylandClient::createShmBuffer() {
  const int stride = width_ * 4;
  const int size = stride * height_;

  int fd = open("shm.tmp", O_RDWR | O_CREAT);
  if (fd == -1) {
    fprintf(stderr, "%s: open failed: %m\n", __FUNCTION__);
    exit(1);
  }

  ftruncate(fd, size);
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
  display_ = wl_display_connect(nullptr);
  if (!display_)
    throw std::runtime_error("Cannot connect to Wayland display\n");

  // 2
  // registry: compositorのオブジェクトを取得するためのもの？
  registry_ = wl_display_get_registry(display_);
  if (!registry_)
    throw std::runtime_error("Cannot get registry from Wayland display\n");

  // RegistryListenerでオブジェクトを受け取る
  wl_registry_add_listener(registry_, registryListener_->getRegistryListener(),
                           registryListener_.get());

  // 3
  wl_display_roundtrip(display_);  // displayに溜まったeventをflushする
  wl_display_dispatch(display_);  // requestが処理されるまでブロックする

  // 4
  // compositorからsurfaceを取得する
  surface_ = wl_compositor_create_surface(compositor_);
  // マウスカーソルやサイズ変更居等のアプリに
  // 最低限必要なインターフェースを実装するものがshell
  shell_surface_ = wl_shell_get_shell_surface(shell_, surface_);

  // TODO: 何故mmap？
  // bnufferとdataを紐づける辺りもイメージ出来ていない
  createShmBuffer();

  // 5
  if (shell_surface_) {
    // プログラムが応答可能かどうか、
    // マウスカーソルが領域に入った時にpingが飛んでくるらしい
    //
    wl_shell_surface_add_listener(
        shell_surface_, shellSurfaceListener_->getShellSurfaceListener(),
        shellSurfaceListener_.get());
    wl_shell_surface_set_toplevel(shell_surface_);
  }

  // 6
  // なにこれ？
  wl_surface_set_user_data(surface_, this);
  wl_shell_surface_set_title(shell_surface_, "wayland-client");

  // 7
  draw_argb8888(data_, 0x00, 0xf0, 0x80, 0x00, width_ * height_);

  // 8
  // bufferをsurfaceの座標0,0に関連付ける
  wl_surface_attach(surface_, buffer_, 0, 0);
  // 再描画範囲指定
  wl_surface_damage(surface_, 0, 0, width_, height_);
  // 依頼
  wl_surface_commit(surface_);
}

void WaylandClient::destroy() {
  if (display_)
    wl_display_disconnect(display_), display_ = nullptr;
}

//

int WaylandClient::displayDispatch() {
  return wl_display_dispatch(display_);
}

//

WaylandClient wc(320, 240);
void handleTrap(int sig) {
  wc.destroy();
}

int main(int argc, char** argv) {
  wc.initialize();
  signal(SIGINT, handleTrap);
  while (wc.displayDispatch() != -1)
    std::cout << "loop" << std::endl;
  wc.destroy();
  std::cout << "EXIT SUCCESS" << std::endl;
  return 0;
}
