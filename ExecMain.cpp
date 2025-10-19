#include <iostream>
#include "src/BDuplex.h"
#include <memory>

int main() {
  constexpr BDuplex::AppOptions options = {
    .key_file_path = "",
    .cert_file_path = "HI",
    .key_password = "",
  };

  const std::unique_ptr<BDuplex::SSLApp> app = std::make_unique<BDuplex::SSLApp>(options);

  if (app->failed()) {
    std::terminate();
  }

  app->run();

  return EXIT_SUCCESS;
}