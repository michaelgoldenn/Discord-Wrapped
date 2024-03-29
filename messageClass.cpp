#include <ctime>
#include <string>

//NOTE: EVERYTHING IN HERE IS SUBJECT TO DRAMATIC CHANGE - nothing is confirmed so methods/variables could be added/deleted whenever

//Stores info about one message, can set/get all the variables in the message class
class Message {
    public:
        time_t timeStamp;
        string content;

        int pingCount;
        int wordCount;
        bool isReply;
        time_t callTime; //If the message was a call, how long was it? If it isn't a call, should be -1
        bool containsLink;

        //Message Constructor
        Message(time_t time, string message){
            timeStamp = time;
            content = message;
            //fill in the other variables from the message
            callTime = getCallTime();
            pingCount = getPingCount();
            wordCount = getWordCount();
            isReply = getIsReply();
            containsLink = getContainsLink();
        }

    private:
        //Gets pingCount from the content
        int getPingCount(){
            if (str == NULL)
                return -1;
            //AWFUL implementation - just counts the nubmer of times a message has @. *please* change this later
            int count = 0;
            for (int i=0; i<content.size(); i++){
                if (content[i] == "@"){
                    count++;
                }
            }
            return count;
        }


        //Gets word from the content
        int getWordCount(){
            if (str == NULL)
                return -1;

            bool inSpaces = true;
            int numWords = 0;

            while (*str != '\0')
            {
                if (std::isspace(*str)){
                    inSpaces = true;
                } else if (inSpaces){
                    numWords++;
                    inSpaces = false;
                }

                ++str;
            }

            return numWords;
        }


        //Gets if it is a reply from the content
        bool getIsReply(){
            //waiting till html things are figured out for this implementation
            return isReply;
        }


        //Gets if it is a message indicating the end of a call
        time_t getCallTime(){
            //waiting till html things are figured out for this implementation
            return 0;
        }


        //Gets if the message contains a link
        //not great but should work
        bool getContainsLink(){
            if (std::regex_match ("http://subject", std::regex("^(https?:\/\/)?([\da-z\.-]+)\.([a-z\.]{2,6})([\/\w \.-]*)*\/?$") )) {
                return true;
            }
            return false;
        }
};

int main(){return 0;};
