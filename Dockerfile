# Use an official C++ runtime as a parent image
FROM gcc:latest

# Copy the current directory contents into the container at /app
ADD . /app

# Install libxlsxwriter
RUN mkdir /app/libs
RUN git clone https://github.com/jmcnamara/libxlsxwriter.git /app/libs/libxlsxwriter
RUN (cd /app/libs/libxlsxwriter && make)
RUN (cd /app/libs/libxlsxwriter && make install)
ENV LD_LIBRARY_PATH=/app/libs/libxlsxwriter/lib:${LD_LIBRARY_PATH}
RUN echo '/usr/local/lib' | tee -a /etc/ld.so.conf.d/x86_64-linux-gnu.conf && ldconfig

# Install stuff
# curl and python3 for testing requests (see README)
# vim because reasons
# libcurl library for c++ code that sends http requests
# rapidjson library for interpreting json response
RUN apt-get update && apt-get install -y curl vim python3-pip rapidjson-dev libcurl4-openssl-dev sqlite3 libsqlite3-dev

# Install general stuff
RUN apt-get update
RUN apt-get install build-essential

# make program
RUN make -C /app

#run program
CMD ["/app/main"]

#run forever (for debugging)
# CMD ["tail", "-f", "/dev/null"]