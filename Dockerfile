# Use an official C++ runtime as a parent image
FROM gcc:latest

# Set the working directory in the container
WORKDIR /code

# Install libxlsxwriter
RUN git clone https://github.com/jmcnamara/libxlsxwriter.git /libs/libxlsxwriter
RUN (cd /libs/libxlsxwriter && make)
RUN (cd /libs/libxlsxwriter && make install)
ENV LD_LIBRARY_PATH=/libs/libxlsxwriter/lib:${LD_LIBRARY_PATH}
RUN echo '/usr/local/lib' | tee -a /etc/ld.so.conf.d/x86_64-linux-gnu.conf && ldconfig

# Install dependencies
RUN apt-get update && apt-get install -y curl vim python3-pip rapidjson-dev libcurl4-openssl-dev sqlite3 libsqlite3-dev build-essential

# Build the program
COPY . .
RUN make

# Run the program
CMD ["./main"]
