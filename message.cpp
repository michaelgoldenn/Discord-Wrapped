#include "message.h"
#include <sstream>

// Function to calculate the word count in the message content
int Message::getWordCount() {
    // Use stringstream to tokenize the message content by spaces
    std::stringstream ss(content);
    std::string word;
    int count = 0;
    while (ss >> word) {
        count++;
    }
    return count;
}