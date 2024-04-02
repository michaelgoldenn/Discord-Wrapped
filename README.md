# Discord-Wrapped

I'm just using this to dump notes about the project

--------------------------------------TESTING RESPONSES------------------------------------------

Using python3

curl --header "Authorization: yourToken" _____ | python3 -m json.tool > out.json
List of open channels as json:                       https://discordapp.com/api/users/@me/channels
50 most recent messages from specific channel:       https://discord.com/api/v9/channels/684551735645175848/messages?limit=50
50 messages before x message(id?):                   https://discord.com/api/v9/channels/684551735645175848/messages?before=1224209852440510504&limit=50

Project Team Software Team 5 Spring 2024 Semester Project

run troyDocker.sh to create docker container with a bind mount to /code

use '>Dev Containers: attach to Running container' in vscode at /code with 
Dev Containers extension to use the vscode feature set

modifications to files in /app will be lost via this method when the container
exits