FROM carnd-pid-base:latest

COPY . /usr/src/pid
WORKDIR /usr/src/pid
WORKDIR /usr/src/pid/build

RUN cmake ..
RUN make

CMD ["./pid"]

# docker run -p 4567:4567 --rm -it carnd-pid-project:latest