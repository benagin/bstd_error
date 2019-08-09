#include <iostream>
#include <string>

#include <bstd_error.hpp>

int main() {

  // Runtime error with some information on where the error occured.
  try {
    throw bstd::error::error("error_example1.cpp, main()", "Example error");
  } catch(const bstd::error::error& _e) {
    std::cout << _e.what() << std::endl;
  }

  // Context  error can be used to show what specific character caused an error
  // in some context.
  try {
    const std::string example_error = "examplle";
    const auto csit = example_error.cbegin() + 6;

    throw bstd::error::context_error(example_error, csit, "Misspelling");
  } catch(const bstd::error::error& _e) {
    std::cout << _e.what() << std::endl;
  }

  // Context error can also be used to mark substrings.
  try {
    const std::string example_error = "examplle";
    // Note: This is probably not a good way to find/capture errors in a string.
    // This is just done here for simplicity.
    const auto csit1 = example_error.cbegin() + 5;
    const auto csit2 = example_error.cbegin() + 7;

    throw bstd::error::context_error(example_error, csit1, csit2, "Misspelling");
  } catch(const bstd::error::error& _e) {
    std::cout << _e.what() << std::endl;
  }

  // Context error will trim portions of the context string if the size exceeds
  // MAX_CONTEXT_SIZE.

  // Trim from the end only.
  try {
    const std::string example_error
      = "This is a long sentience we can use to demonstrate trimming.";
    const auto csit = example_error.cbegin() + 19;

    throw bstd::error::context_error(example_error, csit, "Misspelling");
  } catch(const bstd::error::error& _e) {
    std::cout << _e.what() << std::endl;
  }

  // Trim from the beginning and end.
  try {
    const std::string example_error
      = "This is an even longer sentence we can use to demonstriate trimming from both ends of the string.";
    const auto csit = example_error.cbegin() + 54;

    throw bstd::error::context_error(example_error, csit, "Misspelling");
  } catch(const bstd::error::context_error& _e) {
    std::cout << _e.what() << std::endl;
  }
}
