/*
  __  __            _     _                   _               
 |  \/  | __ _  ___| |__ (_)_ __   ___    ___| | __ _ ___ ___ 
 | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \  / __| |/ _` / __/ __|
 | |  | | (_| | (__| | | | | | | |  __/ | (__| | (_| \__ \__ \
 |_|  |_|\__,_|\___|_| |_|_|_| |_|\___|  \___|_|\__,_|___/___/
                                                              
*/

#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "defines.hpp"
#include "point.hpp"
#include <mosquittopp.h>
#include <nlohmann/json.hpp>

using namespace std;
using namespace mosqpp;
using json = nlohmann::json;

namespace cncpp {

class Machine final : Object, mosquittopp {
public:
  // Lifecycle -----------------------------------------------------------------
  Machine(const string &settings_file);
  Machine() {}
  ~Machine();

  // Methods -------------------------------------------------------------------
  void load(const string &settings_file);
  string desc(bool colored = true) const override;
  data_t quantize(data_t t, data_t &dq) const;

  // Accessors -----------------------------------------------------------------
  data_t A() const { return _A; }
  data_t tq() const { return _tq; }
  data_t fmax() const { return _fmax; }
  data_t error() const { return _error; }
  data_t max_error() const { return _max_error; }

  Point zero() const { return _zero; }
  Point offset() const { return _offset; }

  Point position() const { return _position; }
  Point setpoint() const { return _setpoint; }
  Point setpoint(Point p) { _setpoint = p; return _setpoint; }
  Point setpoint(data_t x, data_t y, data_t z) {
    _setpoint.x(x);
    _setpoint.y(y);
    _setpoint.z(z);
    return _setpoint;
  }

  // MQTT-related methods
  int connect();
  bool connected() { return _connected; }
  void listen_start();
  void listen_stop();
  void on_connect(int rc) override;
  void on_disconnect(int rc) override;
  void on_subscribe(int mid, int qos_count, const int *qos) override;
  void on_unsubscribe(int mid) override;
  void on_message(const struct mosquitto_message *message) override;
  void sync(bool rapid);

  // returns something like "mqtt://localhost:1883"
  string mqtt_host() const { return "mqtt://" + _mqtt_host + ":" + to_string(_mqtt_port); }

private:
  // parameters
  string _settings_file = "";
  Point _zero = Point(0, 0, 0);
  Point _offset = Point(0, 0, 0);
  Point _setpoint, _position;
  data_t _A = 5.0; // m/s/s
  data_t _tq = 0.005; // sampling time (s)
  data_t _fmax = 10000;
  data_t _max_error = 0.005;

  // State variables
  data_t _error = 0.0;

  // MQTT-related params
  string _mqtt_host = "localhost";
  int _mqtt_port = 1883;
  int _mqtt_keepalive = 60;
  string _pub_topic; // publish set-points
  string _sub_topic; // get current postions
  char _msg_buffer[MQTT_BUFLEN];
  bool _connected = false;

};


}





#endif // MACHINE_HPP