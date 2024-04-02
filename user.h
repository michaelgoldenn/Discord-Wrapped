#pragma once

#include <vector>
#include <string>
#include <map>

#include "message.h"
#include "call.h"

// A class that holds all messages and relevant stats about another user
class User{
    public:
        // variables from json
        std::string id;             // user #
        std::string channel;        // channel #        
        std::string username;       // global user id
        std::string global_name;    // display name
        std::string discriminator;  // i.e. #2415                       almost always 0 now

        // post processing variables
        int messagesSent;
        int messagesReceived;

        // constructor
        User(const std::string& _id, const std::string& _channel, const std::string& _username, const std::string& _global_name, const std::string& _discriminator)
        : id(_id), channel(_channel), username(_username), global_name(_global_name), discriminator(_discriminator) {}

        void addMessage(const Message& message);
        void printMessages();

        void addCall(const Call& call);
        void printCalls();

        void printStats();
    
    private:
        std::multimap<std::string, Message> messages;   // multimap of messages ordered by timestamp
        std::multimap<std::string, Call> calls;         // multimap of calls ordered by timestamp
};