#ifndef SRC_WRAPPER_REGISTRY_LISTENER_HPP__
#define SRC_WRAPPER_REGISTRY_LISTENER_HPP__

#include "./wayland_client.hpp"

class WaylandClient::RegistryListener : public Accessor {
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

  static const wl_registry_listener registryListener_;

 public:
  static const wl_registry_listener* registryListener() {
    return &registryListener_;
  }

  RegistryListener(WaylandClient* wc) : Accessor(wc) {}
};

const wl_registry_listener WaylandClient::RegistryListener::registryListener_{
    recieveGlobal, recieveGlobalRemove};

#endif  // SRC_WRAPPER_REGISTRY_LISTENER_HPP__
