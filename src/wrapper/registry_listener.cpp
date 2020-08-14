#include "./registry_listener.hpp"

#include <algorithm>
#include <cstring>

#include "./seat_listener.hpp"
#include "./util/log.h"

//

void WaylandClient::RegistryListener::handleGlobal(struct wl_registry* registry,
                                                   uint32_t name,
                                                   const char* interface,
                                                   uint32_t version) {
  VLOG << "global: interface=" << interface << " name=" << name
       << " version=" << version;

  const uint32_t useVersion = std::min<uint32_t>(1, version);

  if (strcmp(interface, "wl_compositor") == 0)
    owner().compositor_ = static_cast<wl_compositor*>(
        wl_registry_bind(registry, name, &wl_compositor_interface, useVersion));
  else if (strcmp(interface, "wl_shell") == 0)
    owner().shell_ = static_cast<wl_shell*>(
        wl_registry_bind(registry, name, &wl_shell_interface, useVersion));
  else if (strcmp(interface, "wl_shm") == 0)
    owner().shm_ = static_cast<wl_shm*>(
        wl_registry_bind(registry, name, &wl_shm_interface, useVersion));
  else if (strcmp(interface, "wl_seat") == 0) {
    owner().seat_ = static_cast<wl_seat*>(
        wl_registry_bind(registry, name, &wl_seat_interface, useVersion));
    // 受け取った直後で addlistenerしないと手遅れらしい
    wl_seat_add_listener(owner().seat_, SeatListener::seatListener(),
                         owner().seatListener_.get());

    // TODO: keep the useVersion for destructing wl_seat
    // https://aznote.jakou.com/prog/wayland/07.html
  }
}

//

void WaylandClient::RegistryListener::handleGlobalRemove(wl_registry* registry,
                                                         uint32_t name) {
  VLOG << "global_remove: name=" << name;
  printf("RegistryListener: global_remove: name=%0x\n", name);
}

const wl_registry_listener WaylandClient::RegistryListener::registryListener_{
    recieveGlobal, recieveGlobalRemove};
