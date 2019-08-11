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
        : error(_where, _what, "bstd::error") {}

    /// \breif Return a contextualized error message.
    /// \return formatted error message
    inline virtual const char* what() const noexcept override {
      return m_where.data();
    }

  protected:

    /// \brief Explicit constructor that helps contextualize the error by adding
    /// the location from which the error occured.
    /// \param _where some information on where the error occured
    /// \param _what what is the error
    explicit error(const std::string& _where, const std::string& _what,
        const std::string& _name)
        : std::runtime_error(_what), m_where(_where) {
      const std::string where_prefix = "\n" + _name + "\n  Where: ";
      const std::string where_suffix = ".\n  What: "
          + std::string(std::runtime_error::what());

      m_where.insert(0, where_prefix);
      m_where.insert(m_where.end(), where_suffix.cbegin(), where_suffix.cend());
    }

  private:

    std::string m_where;

};


}

#endif
