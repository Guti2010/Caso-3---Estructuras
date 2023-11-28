FROM debian:buster-slim

RUN apt-get update -qq && apt-get install -y \
    g++ \
    libboost-all-dev \
    libcurl4-gnutls-dev && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/myapp

COPY default/ /usr/src/myapp/default/
COPY . .

RUN g++ -std=c++17 -I /usr/src/myapp/default -o myapp server.cpp -lboost_system -lcurl -pthread -lstdc++fs

CMD ["./myapp"]