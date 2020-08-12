#include "./wayland_client.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

#include "./util/log.h"

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

static const char* getShmFilepath() {
  static std::string buffer;
  if (buffer.empty()) {
    // char* xdg = getenv("XDG_RUNTIME_DIR");
    // if (xdg) {
    //   buffer += xdg;
    //   buffer += "/wayland_client_shm.tmp";
    // } else {
    //   std::clog << "environment variable XDG_RUNTIME_DIR is undefined. \n"
    //                "use '/tmp/wayland_client_shm.tmp'."
    //             << std::endl;
    //   buffer = "/tmp/wayland_client_shm.tmp";
    // }
    buffer = "/tmp/wayland_client_shm.tmp";
  }
  return buffer.c_str();
}

void WaylandClient::createShmBuffer() {
  const int stride = width_ * 4;
  const int size = stride * height_;

  const char* shmFilepath = getShmFilepath();
  remove(shmFilepath);  // dangerous
  int fd = open(shmFilepath, O_RDWR | O_EXCL | O_CREAT, 0600);
  if (fd == -1) {
    perror("createShmBuffer: open failed");
    exit(1);
  }

  ftruncate(fd, size);

  // TODO: 何故mmap？
  // bnufferとdataを紐づける辺りもイメージ出来ていない
  void* d = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (d == MAP_FAILED) {
    perror("createShmBuffer: mmap failed");
    close(fd);
    exit(1);
  }
  data_ = static_cast<uint32_t*>(d);

  wl_shm_pool* pool = wl_shm_create_pool(shm_, fd, size);
  buffer_ = wl_shm_pool_create_buffer(pool, 0, width_, height_, stride,
                                      WL_SHM_FORMAT_ARGB8888);
  wl_shm_pool_destroy(pool);

  close(fd);
}

//

void WaylandClient::initialize() {
  registryListener_ = std::make_unique<RegistryListener>(this);
  shellSurfaceListener_ = std::make_unique<ShellSurfaceListener>(this);
  keyboardListener_ = std::make_unique<KeyboardListener>(this);
  pointerListener_ = std::make_unique<PointerListener>(this);
  touchListener_ = std::make_unique<TouchListener>(this);

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
  wl_registry_add_listener(registry_, RegistryListener::registryListener(),
                           registryListener_.get());

  // 3
  // displayに溜まったeventをflushする
  // この辺りで、registryLisnerが値を受け取り変数に格納する
  wl_display_roundtrip(display_);
  // requestが処理されるまでブロックする
  wl_display_dispatch(display_);

  // 4
  // compositorからsurfaceを取得する
  surface_ = wl_compositor_create_surface(compositor_);
  // マウスカーソルやサイズ変更居等のアプリに
  // 最低限必要なインターフェースを実装するものがshell
  shell_surface_ = wl_shell_get_shell_surface(shell_, surface_);

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
  // 適当な色で塗りつぶし
  {
    // ((a << 24) | (r << 16) | (g << 8) | b)
    for (int cnt = width_ * height_; --cnt >= 0;)
      *(data_ + cnt) = 0;
  }

  // 8
  redraw();
}

//

void WaylandClient::destroy() {
  if (display_)
    wl_display_disconnect(display_), display_ = nullptr;
  remove(getShmFilepath());
}

//

int WaylandClient::displayDispatch() {
  return wl_display_dispatch(display_);
}

//

void WaylandClient::redraw(int x, int y, int width, int height) {
  // bufferをsurfaceの座標0,0に関連付ける
  wl_surface_attach(surface_, buffer_, 0, 0);
  // 再描画範囲指定
  wl_surface_damage(surface_, (int32_t)x, (int32_t)y, (int32_t)width,
                    (int32_t)height);
  // 依頼
  wl_surface_commit(surface_);
}
