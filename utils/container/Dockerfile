# ----------
# Sinai RnD
# ----------

FROM ubuntu:latest

ARG DEBIAN_FRONTEND=noninteractive

# Installing packages and dependencies
RUN apt-get update && apt-get install -y software-properties-common

RUN add-apt-repository ppa:ubuntu-toolchain-r/test

RUN apt-get update && apt-get install -y libstdc++6
