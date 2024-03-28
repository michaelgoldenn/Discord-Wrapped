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


# Install general stuff
RUN apt-get update
RUN apt-get install build-essential

# make program
RUN make -C /app

#run program
CMD ["/app/main"]

#run forever (for debugging)
# CMD ["tail", "-f", "/dev/null"]