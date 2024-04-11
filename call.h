#pragma once

#include <string>
#include <vector>

class Call {
    public:
        // variables from json
        std::string id;                             // message #
        std::string timestamp;                      // start time
        std::string ended_timestamp;                // end time
        std::string author;                         // caller's username
        std::string channel_id;                     // channel #

        // post processing variables
        double duration;                         // not sure how to store this yet

        // constructor
        Call(const std::string& _id, const std::string& _timestamp, const std::string& _ended_timestamp, const std::string& _author, const std::string& _channel_id)
            : id(_id), timestamp(_timestamp), ended_timestamp(_ended_timestamp), author(_author), channel_id(_channel_id) {}

        int getLength();
};