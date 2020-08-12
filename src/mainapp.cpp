#include <bits/stdc++.h>

#include "./wrapper/wayland_client.hpp"
using namespace std;

WaylandClient waylandClient(256, 256);
atomic_bool running{false};

void handleTrap(int sig) {
  running = false;
  waylandClient.destroy();
}

void render_main(uint32_t* data) {
  while (running) {
    for (int y = 0; y < 256; ++y) {
      for (int x = 0; x < 256; ++x) {
        data[y * 256 + x] = (x << 8) | y;
      }
    }
    waylandClient.redraw();
    this_thread::sleep_for(chrono::milliseconds(10));
  }
}

int main(int argc, char** argv) {
  waylandClient.initialize();

  running = true;
  signal(SIGINT, handleTrap);
  thread renderThread(render_main, waylandClient.data());

  while (waylandClient.displayDispatch() != -1) {
    // cout << "loop" << endl;
  }
  renderThread.join();

  waylandClient.destroy();
  cout << "exit 0" << endl;
  return 0;
}
