#ifndef BSTD_ERROR_HPP_
#define BSTD_ERROR_HPP_

#include <iostream>
#include <stdexcept>
#include <string>

namespace bstd::error {

/// \brief Base class to help us throw effective errors.
class error : public std::runtime_error {

  public:

    /// \brief Explicit constructor that mirrors base class.
    /// \param _what what is the error
    explicit error(const std::string& _what)
        : error("", _what) {}

    /// \brief Explicit constructor that helps contextualize the error by adding
    /// the location from which the error occured.
    /// \param _where some information on where the error occured
    /// \param _what what is the error
    explicit error(const std::string& _where, const std::string& _what)
        : std::runtime_error(_what), m_where(_where) {}

    /// \breif Return a contextualized error message.
    /// \return formatted error message
    virtual const char* what() const noexcept override {
      const std::string where_prefix = "\n" + get_name() + "\n  Where: ";
      const std::string where_suffix = ".\n  What: "
          + std::string(std::runtime_error::what());

      auto where = m_where;
      where.insert(0, where_prefix);
      where.insert(where.end(), where_suffix.cbegin(), where_suffix.cend());

      return where.data();
    }

  protected:

    /// \brief Get the classes name.
    /// \return the classes name
    virtual const std::string& get_name() const noexcept;

  private:

    std::string m_where;
    static inline const std::string m_name{"bstd::error"};


};


const std::string&
error::
get_name() const noexcept {
  return m_name;
}


}

#endif
