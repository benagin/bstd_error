#ifndef BSTD_CONTEXT_ERROR_HPP_
#define BSTD_CONTEXT_ERROR_HPP_

#include <cmath>

#include "error.hpp"

namespace bstd::error {

/// \brief Error helper for errors within a larger string.
/// Here, context is a string that contains an error.
/// This is useful for things like parsing where you want to indicate the cause
/// of the error within a string.
class context_error : public error {

  public:

    /// Shorthand for string const iterator.
    typedef std::string::const_iterator CSIT;

  private:

    /// \brief Trim context to keep it under _max_context_size.
    /// \param _context          the context string
    /// \param _max_context_size the maximum size of the context string before
    ///                          we trim it
    /// \param context_error     an object so this friend class works (unused)
    /// \return a trimmed string
    friend const auto trim(const std::string& _context,
        const context_error*, const std::size_t _max_context_size = 50) {
      const auto size = _context.size();
      if(size < _max_context_size)
        return _context;

      const auto& size_to_trim = std::ceil((size - _max_context_size) / 2);

      const std::string ellipsis = "...";

      std::string trimmed = "";

      trimmed = _context.substr(size_to_trim, size - size_to_trim);
      trimmed = ellipsis + trimmed + ellipsis;

      return trimmed;
    }

    /// \brief Same as mark_char, but marks a string.
    /// This causes undefined behavior if _start and _last are not valid (do not
    /// point to _context).
    /// \param _start            iterator to the start of the string
    /// \param _last             iterator to the last character of the string
    /// \param _context          the context that contains the string
    /// \param _max_context_size the maximum size of the context string before
    ///                          we trim it
    /// \param context_erorr     an object so this friend class works (unused).
    /// \return a string with a marked substring
    friend const auto mark_string(const CSIT& _start, const CSIT& _last,
        const std::string& _context, const context_error* _ce,
        const std::size_t _max_context_size = 50) {
      if(_context.empty())
        return std::string();

      auto context_copy = _context;
      const auto& start = context_copy.cbegin() +
                          std::distance(_context.cbegin(), _start),
                  end = context_copy.cbegin() +
                        std::distance(_context.cbegin(), _last);

      context_copy.insert(start, {' ', '>', ' '});
      context_copy.insert(end + 3, {' ', '<', ' '});

      return trim(context_copy, _ce, _max_context_size);
    }

    /// \brief Marks a character in a string with angle brackets (>, <).
    /// Example: 'context > t < is misspelled'.
    /// Only return ample context for the character.
    /// This causes undefined behavior if _csit is not valid (does not point to
    /// _context).
    /// \param _csit             the character to mark
    /// \param _context          the context containing the character
    /// \param _max_context_size the maximum size of the context string before
    ///                          we trim it
    /// \param _ce               an object so this friend class works (unused)
    /// \return                  a string with a marked character
    friend const auto mark_char(const CSIT& _csit,
        const std::string& _context, const context_error* _ce,
        const std::size_t _max_context_size = 50) {
      return mark_string(_csit, _csit + 1, _context, _ce, _max_context_size);
    }

    /// \brief Safely construct a std::string with bidirectional iterators.
    /// Without this the normal std::string constructor will throw
    /// std::length_error if _start > _last since std::string's constructor
    /// takes InputIterators.
    /// \param _context container string for _start
    /// \param _start        iterator to the start of the string
    /// \param _last         iterator to the last character of the string
    /// \param context_error an object so this friend class works (unused).
    /// \return a string constructed from the value pointed to by (_start, _last]
    friend const auto safe_construct_string(const std::string& _context,
        const CSIT& _start, const CSIT& _last, const context_error*) noexcept {
      if(_context.empty() or _last == std::cend(_context)
          or _start == std::cend(_context))
        return std::string();

      auto last_copy = _last;

      if(_start <= _last)
        return std::string(_start, _last);

      auto constructed = std::string();
      while(last_copy != _start) {
        constructed += *last_copy;
        last_copy++;
      }

      return constructed;
    }

    /// \brief Safely construct a std::string from an iterator.
    /// \param _context container string for _start
    /// \param _start iterator to the character
    /// \param _ce    an object so this friend class works (unused).
    /// \param context_error an object so this friend class works (unused).
    /// \return a string constructed from the value pointed to by _start
    friend const auto safe_construct_string(const std::string& _context,
        const CSIT& _start, const context_error* _ce) noexcept {
      return safe_construct_string(_context, _start, _start, _ce);
    }

  public:

    /// \brief Explicit constructor that follows the same pattern as error.
    explicit context_error(const std::string& _where, const std::string& _what)
        : error(_where, _what) {}

    /// \brief Report error for a character in a context.
    /// Currently, this has a side effect of modifying the _context
    /// parameter.
    /// \param _context context that contains the error
    /// \param _csit    iterator to the bad character
    /// \param _what    what went wrong
    /// \param _max_context_size The maximum size of the context string before
    ///                          we trim it
    explicit context_error(const std::string& _context, const CSIT& _csit,
        const std::string& _what, const std::size_t _max_context_size = 50)
        : error("character '" + safe_construct_string(_context, _csit, this)
          + "' in context '" +
          mark_char(_csit, _context, this, _max_context_size) + "'", _what) {}

    /// \brief Report error for a string in a context.
    /// Currently, this has a side effect of modifying the _context
    /// parameter.
    /// \param _context the larger expression that contains the bad character
    /// \param _start   iterator to the start of the bad string
    /// \param _last    iterator to the character after the last character of
    ///                 the bad string
    /// \param _what    what went wrong
    /// \param _max_context_size The maximum size of the context string before
    ///                          we trim it
    explicit context_error(const std::string& _context, const CSIT& _start,
        const CSIT& _last, const std::string& _what,
        const std::size_t _max_context_size = 50)
        : error("string '" + safe_construct_string(_context, _start, _last, this)
          + "' in context '" +
          mark_string(_start, _last, _context, this, _max_context_size) + "'",
          _what) {}

};

}

#endif
