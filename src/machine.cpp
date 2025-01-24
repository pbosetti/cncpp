/*
  __  __            _     _                   _               
 |  \/  | __ _  ___| |__ (_)_ __   ___    ___| | __ _ ___ ___ 
 | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \  / __| |/ _` / __/ __|
 | |  | | (_| | (__| | | | | | | |  __/ | (__| | (_| \__ \__ \
 |_|  |_|\__,_|\___|_| |_|_|_| |_|\___|  \___|_|\__,_|___/___/
                                                              
Machine class implementation
Author: Paolo Bosetti, 2024
*/
#include "machine.hpp"
#include <sstream>
#include <iostream>
#include <rang.hpp>
#include <yaml-cpp/yaml.h>


using namespace rang;

namespace cncpp {

Machine::Machine(const std::string &settings_file) : _settings_file(settings_file) {
  load(_settings_file);
  mosqpp::lib_init();
  if (_debug) {
    int major, minor, revision;
    mosqpp::lib_version(&major, &minor, &revision);
    cerr << style::italic << fg::cyan << "Using Mosquitto library version " 
         << major << "." << minor << "." << revision 
         << style::reset << fg::reset << endl;
  }

}

Machine::~Machine() {
  if (disconnect() != MOSQ_ERR_SUCCESS) {
    cerr << fg::red << "Cannot disconnect from MQTT broker" << fg::reset 
         << endl;
  }
  mosqpp::lib_cleanup();
}

int Machine::connect() {
  int rc = mosquittopp::connect(_mqtt_host.c_str(), _mqtt_port, _mqtt_keepalive);
  if (rc != MOSQ_ERR_SUCCESS) {
    throw CNCError("Cannot connect to MQTT broker", this);
  }
  return rc;
}

void Machine::load(const string &settings_file) {
  _settings_file = settings_file;
  auto data = YAML::LoadFile(settings_file);
  _A = data["machine"]["A"].as<double>();
  _tq = data["machine"]["tq"].as<double>();
  _max_error = data["machine"]["max_error"].as<double>();
  _fmax = data["machine"]["fmax"].as<double>();
  _zero = Point(
    data["machine"]["zero"][0].as<double>(), 
    data["machine"]["zero"][1].as<double>(), 
    data["machine"]["zero"][2].as<double>()
  );
  _offset = Point(
    data["machine"]["offset"][0].as<double>(), 
    data["machine"]["offset"][1].as<double>(), 
    data["machine"]["offset"][2].as<double>()
  );
  _mqtt_host = data["mqtt"]["host"].as<string>("localhost");
  _mqtt_port = data["mqtt"]["port"].as<int>(1883);
  _mqtt_keepalive = data["mqtt"]["keepalive"].as<int>(60);
  _pub_topic = data["mqtt"]["topics"]["pub"].as<string>("cncpp/setpoint");
  _sub_topic = data["mqtt"]["topics"]["sub"].as<string>("cncpp/status/#");

}


string Machine::desc(bool colored) const {
  ostringstream ss;
  ss << "A=" << _A << ", ";
  ss << "tq=" << _tq << ", ";
  ss << "max_error=" << _max_error << ", ";
  ss << "fmax=" << _fmax << ", ";
  ss << endl << "zero=" << _zero.desc(false) << ", ";
  ss << endl << "offset=" << _offset.desc(false) << ", ";
  ss << endl << "mqtt_host=" << _mqtt_host << ", ";
  ss << "mqtt_port=" << _mqtt_port;
  return ss.str();
};

data_t Machine::quantize(data_t t, data_t &dq) const {
  data_t q;
  q = ((size_t)(t / _tq) + 1) * _tq;
  dq = q - t;
  return q;
}

// MQTT related methods

void Machine::on_connect(int rc) {
  if (rc == 0 && _debug) {
    cerr << fg::yellow << "Connected to MQTT broker " 
         << _mqtt_host << ":" << _mqtt_port << fg::reset << endl;
  }
}

void Machine::on_subscribe(int mid, int qos_count, const int *granted_qos) {
  if (_debug)
    cerr << fg::yellow << "Subscribed to topic " << _sub_topic << fg::reset  
         << endl;
}

void Machine::on_message(const struct mosquitto_message *message) {
  if (_debug) {
    cerr << fg::yellow << "Received message on topic " 
         << style::bold << message->topic << style::reset
         << fg::reset << endl
         << fg::yellow << "Payload: " 
         << style::bold << (char *)message->payload  << style::reset
         << fg::reset << endl;
  }
  string payload((char *)message->payload, message->payloadlen);
  json j;
  try {
    j = json::parse(payload);
  } catch (json::parse_error &e) {
    cerr << fg::red << "Cannot parse JSON payload: " << e.what() << endl
         << "Payload: " << style::bold << payload 
         << style::reset << fg::reset << endl;
    return;
  }
  _position = Point(j["x"], j["y"], j["z"]);
  if (_debug)
    cerr << fg::yellow <<  "Received: " << j.dump() << fg::reset << endl;
}


void Machine::listen_start() {
  if (subscribe(NULL, _sub_topic.c_str()) != MOSQ_ERR_SUCCESS) {
    throw CNCError("Cannot subscribe to topic " + _sub_topic, this);
  }  
}

void Machine::listen_stop() {
  if (unsubscribe(NULL, _sub_topic.c_str()) != MOSQ_ERR_SUCCESS) {
    throw CNCError("Cannot unsubscribe from topic " + _sub_topic, this);
  }
}

void Machine::sync(bool rapid) {
  Point pos = (_setpoint + _offset);
  json j;
  j["x"] = pos.x();
  j["y"] = pos.y();
  j["z"] = pos.z();
  j["rapid"] = rapid;
  string payload = j.dump();
  int rc = publish(NULL, _pub_topic.c_str(), payload.length(), payload.c_str(), 0, false);
  if (rc != MOSQ_ERR_SUCCESS) {
    throw CNCError("Cannot publish to topic " + _pub_topic, this);
  }

  loop();
}

}



/*
  _____         _   
 |_   _|__  ___| |_ 
   | |/ _ \/ __| __|
   | |  __/\__ \ |_ 
   |_|\___||___/\__|
*/

#ifdef MACHINE_MAIN
#include <csignal>
#include <thread>

static bool Running = true;

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <settings_file.yml>" << endl;
    return 1;
  }

  std::signal(SIGINT, [](int sig) { Running = false; });

  cncpp::Machine machine(argv[1]);
  machine.connect();
  cout << machine.desc() << endl;
  machine.listen_start();

  machine.setpoint(1, 2, 3);
  machine.position(0, 0, 0);

  while (Running) {
    machine.sync();
    machine.setpoint(machine.position());
    this_thread::sleep_for(chrono::milliseconds(100));
  }
  cout << "Exiting..." << endl;
  machine.listen_stop();

  return 0;
}



#endif