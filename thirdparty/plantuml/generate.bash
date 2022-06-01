#!/bin/bash
#!/usr/bin/env bash



SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

PLANT_UML_JAR=${SCRIPT_DIR}/"plantuml-1.2021.14.jar"
PLANT_UML_OPTIONS="-tsvg -tpng -progress -V -debugsvek"
PLANT_UML="java -jar ${PLANT_UML_JAR} ${PLANT_UML_OPTIONS}"



SOURCE_DIR=${1}
OUTPUT_DIR=${2}

# rm ${OUTPUT_DIR}/*.svg
${PLANT_UML} -o ${OUTPUT_DIR} ${SOURCE_DIR}/*
