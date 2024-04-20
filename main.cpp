#include <iostream>
#include <chrono>
#include <regex>
#include <iomanip>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <curl/curl.h>
#include <thread>
#include <ctime>
#include <sstream>
#include <future>
#include <sqlite3.h>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "user.h"

std::vector<User> users;
int timeZoneOffset = 0;

int hourCounters[24] = {0};

int totalMessagesSent = 0;
int totalMessagesReceived = 0;
int totalCharactersSent = 0;
int totalCharactersReceived = 0;

int totalCallsSent = 0;
int totalCallsReceived = 0;
double totalTimeCalling = 0;

User oldestMessage;

// function to format a timestamp based on the global timezone offset
std::string formatTimestamp(const std::string& timestamp) {
    std::tm tm = {};
    std::istringstream ss(timestamp);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) {
        return "Invalid timestamp format";
    }

    tm.tm_hour += timeZoneOffset; // Subtract timeZoneOffset hours
    if (tm.tm_hour < 0) {
        tm.tm_hour += 24; // Add 24 hours to handle negative values
        tm.tm_mday--; // Adjust day accordingly
    }

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << tm.tm_mon + 1 << '/'
        << std::setw(2) << tm.tm_mday << '/'
        << std::setw(2) << tm.tm_year % 100 << ' '
        << std::setw(2) << tm.tm_hour << ':'
        << std::setw(2) << tm.tm_min;
    return oss.str();
}

// Function to update the hour counter based on a timestamp and timezone offset
void updateHourCounter(const std::string& timestamp) {
    // Extract the hour component from the timestamp
    std::string hourStr = timestamp.substr(11, 2); // Assuming the timestamp format is consistent
    int hour = std::stoi(hourStr);

    // Adjust the hour based on the timezone offset
    hour = (hour + timeZoneOffset + 24) % 24; // Ensure the result is within 0-23 range

    // Increment the corresponding counter in the array
    if (hour >= 0 && hour < 24) {
        hourCounters[hour]++;
    }
}

// replaces unprintable characters with _ for now
std::string replaceSpecialChars(const std::string& str) {
    std::regex pattern("[^[:print:]]");
    return std::regex_replace(str, pattern, "_");
}

// function to truncate strings longer than 20 characters with ...
std::string truncateString(const std::string &str, int maxLength) {
    if (str.length() > (size_t)maxLength) {
        return str.substr(0, maxLength - 3) + "...";
    }
    return str;
}

// clears line and replaces it with input message
void updateTerminalLine(const std::string& message) {
    std::cout << "\033[1G"; // move cursor to the beginning of the line
    std::cout << "\033[2K"; // clear the entire line
    std::cout << message << std::flush;
}

// callback function to receive HTTP response
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *data) {
    data->append((char *)contents, size * nmemb);
    return size * nmemb;
}

// Perform HTTP request and parse response into RapidJSON document
bool performHttpRequest(const std::string& url, const std::string& auth_token, rapidjson::Document& doc) {
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (!curl) {
        std::cout << "failed to initialize libcurl." << std::endl;
        return false;
    }

    struct curl_slist *headers = NULL;
    std::string auth_header = "Authorization: " + auth_token;
    headers = curl_slist_append(headers, auth_header.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    std::string response_data;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cout << "failed to perform HTTP request: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return false;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    // Parse the response data into the RapidJSON document
    doc.Parse(response_data.c_str());
    if (doc.HasParseError()) {
        std::cout << "failed to parse JSON response." << std::endl;
        return false;
    }

    return true;
}

// print a given message
void printMessage(const Message& message) {
    std::cout << "timestamp: " << formatTimestamp(message.timestamp) << " | message_id: " << message.id << " | author: " << message.author << " - content: " << message.content << std::endl;
}

// print a given call
void printCall(const Call& call) {
    std::cout << "timestamp: " << formatTimestamp(call.timestamp) << " | message_id " << call.id << " | author: " << call.author << " - ended timestamp: " << formatTimestamp(call.ended_timestamp) << std::endl;
}

// Creates a "super user" that has the total from all other users
User addAllUsers() {
    // Initialize a 'super user' with empty or zero values
    User superUser("", "", "", "total", "super_user", "");

    // Aggregate data from each user
    for (const auto& user : users) {
        superUser.totalMessages += user.totalMessages;
        superUser.messagesSent += user.messagesSent;
        superUser.messagesReceived += user.messagesReceived;
        superUser.charactersSent += user.charactersSent;
        superUser.charactersReceived += user.charactersReceived;
        superUser.totalCalls += user.totalCalls;
        superUser.callsSent += user.callsSent;
        superUser.callsReceived += user.callsReceived;
        superUser.callTime += user.callTime;

        // Combine messages and calls multimap
        superUser.messages.insert(user.messages.begin(), user.messages.end());
        superUser.calls.insert(user.calls.begin(), user.calls.end());
    }

    return superUser;
}

void processCommand(const std::string& command) {
    std::istringstream iss(command);
    std::vector<std::string> tokens { std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };

    if (tokens.empty()) {
        std::cerr << "invalid command." << std::endl;
        return;
    }

    if (tokens[0] == "get") {
        if (tokens.size() < 3) {
            std::cerr << "invalid command format. usage: get <global_name> messages" << std::endl;
            return;
        }

        std::string username = tokens[1]; // extract the user name
        std::string commandType = tokens[2]; // extract command type

        if (commandType == "messages") {
            std::cout << std::endl;
            std::cout << "fetching messages for " << username << std::endl;
            for (const auto& user : users) {
                if (user.username == username) {
                    for (const auto& message : user.messages) {
                        printMessage(message.second);
                    }
                    break; // stop iterating once the messages for the target user are printed
                }
            }
        } else if (commandType == "calls") {
            std::cout << std::endl;
            std::cout << "fetching calls for " << username << std::endl;
            for (const auto& user : users) {
                if (user.username == username) {
                    for (const auto& call : user.calls) {
                        printCall(call.second);
                    }
                    break; // stop iterating once the messages for the target user are printed
                }
            }
        } else {
            std::cerr << "invalid command format. Usage: get <global_name> messages" << std::endl;
        }
    } else if (command == "users") {
        std::cout << std::endl;
        std::cout << "printing list of users with various stats." << std::endl;
        // table headers
        std::cout << "+----------------------+--------------+--------------+--------------+--------------+------------+-------------------+" << std::endl;
        std::cout << "| " << std::setw(20) << std::left << "username" << " | "
                << std::setw(12) << std::left << "total msgs" << " | "
                << std::setw(12) << std::left << "chars sent" << " | "
                << std::setw(12) << std::left << "chars recv" << " | "
                << std::setw(12) << std::left << "total calls" << " | "
                << std::setw(10) << std::left << "call time" << " |"
                << std::setw(18) << std::left << "oldest msg time" << " |" << std::endl;
        std::cout << "+----------------------+--------------+--------------+--------------+--------------+------------+-------------------+" << std::endl;
        // print the list of users with formatted info
        for (const auto &user : users) {
            std::string formatted_id = truncateString(user.username, 20);
            std::string formatted_total_msgs = std::to_string(user.totalMessages);
            std::string formatted_chars_sent = std::to_string(user.charactersSent);
            std::string formatted_chars_recv = std::to_string(user.charactersReceived);
            std::string formatted_total_calls = std::to_string(user.totalCalls);

            // round callTime to two decimal places
            std::stringstream call_time_ss;
            call_time_ss << std::fixed << std::setprecision(2) << user.callTime;
            std::string formatted_call_time = call_time_ss.str();

            std::cout << "| " << std::setw(20) << std::left << formatted_id << " | "
                    << std::setw(12) << std::left << formatted_total_msgs << " | "
                    << std::setw(12) << std::left << formatted_chars_sent << " | "
                    << std::setw(12) << std::left << formatted_chars_recv << " | "
                    << std::setw(12) << std::left << formatted_total_calls << " | "
                    << std::setw(10) << std::left << formatted_call_time << " |"
                    << std::setw(18) << std::left << formatTimestamp(user.messages.begin() -> first) << " |" << std::endl;
        }
        std::cout << "+----------------------+--------------+--------------+--------------+--------------+------------+-------------------+" << std::endl;
        //just copied what came above for the "Total" line
        User superUser = addAllUsers();
        std::string formatted_id = truncateString(superUser.username, 20);
        std::string formatted_total_msgs = std::to_string(superUser.totalMessages);
        std::string formatted_chars_sent = std::to_string(superUser.charactersSent);
        std::string formatted_chars_recv = std::to_string(superUser.charactersReceived);
        std::string formatted_total_calls = std::to_string(superUser.totalCalls);

        // sound callTime to two decimal places
        std::stringstream call_time_ss;
        call_time_ss << std::fixed << std::setprecision(2) << superUser.callTime;
        std::string formatted_call_time = call_time_ss.str();

        std::cout << "| " << std::setw(20) << std::left << formatted_id << " | "
                << std::setw(12) << std::left << formatted_total_msgs << " | "
                << std::setw(12) << std::left << formatted_chars_sent << " | "
                << std::setw(12) << std::left << formatted_chars_recv << " | "
                << std::setw(12) << std::left << formatted_total_calls << " | "
                << std::setw(10) << std::left << formatted_call_time << " |"
                << std::setw(18) << std::left << formatTimestamp(superUser.messages.begin() -> first) << " |" << std::endl;
        std::cout << "+----------------------+--------------+--------------+--------------+--------------+------------+-------------------+" << std::endl;
        std::cout << std::endl;
    } else if (command == "exit") {
        std::cout << std::endl;
        std::cout << "exiting program..." << std::endl;
        exit(0);
    } else if (command == "stats") {
        std::cout << std::endl;
        std::cout << "complete, total statistics for yourself:"                 << std::endl;          
        std::cout << "messages sent:          " << totalMessagesSent            << std::endl;
        std::cout << "messages received:      " << totalMessagesReceived        << std::endl;
        std::cout << "characters sent:        " << totalCharactersSent          << std::endl;
        std::cout << "characters received:    " << totalCharactersReceived      << std::endl;
        std::cout << "calls started:          " << totalCallsSent               << std::endl;
        std::cout << "calls received:         " << totalCallsReceived           << std::endl;
        std::cout << "hours spent in a call:  " << totalTimeCalling             << std::endl;
        std::cout << "the first message ever sent/received on your account:"    << std::endl;
        printMessage(oldestMessage.messages.begin() -> second);
        std::cout << std::endl;
    } else if (command == "times") {
        std::cout << std::endl;
        std::cout << "printing total messages sent each hour of the day" << std::endl;
        std::cout << std::endl;
        std::cout << "   hour | messages" << std::endl;
        std::cout << "--------------------" << std::endl;
        for (int hour = 0; hour < 24; hour++) {
        // print the hour and message count
        std::cout << std::right << std::setw(4) << hour << ":00 | " << std::setw(8) << hourCounters[hour] << std::endl;
    }
    std::cout << std::endl;
    } else {
        std::cout << std::endl;
        std::cout << "invalid command." << std::endl;
        std::cout << std::endl;
    }
}

std::string getOlderTimestamp(const std::string& timestamp1, const std::string& timestamp2) {
    std::tm tm1 = {}, tm2 = {};
    strptime(timestamp1.c_str(), "%Y-%m-%dT%H:%M:%S", &tm1);
    strptime(timestamp2.c_str(), "%Y-%m-%dT%H:%M:%S", &tm2);

    std::time_t time1 = std::mktime(&tm1);
    std::time_t time2 = std::mktime(&tm2);

    return (time1 < time2) ? timestamp1 : timestamp2;
}

const User& getOldestFirstMessageUser(const User& user1, const User& user2) {
    if (user1.messages.empty() && user2.messages.empty()) {
        throw std::invalid_argument("Both users have no messages.");
    } else if (user1.messages.empty()) {
        return user2;
    } else if (user2.messages.empty()) {
        return user1;
    }

    const std::string& firstMsgTime1 = user1.messages.begin() -> first;
    const std::string& firstMsgTime2 = user2.messages.begin() -> first;

    std::tm tm1 = {}, tm2 = {};
    strptime(firstMsgTime1.c_str(), "%Y-%m-%dT%H:%M:%S", &tm1);
    strptime(firstMsgTime2.c_str(), "%Y-%m-%dT%H:%M:%S", &tm2);

    std::time_t time1 = std::mktime(&tm1);
    std::time_t time2 = std::mktime(&tm2);

    return (time1 < time2) ? user1 : user2;
}

// TODO: this is ass
// function to calculate the time difference in hours between two timestamps
double calculateCallDuration(const std::string& startTimestamp, const std::string& endTimestamp) {
    std::tm start_tm = {};
    std::tm end_tm = {};

    std::istringstream start_ss(startTimestamp);
    start_ss >> std::get_time(&start_tm, "%Y-%m-%dT%H:%M:%S%z");

    std::istringstream end_ss(endTimestamp);
    end_ss >> std::get_time(&end_tm, "%Y-%m-%dT%H:%M:%S%z");

    std::chrono::system_clock::time_point start = std::chrono::system_clock::from_time_t(std::mktime(&start_tm));
    std::chrono::system_clock::time_point end = std::chrono::system_clock::from_time_t(std::mktime(&end_tm));

    std::chrono::duration<double> duration = end - start;
    double hours = duration.count() / 3600.0; // Convert seconds to hours

    return hours;
}

// function to map symbolic time zones to their offset values
int getTimeZoneOffset(const std::string& symbolicTimeZone) {
    if (symbolicTimeZone == "EST") {
        return -5; // Eastern Standard Time (UTC-5)
    } else if (symbolicTimeZone == "PST") {
        return -8; // Pacific Standard Time (UTC-8)
    } else if (symbolicTimeZone == "GMT") {
        return 0; // Greenwich Mean Time (UTC+0)
    } else {
        std::cout << "timezone not recognized, defaulting to GMT (-0)" << std::endl;
        return 0; // Default to GMT if symbolic timezone is not recognized
    }
}

int main(int argc, char *argv[]) {
    std::cout << std::endl;
    std::string auth_token;

    try {
        if (argc != 3) {
            throw std::invalid_argument("Usage: <symbolicTimeZone> <auth_token>");
        }

        timeZoneOffset = getTimeZoneOffset(argv[1]); // Set global timezone offset from symbolic timezone
        auth_token = argv[2];                        // Set auth token from command line
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "requesting your discord channel list..." << std::endl;

    // perform http request to fetch channel list
    std::string url = "https://discordapp.com/api/users/@me/channels";
    rapidjson::Document channelDoc;
    if (!performHttpRequest(url, auth_token, channelDoc)) {
        std::cerr << "failed to fetch channel list." << std::endl;
        return 1;
    }

    std::cout << "channel list request performed." << std::endl;

    // parse JSON response using RapidJSON
    if (!channelDoc.IsArray()) {
        std::cerr << "invalid JSON response format." << std::endl;
        return 1;
    }
    
    // process JSON data into User objects, discarding non type 1/DM channels
    int invalidChannels = 0; // number of channels that need to be skipped due to invalid formatting
    if (channelDoc.IsArray()) {
        for (rapidjson::SizeType i = 0; i < channelDoc.Size(); i++) {
            const rapidjson::Value &channelJson = channelDoc[i];
            int type = channelJson["type"].GetInt();
            if (type != 1) {
                continue; // skip channels that are not type 1
            }

            // a ton of conditions that need to be met for the channel to be processed as a new user
            if (!channelJson.IsObject() || !channelJson.HasMember("id") || !channelJson["id"].IsString()
                || !channelJson.HasMember("type") || !channelJson["type"].IsInt()
                || !channelJson.HasMember("last_message_id") || !channelJson["last_message_id"].IsString()
                || !channelJson.HasMember("recipients") || !channelJson["recipients"].IsArray()
                || channelJson["recipients"].Size() < 1 || !channelJson["recipients"][0].IsObject()
                || !channelJson["recipients"][0].HasMember("id") || !channelJson["recipients"][0]["id"].IsString()
                || !channelJson["recipients"][0].HasMember("username") || !channelJson["recipients"][0]["username"].IsString()
                || !channelJson["recipients"][0].HasMember("global_name") || !channelJson["recipients"][0]["global_name"].IsString()
                || !channelJson["recipients"][0].HasMember("discriminator") || !channelJson["recipients"][0]["discriminator"].IsString()) {
                invalidChannels++;
                continue;
            }

            // process channel with type 1
            std::string channel = channelJson["id"].GetString();
            std::string last_message_id = channelJson["last_message_id"].GetString();
            std::string id = channelJson["recipients"][0]["id"].GetString();
            std::string username = channelJson["recipients"][0]["username"].GetString();
            std::string global_name = channelJson["recipients"][0]["global_name"].GetString();
            std::string discriminator = channelJson["recipients"][0]["discriminator"].GetString();

            // create User object and add to vector
            User user(channel, id, last_message_id, username, global_name, discriminator);
            users.push_back(user);
        }
    }
    std::cerr << "skipped " << invalidChannels << " channel(s) due to invalid formatting, usually a result of being empty." << std::endl << std::endl;
    channelDoc.Clear();

    // iterate through every user, grab their messages, and add them to the call/message multimaps respectively. also counts some stats.
    for (auto& user : users) {

        int totalMessages = 0;
        int messagesSent = 0;
        int messagesReceived = 0;
        int charactersSent = 0;
        int charactersReceived = 0;

        int totalCalls = 0;
        int callsSent = 0;
        int callsReceived = 0;
        int callTime = 0;

        int httpIterations = 1; // entirely used for the silly ... animation
        bool isFirstIteration = true; // need to know whether we need a last message id or not
        bool isLastIteration = false; // need to know whether we need more iterations after this one
        std::string lastProcessedMessageId; // id of the last message processed in the previous iteration

        while (!isLastIteration) {
            // iteration "animation"
            if (httpIterations % 4 == 0) {
                updateTerminalLine("requesting Discord messages for " + user.username);
                httpIterations = 0;
            } else if (httpIterations % 3 == 0) {
                updateTerminalLine("requesting Discord messages for " + user.username + "...");
            } else if (httpIterations % 2 == 0) {
                updateTerminalLine("requesting Discord messages for " + user.username + "..");
            } else {
                updateTerminalLine("requesting Discord messages for " + user.username + ".");
            }

            std::string url;
            if (isFirstIteration) {
                url = "https://discord.com/api/v9/channels/" + user.channel + "/messages?limit=100";
            } else {
                url = "https://discord.com/api/v9/channels/" + user.channel + "/messages?before=" + lastProcessedMessageId + "&limit=100";
            }

            rapidjson::Document messageDoc;
            if (!performHttpRequest(url, auth_token, messageDoc)) {
                updateTerminalLine("failed to fetch messages for user: " + user.username);
                std::this_thread::sleep_for(std::chrono::seconds(2)); // wait 2 second to show that fetching failed
                continue; // move to the next user
            }

            if (!messageDoc.IsArray()) {
                updateTerminalLine("invalid JSON response for user: " + user.username);
                std::this_thread::sleep_for(std::chrono::seconds(2)); // wait 2 second to indicate an issue with the response
                continue; // move to the next user
            }

            int messagesThisIteration = 0;

            // MAJOR DATA PROCESSING LETS GOOOO
            for (rapidjson::SizeType i = 0; i < messageDoc.Size(); i++) {
                const rapidjson::Value& messageJson = messageDoc[i];
                int messageType = messageJson["type"].GetInt();

                if (messageType == 0 || messageType == 19) {
                    // process message object
                    std::string id = messageJson["id"].GetString();
                    std::string content = messageJson["content"].GetString();
                    std::string author = messageJson["author"]["username"].GetString();
                    std::string channel_id = messageJson["channel_id"].GetString();
                    std::string timestamp = messageJson["timestamp"].GetString();
                    bool mentioned = !messageJson["mentions"].Empty(); // check if mentions array is not empty

                    // create and add message object to user's messages multimap
                    Message message(id, content, author, channel_id, timestamp, mentioned);
                    user.messages.emplace(message.timestamp, std::move(message));

                    // iterators
                    totalMessages++;                                                    // total messages
                    if (user.username == author) {
                        messagesReceived++;                                             // total received messages
                        charactersReceived = charactersReceived + content.length();     // total received characters
                    } else {
                        messagesSent++;                                                 // total sent messages
                        updateHourCounter(timestamp);                                   // add your messages timestamp to hourCounter
                        charactersSent = charactersSent + content.length();             // total sent characters
                    }
                    
                    messagesThisIteration++;
                } else if (messageType == 3) {
                    // process call object

                    if (messageJson["call"]["ended_timestamp"].IsNull()) {
                        messagesThisIteration++;
                        continue;
                    }

                    std::string id = messageJson["id"].GetString();
                    std::string timestamp = messageJson["timestamp"].GetString();
                    std::string ended_timestamp = messageJson["call"]["ended_timestamp"].GetString();
                    std::string author = messageJson["author"]["username"].GetString();
                    std::string channel_id = messageJson["channel_id"].GetString();
                    double duration = calculateCallDuration(timestamp, ended_timestamp);

                    // create and add call object to user's calls multimap
                    Call call(id, timestamp, ended_timestamp, author, channel_id);
                    user.calls.emplace(call.timestamp, std::move(call));
                    call.duration = duration;

                    // iterators
                    totalCalls++;                                                       // total calls
                    if (user.username == author) {
                        callsReceived++;                                                // total calls received
                    } else {
                        callsSent++;                                                    // total calls sent
                    }
                    callTime = callTime + duration;                                     // total time on call

                    messagesThisIteration++;
                } else {
                    messagesThisIteration++;
                }
            }

            const rapidjson::Value& messages = messageDoc.GetArray();
            if (!messages.Empty()) {
                lastProcessedMessageId = messages[messages.Size() - 1]["id"].GetString();
            }

            messageDoc.Clear();
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // avoid rate limiting
            isFirstIteration = false;
            if (messagesThisIteration < 100) {
                isLastIteration = true;
            }
            httpIterations++;
        } 

        // check if this user had an older message than the previous oldest message exchanger
        oldestMessage = getOldestFirstMessageUser(user, oldestMessage);

        // add to individual user totals
        user.totalMessages = totalMessages;
        user.messagesSent = messagesSent;
        user.messagesReceived = messagesReceived;
        user.charactersSent = charactersSent;
        user.charactersReceived = charactersReceived;

        user.totalCalls = totalCalls;
        user.callsSent = callsSent;
        user.callsReceived = callsReceived;
        user.callTime = callTime;

        // add to overall totals
        totalMessagesSent += messagesSent;
        totalMessagesReceived += messagesReceived;
        totalCharactersSent += charactersSent;
        totalCharactersReceived += charactersReceived;

        totalCallsSent += callsSent;
        totalCallsReceived += callsReceived;
        totalTimeCalling += callTime;
    }
    updateTerminalLine("finished fetching all user messages.");
    std::cout << std::endl << std::endl;

    std::string command;
    while (true) {
        std::cout << "enter one of the following commands:"                                                             << std::endl;
        std::cout << "- stats:                          - returns list of overall personal statistics"                  << std::endl;
        std::cout << "- times:                          - returns list of overall message sending times"                << std::endl;
        std::cout << "- users:                          - returns list of DMs open with user info"                      << std::endl;
        std::cout << "- get <username> messages:        - returns list of messages for given user"                      << std::endl;
        std::cout << "- get <username> calls:           - returns list of calls for given user"                         << std::endl;
        std::cout << "- exit:                           - exits the program"                                            << std::endl;
        std::getline(std::cin, command);

        // Process the command
        processCommand(command);
    }

    return 0;
}