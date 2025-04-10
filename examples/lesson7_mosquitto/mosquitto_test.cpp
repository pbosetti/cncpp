// cmake -Bbuild

#include<iostream>
#include<mosquittopp.h>

using namespace std;
using namespace mosqpp; // mosquitto namspace

// MQTT Clint
class MqttClient : public mosquittopp {
public:

  // Class constructor
  MqttClient(const char * id, const char * host, int port, const char * topic) 
    : mosquittopp(id), _topic(topic)
  {
    connect(host, port, 60); // function contained in mosqpp
    loop_start();
  }

  // Method "on_connect"
  // After the connection is enstablished
  void on_connect(int rc) override {
    if (rc == 0) {
      cout << "Connected - code " << rc << endl;
      subscribe(nullptr, _topic);
    }
  }

  // Method "on_message"
  // When we receive a message
  void on_message(const mosquitto_message* message) override {
    if (message && message->payload) {
      cout << static_cast<char *>(message->payload) << endl;
    }
  }

private:
  const char * _topic;
};

int main () 
{
  string topic = "#";
  mosqpp::lib_init();
  MqttClient("mqtt_cli", "localhost", 1883, topic.c_str());
  cout << "Press enter to quit ..." << endl;
  cin.get();
  mosqpp::lib_cleanup();
  return 0;

  // Subscribe to the all topics (#) with 
  // build/examples/lesson7_mosquitto
  // publish messages on topics with
  // build/_deps/mosquitto-build/client/mosquitto_pub -t "test" -m "message"
}