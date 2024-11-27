#! /usr/bin/bash

topic="$1"
message="$2"
mosquitto_pub -h $(hostname -I) -t $topic  -m $message
