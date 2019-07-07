#ifndef BSTD_ERROR_HPP_
#define BSTD_ERROR_HPP_

#include <stdexcept>
#include <string>

namespace bstd::error {

/// \brief Base class to help us throw effective errors.
class error : public std::runtime_error {

  public:

    /// \brief Explicit constructor that helps contextualize the error by adding
    /// the location from which the error occured.
    /// \param _where some information on where the error occured
    /// \param _what what is the error
    explicit error(const std::string& _where, const std::string& _what)
      : std::runtime_error("\nbstd::error\n  Where: " + _where + ".\n  What: " \
          + _what + ".\n") {}

};

}

#endif
