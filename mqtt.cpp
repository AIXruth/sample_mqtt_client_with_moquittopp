#include "mqtt.h"

#ifdef DEBUG
#include <iostream>
#endif

mqtt_client::mqtt_client(const char *_id, const char *_topic, const char *_host, int _port) : mosquittopp(_id) {
    this->keepalive = DEFAULT_KEEP_ALIVE;
    mosqpp::lib_init();
    username_pw_set(USER, PASSWORD);
    this->keepalive = 60;    // Basic configuration setup for myMosq class
    this->id = _id;
    this->port = _port;
    this->host = _host;
    this->topic = _topic;
    connect(host, port, keepalive);
    loop_start();
    subscribe(NULL, MQTT_TOPIC);
}

mqtt_client::~mqtt_client() {
  loop_stop();
  mosqpp::lib_cleanup();
}

void mqtt_client::on_connect(int rc) {
    if ( rc == 0 ) {
        std::cout << ">> myMosq - connected with server" << std::endl;
    } else {
        std::cout << ">> myMosq - Impossible to connect with server(" << rc << ")" << std::endl;
    }
}

void mqtt_client::on_disconnect(int rc) {
    std::cout << ">> myMosq - disconnection(" << rc << ")" << std::endl;
    do {
      connect(host, port, keepalive);
    } while (! MOSQ_ERR_SUCCESS);
}

void mqtt_client::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
    #ifdef DEBUG
        std::cout << "Subscription succeeded." << std::endl;
    #endif
}

void mqtt_client::on_message(const struct mosquitto_message *message)
{
    int payload_size = MAX_PAYLOAD + 1;
    char buf[payload_size];
    
    if(!strcmp(message->topic, PUBLISH_TOPIC))
    {
        memset(buf, 0, payload_size * sizeof(char));

        /* Copy N-1 bytes to ensure always 0 terminated. */
        memcpy(buf, message->payload, MAX_PAYLOAD * sizeof(char));

        #ifdef DEBUG
            std::cout << buf << std::endl;
        #endif

        // Examples of messages for M2M communications...
        if (!strcmp(buf, "STATUS"))
        {
            snprintf(buf, payload_size, "This is a Status Message...");
            publish(NULL, PUBLISH_TOPIC, strlen(buf), buf);
            #ifdef DEBUG
                std::cout << "Status Request Recieved." << std::endl;
            #endif
        }

        if (!strcmp(buf, "ON"))
        {
            snprintf(buf, payload_size, "Turning on...");
            publish(NULL, PUBLISH_TOPIC, strlen(buf), buf);
            #ifdef DEBUG
                std::cout << "Request to turn on." << std::endl;
            #endif
        }

        if (!strcmp(buf, "OFF"))
        {
            snprintf(buf, payload_size, "Turning off...");
            publish(NULL, PUBLISH_TOPIC, strlen(buf), buf);
            #ifdef DEBUG
                std::cout << "Request to turn off." << std:: endl;
            #endif
        }
    }
}

void mqtt_client::on_publish(int mid) {
    std::cout << ">> myMosq - Message (" << mid << ") succeed to be published " << std::endl;
}

bool mqtt_client::send_message(const  char * _message) {
 // Send message - depending on QoS, mosquitto lib managed re-submission this the thread
 //
 // * NULL : Message Id (int *) this allow to latter get status of each message
 // * topic : topic to be used
 // * lenght of the message
 // * message
 // * qos (0,1,2)
 // * retain (boolean) - indicates if message is retained on broker or not
 // Should return MOSQ_ERR_SUCCESS
     int ret = publish(NULL,this->topic,strlen(_message),_message,1,false);
    return ( ret == MOSQ_ERR_SUCCESS );
}