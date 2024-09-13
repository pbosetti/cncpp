/*
   ____ _   _  ____ ____  ____          _       _           _
  / ___| \ | |/ ___|  _ \|  _ \    __ _| | ___ | |__   __ _| |
 | |   |  \| | |   | |_) | |_) |  / _` | |/ _ \| '_ \ / _` | |
 | |___| |\  | |___|  __/|  __/  | (_| | | (_) | |_) | (_| | |
  \____|_| \_|\____|_|   |_|      \__, |_|\___/|_.__/ \__,_|_|
                                  |___/
Global include for CNCpp project
*/

#ifndef CNCPP_HPP
#define CNCPP_HPP

#include <optional>
#include <string>

/*
  ____        __ _
 |  _ \  ___ / _(_)_ __   ___  ___
 | | | |/ _ \ |_| | '_ \ / _ \/ __|
 | |_| |  __/  _| | | | |  __/\__ \
 |____/ \___|_| |_|_| |_|\___||___/

*/

#define VERSION "0.0.1"
#define BUILD_TYPE "Release"

#define NUMBERS_WIDTH "8" // Must be a string

typedef double data_t;
typedef std::optional<data_t> opt_data_t;

/*
  _   _
 | \ | | __ _ _ __ ___   ___  ___ _ __   __ _  ___ ___
 |  \| |/ _` | '_ ` _ \ / _ \/ __| '_ \ / _` |/ __/ _ \
 | |\  | (_| | | | | | |  __/\__ \ |_) | (_| | (_|  __/
 |_| \_|\__,_|_| |_| |_|\___||___/ .__/ \__,_|\___\___|
                                 |_|
*/

namespace cncpp {

class Object {
public:
  virtual std::string desc(bool colored = true) const = 0;
};

class CNCError : public std::exception {
public:
  CNCError(const char *msg, const Object *o) : _msg(msg), _obj(o) {}
  const char *what() const noexcept override { return _msg; }
  std::string who() noexcept { return _obj->desc(false); }

private:
  const char *_msg;
  const Object *_obj;
};

} // namespace cncpp

#endif // CNCPP_HPP
