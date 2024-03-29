#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <regex>

#ifndef MESSAGE_H
#define MESSAGE_H

class Message {
    public:
        int id;
        std::string content;
        time_t timeStamp;

        int mentionCount;
        int wordCount;
        bool isReply;
        time_t callTime;
        bool containsLink;

        // Default constructor
        Message() : id(0), timeStamp(0), content(""), mentionCount(0), wordCount(0), isReply(false), callTime(0), containsLink(false){};

        Message(int ID, time_t time, std::string message);

        void printMessage() const;

    private:
        int getMentionCount();
        int getWordCount();
        bool getIsReply();
        time_t getCallTime();
        bool getContainsLink();
};

#endif
