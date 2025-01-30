#include <iostream>
#include <mosquittopp.h>

using namespace mosqpp;
using namespace std;

class MqttClient : public mosquittopp {
public:
  MqttClient(const char* id, const char* host, int port, const char* topic)
    : mosquittopp(id), _topic(topic) {
    connect(host, port, 60);
    loop_start();
  }

  void on_connect(int rc) override {
    if(rc == 0) {
      cout << "Connected - code " << rc << endl;
      subscribe(nullptr, _topic);
    }
  }

  void on_message(const mosquitto_message* message) override {
    if(message && message->payload) {
      cout << static_cast<char*>(message->payload) << endl;
    }
  }

private:
  const char* _topic;
};

int main(int argc, char const *argv[]) {
  string topic = "#";
  if(argc > 1) {
    topic = argv[1];
  }
  mosqpp::lib_init();
  MqttClient client("mqtt_cli", "localhost", 1883, topic.c_str());
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();
  mosqpp::lib_cleanup();
  return 0;
}