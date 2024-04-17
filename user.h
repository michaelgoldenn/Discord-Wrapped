#pragma once

#include <vector>
#include <string>
#include <map>

#include "message.h"
#include "call.h"

// a class that holds all messages and relevant stats about another user
class User{
    public:
        // variables from json
        std::string channel;            // channel # 
        std::string id;                 // user #       
        std::string last_message_id;    // last message #
        std::string username;           // global user id
        std::string global_name;        // display name
        std::string discriminator;      // i.e. #2415                almost always 0 now

        // iterators
        int totalMessages = 0;
        int messagesSent = 0;
        int messagesReceived = 0;
        int charactersSent = 0;
        int charactersReceived = 0;

        int totalCalls = 0;
        int callsSent = 0;
        int callsReceived = 0;
        double callTime = 0;

        // post processing statistics

        // constructor
        User(const std::string& _channel, const std::string& _id, const std::string& _last_message_id, const std::string& _username, const std::string& _global_name, const std::string& _discriminator)
            : channel(_channel), id(_id), last_message_id(_last_message_id), username(_username), global_name(_global_name), discriminator(_discriminator) {}

        mutable std::multimap<std::string, Message> messages;   // multimap of messages ordered by timestamp
        mutable std::multimap<std::string, Call> calls;         // multimap of calls ordered by timestamp
};