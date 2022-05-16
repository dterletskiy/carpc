#!/bin/bash
#!/usr/bin/env bash



SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

ANTLR4_JAR=${SCRIPT_DIR}/"antlr-4.10.1-complete.jar"
ANTLR4_OPTIONS="-Dlanguage=Python3 -listener -visitor"
ANTLR4="java -jar ${ANTLR4_JAR} ${ANTLR4_OPTIONS}"



OUTPUT_DIR=${1}
shift
G4_FILES=${@}

echo "ANTRL4 code generation..."
echo ${ANTLR4} -o ${OUTPUT_DIR} ${G4_FILES}
${ANTLR4} -o ${OUTPUT_DIR} ${G4_FILES}
