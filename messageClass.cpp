#include <ctime>
#include <string>

//NOTE: EVERYTHING IN HERE IS SUBJECT TO DRAMATIC CHANGE - nothing is confirmed so methods/variables could be added/deleted whenever

//Stores info about one message, can set/get all the variables in the message class
class Message {
    public:
        time_t timeStamp;
        std::string content;

        int pingCount;
        int wordCount;
        bool isReply;
        bool isCallEnd;
        bool containsLink;

        //Default constructor sets everything to either 0 or false
        Message(){
            pingCount = 0;
            wordCount = 0;
            isReply = false;
            isCallEnd = false;
            containsLink = false;
        }

        //Message Constructor with parameters
        Message(int pCount, int wCount, bool reply, bool callEnd, bool link){
            pingCount = pCount;
            wordCount = wCount;
            isReply = reply;
            isCallEnd = callEnd;
            containsLink = link;
        }

        //a bunch of setter methods
        void setContent(std::string newContent){
            content = newContent;
        }

        void setTimeStamp(time_t newTimeStamp){
            timeStamp = newTimeStamp;
        }

        void setPingCount(int newPingCount){
            pingCount = newPingCount;
        }

        void setWordCount(int newWordCount){
            wordCount = newWordCount;
        }

        void setIsReply(bool newIsReply){
            isReply = newIsReply;
        }

        void setIsCallEnd(bool newIsCallEnd){
            isCallEnd = newIsCallEnd;
        }

        void setContainsLink(bool newContainsLink){
            containsLink = newContainsLink;
        }


        //a bunch of getter methods
        std::string getContent(){
            return content;
        }

        time_t getTimeStamp(){
            return timeStamp;
        }

        int getPingCount(){
            return pingCount;
        }

        int getWordCount(){
            return wordCount;
        }

        bool getIsReply(){
            return isReply;
        }

        bool getIsCallEnd(){
            return isCallEnd;
        }

        bool getContainsLink(){
            return containsLink;
        }
};
