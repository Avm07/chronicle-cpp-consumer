FROM debian:buster
RUN apt update
RUN apt install -y git g++ cmake libboost-dev libboost-thread-dev libboost-test-dev libboost-filesystem-dev libboost-date-time-dev libboost-system-dev libboost-iostreams-dev libboost-program-options-dev libboost-locale-dev libssl-dev libgmp-dev pkg-config libzmq5-dev zlib1g-dev
RUN mkdir build
WORKDIR /build
RUN git clone https://github.com/EOSChronicleProject/eos-chronicle.git
WORKDIR /build/eos-chronicle
RUN git submodule update --init --recursive
RUN mkdir build
WORKDIR /build/eos-chronicle/build
RUN cmake ..
# use "make -j N" for N CPU cores for faster compiling (may require more RAM)
RUN make