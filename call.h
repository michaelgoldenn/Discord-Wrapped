#pragma once

#include <string>
#include <vector>

class Call {
    public:
        // variables from json
        std::string id;                             // message #
        std::string timestamp;                      // start time
        std::string ended_timestamp;                // end time
        std::string user_id;                        // caller's user #
        std::string channel_id;                     // channel #

        // post processing variables
        std::string length;                         // not sure how to store this yet

        int getLength();
};