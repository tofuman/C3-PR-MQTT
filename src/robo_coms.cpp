#include "robo_coms.h"
#include "config.h"

static const std::string TOPIC_RECV_ACTION = std::string("action/");
static const std::string TOPIC_RECV_SETTING = std::string("setting/");
static const std::string TOPIC_RECV_ALL = std::string("all");

static const std::string TOPIC_SEND_STATUS = std::string("status/");
static const std::string TOPIC_SEND_REGISTER = std::string("register");

static const std::string PAYLOAD_ACTION_FORWARD = std::string("forward");
static const std::string PAYLOAD_ACTION_LEFT = std::string("left");
static const std::string PAYLOAD_ACTION_RIGHT = std::string("right");
static const std::string PAYLOAD_ACTION_BACK = std::string("back");
static const std::string PAYLOAD_ACTION_FLASH = std::string("back");


bool RoboComs::setup(Config* config)
{
    bool res= false;
    char topic[30];

    this->config = config;

    res = mqtt.setup(config);
    if (res) {
        strcpy(topic, TOPIC_RECV_ACTION.c_str());
        strcpy(topic+ strlen(TOPIC_RECV_ACTION.c_str()), config->get_name());
        res = mqtt.subscribe(topic);
    }
    if (res) {
        strcpy(topic, TOPIC_RECV_SETTING.c_str());
        strcpy(topic+ strlen(TOPIC_RECV_SETTING.c_str()), config->get_name());
        res = mqtt.subscribe(topic);
    }
    if (res) {
        strcpy(topic, TOPIC_RECV_ALL.c_str());
        res = mqtt.subscribe(topic);
    }
    if (res) {
        strcpy(topic, TOPIC_SEND_REGISTER.c_str());
        res = mqtt.publish(topic, config->get_name());
    }

    return res;

}

void RoboComs::tick()
{
    char topic[30];
    mqtt.tick();
    strcpy(topic, TOPIC_SEND_STATUS.c_str());
    strcpy(topic+ strlen(TOPIC_SEND_STATUS.c_str()), config->get_name());
    mqtt.publish(topic, "pingubert");
}

Message* RoboComs::fetch_message() {
    Message* ret = mqtt.get_message();;
    if (ret != NULL) {
        //returns false if topic is not a valid topic
        if(!parse_type(&ret)){
            delete ret;
            ret = NULL;
        }
    }
    return ret;

}


bool RoboComs::parse_type(Message** message)
{
    Message* message_ptr = *message;
    std::string topic = message_ptr->get_topic();
    if(topic.rfind(TOPIC_RECV_ACTION, 0) == 0) {
        message_ptr->set_type(MESSAGE_ACTION);
        parse_action(message);
    } else if (topic.rfind(TOPIC_RECV_SETTING, 0) == 0) {
        message_ptr->set_type(MESSAGE_SETTING);
    } else if (topic.rfind(TOPIC_RECV_ALL, 0) == 0) {
        message_ptr->set_type(MESSAGE_ALL);
    } else {
        Serial.println("Unknown Message Topic!");
        return false;
    }

    return true;

}


bool RoboComs::parse_action(Message** message)
{
    Message* message_ptr = *message;
    std::string topic = message_ptr->get_topic();
    if(topic.rfind(PAYLOAD_ACTION_FORWARD, 0) == 0) {
        message_ptr->set_action(ACTION_FORWARD);
    } else if (topic.rfind(PAYLOAD_ACTION_LEFT, 0) == 0) {
        message_ptr->set_action(ACTION_LEFT);
    } else if (topic.rfind(PAYLOAD_ACTION_RIGHT, 0) == 0) {
        message_ptr->set_action(ACTION_RIGHT);    
    } else if (topic.rfind(PAYLOAD_ACTION_BACK, 0) == 0) {
        message_ptr->set_action(ACTION_BACK);
    } else if (topic.rfind(PAYLOAD_ACTION_FLASH, 0) == 0) {
        message_ptr->set_action(ACTION_FLASH);
    } else {
        message_ptr->set_action(ACTION_NONE);
        return false;
    }
    return true;

}