#include "robo_coms.h"
#include "config.h"


bool RoboComs::setup(Config* config)
{
    bool res= false;
    char topic[30];

    this->config = config;

    res = mqtt.setup(config);
    if (res) {
        strcpy(topic, TOPIC_RECV_ACTION);
        strcpy(topic+ strlen(TOPIC_RECV_ACTION), config->get_name());
        res = mqtt.subscribe(topic);
    }
    if (res) {
        strcpy(topic, TOPIC_RECV_SETTING);
        strcpy(topic+ strlen(TOPIC_RECV_SETTING), config->get_name());
        res = mqtt.subscribe(topic);
    }
    if (res) {
        strcpy(topic, TOPIC_RECV_ALL);
        res = mqtt.subscribe(topic);
    }
    if (res) {
        strcpy(topic, TOPIC_SEND_REGISTER);
        res = mqtt.publish(topic, config->get_name());
    }

    return res;

}

void RoboComs::tick()
{
    bool res= false;
    char topic[30];
    mqtt.tick();
    strcpy(topic, TOPIC_SEND_STATUS);
    strcpy(topic+ strlen(TOPIC_SEND_STATUS), config->get_name());
    res = mqtt.publish(topic, "pingubert");
}

Message* RoboComs::fetch_message() {
    Message* ret = NULL;
    int pending = mqtt.get_messages_pending();
    if (pending != 0) 
    {
        ret = mqtt.get_message();

    }
    return ret;

}


void RoboComs::parse_type(Message** message)
{
    Message* message_ptr = *message;
    std::string topic = message_ptr->get_topic();

}