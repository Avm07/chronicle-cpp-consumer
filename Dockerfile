# В качестве базового образа для сборки используем gcc:latest
FROM eu.gcr.io/list-limited/gcc

WORKDIR /app
COPY . /app
RUN git submodule update --init --recursive

RUN mkdir build && cd build \
      && cmake ..\
      && make 
RUN chmod a+x /app/entrypoint.sh
# Установим рабочую директорию для сборки проекта
WORKDIR /app/build
RUN  ldconfig -v | grep -v ^$'\t'.
ENTRYPOINT [ "/app/build/main", "/app/examples/", "config.json"]

# Запуск ---------------------------------------

# # В качестве базового образа используем ubuntu:latest
# FROM ubuntu:latest

# FROM ubuntu:16.04
# RUN apt-get -y update \
#     && apt-get -y upgrade

# RUN apt-get install -y \
#     openssh-server \
#     g++ \
#     cmake \
#     git 

# #installing the mongoc dependencies and driver
# RUN apt-get install -y \
#     pkg-config \
#     libssl-dev \
#     libsasl2-dev

# RUN cd ~ \
#     && wget https://github.com/mongodb/mongo-c-driver/releases/download/1.6.2/mongo-c-driver-1.6.2.tar.gz \
#     && tar xzf mongo-c-driver-1.6.2.tar.gz \
#     && cd mongo-c-driver-1.6.2 \
#     && ./configure --disable-automatic-init-and-cleanup \
#     && make \
#     && make install \
#     && cd ~ \
#     && rm mongo-c-driver-1.6.2.tar.gz \
#     && rm -rf mongo-c-driver-1.6.2

# #installing mongocxx driver - connects c++ to mongo
# RUN cd ~ \
#     && wget https://github.com/mongodb/mongo-cxx-driver/archive/r3.1.1.tar.gz \
#     && tar -xzf r3.1.1.tar.gz \
#     && cd mongo-cxx-driver-r3.1.1/build \
#     && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local .. \
#     && make EP_mnmlstc_core \
#     && make \
#     && make install \
#     && cd ~ \
#     && rm r3.1.1.tar.gz \
#     && rm -rf mongo-cxx-driver-r3.1.1

# # Добавим пользователя, потому как в Docker по умолчанию используется root
# # Запускать незнакомое приложение под root'ом неприлично :)
# RUN groupadd -r sample && useradd -r -g sample sample
# USER sample

# # Установим рабочую директорию нашего приложения
# WORKDIR /app

# # Скопируем приложение со сборочного контейнера в рабочую директорию
# COPY --from=build /app/build/main .
# RUN  ldconfig -v | grep -v ^$'\t'.

# # Установим точку входа
# ENTRYPOINT ["/app/main"]