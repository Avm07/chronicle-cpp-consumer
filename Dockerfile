FROM gcc:latest

RUN wget https://github.com/Kitware/CMake/releases/download/v3.14.3/cmake-3.14.3-Linux-x86_64.sh \
      -q -O /tmp/cmake-install.sh \
      && chmod u+x /tmp/cmake-install.sh \
      && mkdir /usr/bin/cmake \
      && /tmp/cmake-install.sh --skip-license --prefix=/usr/bin/cmake \
      && rm /tmp/cmake-install.sh

ENV PATH="/usr/bin/cmake/bin:${PATH}"

RUN cd /tmp && wget http://downloads.sourceforge.net/project/boost/boost/1.70.0/boost_1_70_0.tar.gz \
      && tar xfz boost_1_70_0.tar.gz \
      && rm boost_1_70_0.tar.gz \
      && cd boost_1_70_0 \
      && ./bootstrap.sh --prefix=/usr/local --with-libraries=program_options,date_time,log,system,filesystem,thread,regex,chrono,atomic \
      && ./b2 install link=static link=shared \
      && cd /home 

RUN apt-get update && apt-get -y install pkg-config libssl-dev libsasl2-dev libpthread-stubs0-dev -y

RUN cd /tmp && wget https://github.com/mongodb/mongo-c-driver/releases/download/1.14.0/mongo-c-driver-1.14.0.tar.gz \
      && tar xzf mongo-c-driver-1.14.0.tar.gz \
      && cd mongo-c-driver-1.14.0 \
      && mkdir cmake-build \
      && cd cmake-build \
      && cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF .. \
      && make \
      && make install 

RUN   cd /tmp && curl -OL https://github.com/mongodb/mongo-cxx-driver/archive/r3.4.0.tar.gz \
      && tar -xzf r3.4.0.tar.gz \
      && cd mongo-cxx-driver-r3.4.0/build \
      && cmake  -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local .. \
      && make install 

RUN ldconfig -v

WORKDIR /app
COPY . /app
RUN git submodule update --init --recursive

RUN mkdir build && cd build \
      && cmake ..\
      && make 

RUN chmod a+x /app/entrypoint.sh
