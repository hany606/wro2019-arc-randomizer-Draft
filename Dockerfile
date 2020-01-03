FROM ubuntu:18.04

WORKDIR /home

RUN apt-get -qqy update
RUN apt-get -qqy install g++ gcc cmake git nano build-essential

RUN mkdir shared
