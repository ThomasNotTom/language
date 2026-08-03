#include <catch2/catch_test_macros.hpp>

#include "generation/generator.hpp"
#include "io/file_reader.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_test_failure_exception.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <csignal>
#include <cstdlib>
#include <filesystem>

class CompilerBuildListener : public Catch::EventListenerBase {
public:
  using Catch::EventListenerBase::EventListenerBase;
  void testRunStarting(const Catch::TestRunInfo& testRunInfo) override {
    if (std::filesystem::exists("./bin/main")) {
      return;
    }
    std::cerr << "Compiler not found";
    std::exit(1);
  }
};

CATCH_REGISTER_LISTENER(CompilerBuildListener)

std::string exec(std::string cmd) {
  std::array<char, 128> buffer;
  std::string result;

  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"),
                                                pclose);

  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

void checkOutput(std::string filePath, std::string expectedOut) {}

TEST_CASE("Run \"./examples/addition.lang\"", "[system]") {
  std::system("./bin/main ./examples/addition.lang");
  int out = std::system("./main.out");
  REQUIRE(out != -1);
  REQUIRE(WIFEXITED(out));
  REQUIRE(WEXITSTATUS(out) == 5);
};

TEST_CASE("Run \"./examples/integer_primitives.lang\"", "[system]") {
  std::system("./bin/main ./examples/integer_primitives.lang");
  int out = std::system("./main.out");
  REQUIRE(out != -1);
  REQUIRE(WIFEXITED(out));
  REQUIRE(WEXITSTATUS(out) == 0);
};

TEST_CASE("Run \"./examples/print.lang\"", "[system]") {
  std::system("./bin/main ./examples/print.lang");
  std::string out = exec("./main.out");

  REQUIRE(out == "1\n2\n");
};

TEST_CASE("Run \"./examples/transitive_assignment.lang\"", "[system]") {
  std::system("./bin/main ./examples/transitive_assignment.lang");
  int out = std::system("./main.out");

  REQUIRE(out != -1);
  REQUIRE(WIFEXITED(out));
  REQUIRE(WEXITSTATUS(out) == 1);
};

TEST_CASE("Run \"./examples/variable_declaration.lang\"", "[system]") {
  std::system("./bin/main ./examples/variable_declaration.lang");
  int out = std::system("./main.out");

  REQUIRE(out != -1);
  REQUIRE(WIFEXITED(out));
  REQUIRE(WEXITSTATUS(out) == 1);
};
