/*
 * Controls the network side of things
 */

#pragma once

#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <netdb.h>
#include <memory>

namespace BDuplex {
  struct NetworkOptions {
    int backlog = 4096;
    const char* port = "6969";
  };

  template<bool SSL>
  class NetworkContext {
  public:
    explicit NetworkContext(const NetworkOptions& options = NetworkOptions{}) noexcept
      : m_fd(-1) {
      this->init(options);
    }

    ~NetworkContext() noexcept {
     // close(m_fd);
    }

    bool init(const NetworkOptions& options) noexcept {
      std::cout << "network init" << std::endl;
      addrinfo info = {};
      addrinfo* res = nullptr;

      info.ai_family = AF_INET6;
      info.ai_socktype = SOCK_STREAM;
      info.ai_flags = AI_PASSIVE;

      if (getaddrinfo(nullptr, options.port, &info, &res) != 0) {
        return false;
      }

      m_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
      if (m_fd == -1) {
        return false;
      }

      constexpr int no = 0;
      constexpr int yes = 1;

      if (setsockopt(m_fd, IPPROTO_IPV6, IPV6_V6ONLY, &no, sizeof(no)) == -1) {
        freeaddrinfo(res);
        return false;
      }

      if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        freeaddrinfo(res);
        return false;
      }

      if (bind(m_fd, res->ai_addr, res->ai_addrlen) == -1) {
        freeaddrinfo(res);
        return false;
      }

      freeaddrinfo(res);

      if (listen(m_fd, options.backlog) == -1) {
        return false;
      }

      return true;
    }

    void exit() noexcept {
      if (m_fd == -1) {
        close(m_fd);
      }
    }

    /*
     * Returns the native FD
     */
    [[nodiscard]] int getNativeFD() const noexcept {
      return m_fd;
    }

  private:
    int m_fd;
  };
}