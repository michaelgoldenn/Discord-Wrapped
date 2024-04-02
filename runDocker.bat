:: This is a bat file that builds and runs a docker container
:: Needs work. Right now it rebuilds every time, which is unneccesary. 
:: Also go into docker desktop every once in a while and remove all images,
:: builds, and containers to free drive space

:: Instructions for use:
:: 1. Install Docker Desktop: https://desktop.docker.com/win/main/amd64/Docker%20Desktop%20Installer.exe?utm_source=docker&utm_medium=webreferral&utm_campaign=dd-smartbutton&utm_location=module
:: 2. Sign in if they make you
:: 3. run this file, it'll print the output of the `main` file

@echo off
docker build . -t discord_wrapped
echo -------------- Main Output --------------
docker run -it discord_wrapped
echo. 
pause
