#include <map>
#include <string>
#include <ctime>



//A class that holds all messages and relevant stats about a user
class userClass{
    public:
        //stores messages, mapped from the time associated with the message
        map<time_t, Message> messages;
        
        int totalMessages;
        time_t timeSpentInCall;
        //include some other stats - like a graph of messages over time and stuff like that
};