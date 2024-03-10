#include <map>
#include <ctime>

//not proper c++, I just scribbled this down

//A class that holds all messages and relevant stats about a user
class userClass{
    public:
        map<something, something else> messages;
        
        int totalMessages;
        time_t timeSpentInCall;
        //include some other stats - like a graph of messages over time and stuff like that
};