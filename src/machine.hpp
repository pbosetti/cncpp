/*
  __  __            _     _                   _               
 |  \/  | __ _  ___| |__ (_)_ __   ___    ___| | __ _ ___ ___ 
 | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \  / __| |/ _` / __/ __|
 | |  | | (_| | (__| | | | | | | |  __/ | (__| | (_| \__ \__ \
 |_|  |_|\__,_|\___|_| |_|_|_| |_|\___|  \___|_|\__,_|___/___/
                                                              
Machine class header file
Author: Paolo Bosetti, 2024
*/
#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "defines.hpp"
#include "point.hpp"
#include <string>
#include <mosquittopp.h>
#include <nlohmann/json.hpp>

#define BUFLEN 1024

using namespace std;
using namespace mosqpp;
using json = nlohmann::json;

namespace cncpp {

class Machine : Object, mosquittopp {
public:
  // Lifecycle
  Machine(const std::string &settings_file);
  Machine() {}
  ~Machine();

  // Methods
  void load(const string &settings_file);
  string desc(bool colored = true) const override;
  data_t quantize(data_t t, data_t &dq) const;

  // MQTT related methods
  int connect();
  void sync(bool rapid = false);
  void listen_start();
  void listen_stop();
  void on_connect(int rc) override;
	void on_message(const struct mosquitto_message *message) override;
	void on_subscribe(int mid, int qos_count, const int *granted_qos) override;

  // Accessors
  Point position() const { return _position; }
  Point position(Point p) { _position = p; return _position; }
  Point position(data_t x, data_t y, data_t z) { 
    _position.x(x);
    _position.y(y);
    _position.z(z);
    return _position;
  }
  Point setpoint() const { return _setpoint; }
  Point setpoint(Point p) { _setpoint = p; return _setpoint; }
  Point setpoint(data_t x, data_t y, data_t z) { 
    _setpoint.x(x);
    _setpoint.y(y);
    _setpoint.z(z);
    return _setpoint;
  }
  Point offset() const { return _offset; }
  Point zero() const { return _zero; }
  data_t max_error() const { return _max_error; }
  data_t error() const { return _error; }
  data_t fmax() const { return _fmax; }
  data_t tq() const { return _tq; }
  data_t A() const { return _A; }

  string mqtt_host() const { return "mqtt://" + _mqtt_host + ":" + to_string(_mqtt_port); }

private:
  std::string _settings_file;
  data_t _A;                     // Maximum acceleration (m/s/s)
  data_t _tq;                    // Sampling time (s)
  data_t _max_error, _error;      // Maximum and actual positioning error (mm)
  data_t _fmax;                  // Maximum feedrate (mm/min)
  Point _zero;                   // Initial machine position
  Point _setpoint, _position;     // Setpoint and actual position
  Point _offset;                 // Workpiece origin coordinates
  string _mqtt_host = "localhost";
  int _mqtt_port = 1883;
  int _mqtt_keepalive = 60;
  string _pub_topic;
  string _sub_topic;
  char _msg_buffer[BUFLEN];
};


}





#endif // MACHINE_HPP