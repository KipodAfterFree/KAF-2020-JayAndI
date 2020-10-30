FROM ubuntu:20.04 AS builder

# Install things
RUN apt-get -y update
RUN apt-get -y install wget curl nano
RUN apt-get -y install g++
RUN apt-get -y install cmake

# Install boost headers
RUN mkdir /project
WORKDIR /project

RUN mkdir include

RUN wget https://dl.bintray.com/boostorg/release/1.74.0/source/boost_1_74_0.tar.bz2 -O /tmp/boost.tar.bz2 
RUN cd /tmp && tar --bzip2 -xf /tmp/boost.tar.bz2 && rm /tmp/boost.tar.bz2
RUN cp -r /tmp/boost*/boost include/boost

COPY src ./src
COPY include ./include/
COPY CMakeLists.txt .

RUN cmake -H. -Bbuild
RUN cmake --build build -- -j3


FROM ubuntu:20.04 AS server

WORKDIR /server

COPY res /res
COPY --from=builder /project/bin/server server

RUN chmod +x server

CMD ["./server"]
