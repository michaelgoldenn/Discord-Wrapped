#ifndef USER_H
#define USER_H

#include <map>
#include <string>
#include <ctime>

#include "message.h"

//A class that holds all messages and relevant stats about another user
class userClass{
    public:
        int userID;
        std::map<int, Message> messages; //stores messages, mapped from the message ID with the message
        
        int totalMessages;
        long long int totalWords;
        time_t timeSpentInCall;
        std::map<std::string, int> wordFrequency; //stores words and how often they've been seen

        // Default constructor
        userClass() : userID(0), totalMessages(0), totalWords(0), timeSpentInCall(0) {}

        // Constructor with userID parameter
        userClass(int id) : userID(id), totalMessages(0), totalWords(0), timeSpentInCall(0) {}

        time_t addMessageToUser(Message newMessage);
        void updateWordFrequencies(const std::string& inputString);
        void printMessages();
        void printStats();

        //include some other stats - like a graph of messages over time and stuff like that
};

#endif