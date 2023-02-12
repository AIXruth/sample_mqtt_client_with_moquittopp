#include "mqtt.h"

std::string input_reader() {
        std::string s;
        if (std::cin.good() && std::cin >> s) return s;
        return std::string();
}


int main(int argc, char *argv[])
{
    using namespace std::literals;
    class mqtt_client *iot_client;
    //int rc;
    std::string input;

    char client_id[] = CLIENT_ID;
    char topic[] = MQTT_TOPIC;
    char host[] = BROKER_ADDRESS;
    int port = MQTT_PORT;

    iot_client = new mqtt_client(client_id, topic, host, port);

    iot_client->send_message("AN");
    // iot_client->subscribe(NULL, MQTT_TOPIC);
    while(1) {
        auto f = std::async(std::launch::async, input_reader);
        while (f.wait_for(2s) == std::future_status::timeout) {
            std::cout << "." << std::flush;
        }
        input = f.get();
        // std::cout << "Input was: " << input << std::endl;
        if (input == "stop") break;
        // else iot_client->send_message(input.c_str());
        else {
            int len = input.length();
            iot_client->publish(NULL, "PUBTOP", len, input.c_str());
        }
    }

    return 0;
}

