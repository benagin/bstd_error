#include "json.hpp"

namespace bstd::json {

json::
json(const std::string& _string, const bool _debug) : m_debug(_debug) {
  if(m_debug)
    std::cout << "bstd::json" << std::endl;

  constexpr std::string_view dot_json{".json"};
  const auto extension =
    _string.substr(_string.size() - dot_json.length());

  auto json_string = _string;

  // Check if we have a JSON string or path.
  if(extension != dot_json) {
    // Try to open string as a path.

    // TODO: implement file size check.
    std::ifstream ifs(_string);

    if(!ifs.is_open())
      throw bstd::error::error("bstd::json(string, bool)",
          "Couldn't open json file at path: " + _string + ". Does it exist?");

    std::stringstream stream;
    stream << ifs.rdbuf();
    json_string = stream.str();
  }

  if(m_debug)
    std::cout << "Parsing json string: " << std::endl << json_string << std::endl;

  // Limit the size of JSON strings.
  if(json_string.size() > MAX_STRING_SIZE) {
    std::stringstream ss;
    ss << MAX_STRING_SIZE;
    throw error::error("btsd::json",
        "The JSON object is too large. The current maximum string size is "
        + ss.str());
  }

  parse(_string);
}


const size_t
json::
size() const {
  return m_children.size();
}


const std::string&
json::
get_path() const {
  return m_path;
}


const std::string
json::
to_string() const {
  return "json";
}


const bool
json::
operator==(const json& _rhs) const {
  return this->to_string() == _rhs.to_string();
}


// TODO: add max size check.
const std::string
json::
operator+(const char* _rhs) const {
  return *this + std::string(_rhs);
}


// TODO: add max size check.
const std::string
json::
operator+(const std::string& _rhs) const {
  return this->to_string() + _rhs;
}


// TODO: add max size check.
const json
json::
operator+(const json& _rhs) const {
  json result(*this);
  result.add_child(_rhs);
  return result;
}


void
json::
parse(const std::string& _string) {
  if(m_debug)
    std::cout << "bstd::json::parse" << std::endl;

  const auto p = new parser(m_debug);
  p->parse(_string, this);
  delete p;
}


// TODO: add max size check.
void
json::
add_child(const json& _child) {
  m_children.push_back(_child);
}


void
json::
write() const {
  write(m_path);
}


void
json::
write(const std::string& _path) const {
  if(m_debug)
    std::cout << "bstd::json::write" << std::endl;

  // No need to crash here, we can just do nothing. Log a warning instead.
  if(_path.empty()) {
    std::cerr << "Attempting to write json object to empty path." << std::endl;
    return;
  }

  // TODO: check file size before or after writing.
  std::ofstream ofs(_path, std::ofstream::trunc);

  if(!ofs.is_open())
    throw bstd::error::error("Couldn't open json file for writing: " + _path +
        ". Does it exist?", "json::write");

  ofs << to_string();
}


}
