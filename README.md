# Discord-Wrapped

I'm just using this to dump notes about the project

--------------------------------------TESTING RESPONSES------------------------------------------

Using python3

curl --header "Authorization: yourToken" _____ | python3 -m json.tool > out.json
List of open channels as json:                       https://discordapp.com/api/users/@me/channels

Main:
50 most recent messages from specific channel:       https://discord.com/api/v9/channels/684551735645175848/messages?limit=50
50 messages before x message(id?):                   https://discord.com/api/v9/channels/684551735645175848/messages?before=1224209852440510504&limit=50
soviet_steel

Alt:
50 most recent messages from specific channel:       https://discord.com/api/v9/channels/1224614908906180659/messages?limit=50
sayyouwill

Project Team Software Team 5 Spring 2024 Semester Project

run troyDocker.sh to create docker container with a bind mount to /code

use '>Dev Containers: attach to Running container' in vscode at /code with 
Dev Containers extension to use the vscode feature set

modifications to files in /app will be lost via this method when the container

TODO:
- fix timestamp rounding
- add column to users table
- add "first message timestamp" to user objects
- optimize the http request making process by keeping the connection open instead of restarting it for each request
- add "self" information in the same scope as the array of users
    - things like first message ever sent, total messages sent to everyone, total messages received from everyone, etc (totals)
- make "ranking" animation that shows most messaged people over time
    - add all messages to a single multimap in main sorted by timestamp, use channel id of each message to identify which conversation it was from.
    - iterate through every message and add them to a buffer, removing messages from the buffer which are more than 30 days older than the most recently iterated one.
    keep count of the number of messages belonging to each user exist in that time window, and then you could save it as a list of rankings which can be displayed in order
    (one ranking per day, representing the past 30 day totals.)
    - 