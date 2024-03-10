#include <ctime>
#include <string>

//NOTE: EVERYTHING IN HERE IS SUBJECT TO DRAMATIC CHANGE - nothing is confirmed so methods/variables could be added/deleted whenever

//Stores info about one message
class Message {
    public:
        time_t timeStamp;
        std::string content;

        int getPingCount(std::string);
        int getWordCount(std::string);
        bool getIsReply(std::string);
        bool getIsCallEnd(std::string);
        bool getContainsLink(std::string);

        int pingCount;
        int wordCount;
        bool isReply;
        bool isCallEnd;
        bool containsLink;
};

int Message::getPingCount(std::string content){
    //TODO
}

int Message::getWordCount(std::string content){
    //TODO
}

bool Message::getIsReply(std::string content){
    //TODO
}

bool Message::getIsCallEnd(std::string content){
    //TODO
}

bool Message::getContainsLink(std::string content){
    //TODO
}