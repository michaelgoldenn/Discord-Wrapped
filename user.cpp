#include "user.h"

time_t userClass::addMessageToUser(Message newMessage){
    messages.insert({newMessage.id, newMessage});

    //update vars accordingly
    totalMessages++;
    totalWords+=newMessage.wordCount;
    updateWordFrequencies(newMessage.content);
    //timeSpentInCall: Do something with that later when we get the implementation specifics worked out
    //implement wordlist later

    return newMessage.timeStamp;
}

void userClass::printMessages(){
    for (const auto& pair : messages) {
        pair.second.printMessage(); // Assuming Message class has a printMessage() function
    }
}

// Function to update word frequencies
void userClass::updateWordFrequencies(const std::string& inputString) {
    std::istringstream iss(inputString);
    std::string word;
    while (iss >> word) {
        // Increment the frequency count for the current word
        this->wordFrequency[word]++;
    }
}

void userClass::printStats(){
    printf("\n");
    printf("Total Messages sent: \t%d\n", totalMessages);
    printf("Total words spoken: \t%lld\n", totalWords);
}