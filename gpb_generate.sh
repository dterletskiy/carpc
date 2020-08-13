#!/bin/sh


SRC_DIR="/home/scorpion/Source/RPC/application/api/app/proto/"
DST_DIR="/home/scorpion/Source/RPC/application/api/app/proto/"

PROTO_FILE="SensorData.proto"

protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/$PROTO_FILE
