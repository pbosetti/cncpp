/*
  __  __            _     _                   _               
 |  \/  | __ _  ___| |__ (_)_ __   ___    ___| | __ _ ___ ___ 
 | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \  / __| |/ _` / __/ __|
 | |  | | (_| | (__| | | | | | | |  __/ | (__| | (_| \__ \__ \
 |_|  |_|\__,_|\___|_| |_|_|_| |_|\___|  \___|_|\__,_|___/___/
                                                              
Class implementation
*/

#include "machine.hpp"

namespace cncpp {

data_t Machine::quantize(data_t t, data_t &dq) const {
  data_t q;
  q = static_cast<size_t>((t / _tq) + 1) * _tq;
  dq = q - t;
  return q;
}

}