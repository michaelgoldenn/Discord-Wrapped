#include "message.h"

// Message Constructor
Message::Message(time_t time, std::string message){
    timeStamp = time;
    content = message;
    // fill in the other variables from the message
    callTime = getCallTime();
    pingCount = getPingCount();
    wordCount = getWordCount();
    isReply = getIsReply();
    containsLink = getContainsLink();
}

// Prints the contents of the message to the terminal
void Message::printMessage(){    
    printf("\nPrinting Message:\n");
    printf("Content: \t%s\n", content.c_str());
    printf("Word Count:\t%d\n", wordCount);
    printf("Timestamp: \t$");
    // put the time into a string
    char buffer[32];
    std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", std::localtime(&timeStamp));  
    std::cout << buffer << "\n";

    printf("Call Time: \t%ld\n", callTime);
    printf("Contains Link: \t%s\n", containsLink?"true":"false");
    printf("Is Reply: \t%s\n", isReply?"true":"false");
    printf("Ping Count: \t%d\n", pingCount);
}

// Gets pingCount from the content
int Message::getPingCount(){
    // AWFUL implementation - just counts the number of times a message has @. *please* change this later
    int count = 0;
    for (int i=0; i<content.size(); i++){
        if (content[i] == '@'){
            count++;
        }
    }
    return count;
}

// Gets word from the content
int Message::getWordCount(){
    bool inSpaces = true;
    int numWords = 0;
    const char* str = content.c_str();

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

// Gets if it is a reply from the content
bool Message::getIsReply(){
    // waiting till html things are figured out for this implementation
    return false;
}

// Gets amount of time spent calling
time_t Message::getCallTime(){
    // waiting till html things are figured out for this implementation
    return 0;
}

// Gets if the message contains a link
bool Message::getContainsLink() {
    if (std::regex_search(content, std::regex("(https?://)?([\\w\\.-]+)\\.([a-z]{2,})(/[\\w\\.-]*)*"))) {
        return true;
    }
    return false;
}

