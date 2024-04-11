#pragma once

#include <string>

class Message {
    public:
        // variables from json
        std::string id;             // message #
        std::string content;        // string of message contents
        std::string author;         // sender's username
        std::string channel_id;     // channel #
        std::string timestamp;      // time the message was sent
        bool mentioned;             // whether the other person was pinged

        // post processing variables
        std::string wordCount;      // number of words in the message

        // constructor
        Message(const std::string& _id, const std::string& _content, const std::string& _author, const std::string& _channel_id, const std::string& _timestamp, bool _mentioned)
            : id(_id), content(_content), author(_author), channel_id(_channel_id), timestamp(_timestamp), mentioned(_mentioned) {}

        int getWordCount();
};