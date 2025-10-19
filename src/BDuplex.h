#pragma once

#include <vector>

#include "Net.h"

namespace BDuplex {
  inline void hi() {
    std::cout << "Hello world\n";
  }

  struct AppOptions {
    std::string_view key_file_path;
    std::string_view cert_file_path;
    std::string_view key_password;
  };

  template<bool SSL>
  class TemplatedApp {
  public:
    explicit TemplatedApp(const AppOptions& options) noexcept : m_options(options) {
      // Init networking
    //  m_failed = m_network.init();
      std::cout << options.cert_file_path << "\n";
    };

    TemplatedApp(const TemplatedApp& other) noexcept = delete;

    TemplatedApp(TemplatedApp&& other) noexcept {

    }

    ~TemplatedApp() noexcept = default;

    TemplatedApp close() noexcept {
      m_network->exit();
    }
    /*
     * Has the app failed to initialize?
     */
    [[nodiscard]] bool failed() noexcept {
      return m_failed;
    }

    void run() noexcept {

    }

  private:
    bool m_failed;
    NetworkContext<SSL> m_network;
    AppOptions m_options;
    std::vector<void*> m_clients;
  };

  using SSLApp = TemplatedApp<true>;
  using App = TemplatedApp<false>;
}
