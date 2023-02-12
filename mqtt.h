
#define MQTT_TOPIC "EXAMPLE_TOPIC"
#define PUBLISH_TOPIC "EXAMPLE_TOPIC"
#define CLIENT_ID "Client_ID123nwtest"
#define BROKER_ADDRESS "florid"
#define MQTT_PORT 1884
#define USER "mqttpi"
#define PASSWORD "pWd123"

#include <iostream>
#include <mosquittopp.h>
#include <cstring>
#include <cstdio>
#include <thread>
#include <future>

#define MAX_PAYLOAD 50
#define DEFAULT_KEEP_ALIVE 60

class mqtt_client : public mosqpp::mosquittopp {
private:
    const char * host;
    const char * id;
    const char * topic;
    int          port;
    int          keepalive;

    void on_disconnect(int rc);
    void on_connect(int rc);
    void on_message(const struct mosquitto_message *message);
    void on_subscribe(int mid, int qos_count, const int *granted_qos);
    void on_publish(int mid);
public:
    mqtt_client (const char *_id, const char *_topic, const char *_host, int _port);
    ~mqtt_client();
    bool send_message(const char * _message);

};


