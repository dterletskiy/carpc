#!/usr/bin/env bash

source base.bash



ROOT_DIR=${PWD}
SOURCE_DIR=${ROOT_DIR}/source
BUILD_DIR=${ROOT_DIR}/build
GEN_DIR=${ROOT_DIR}/gen
API_DIR=
IMP_DIR=

LEXER_GRAMMAR_FILE=IdlLexer.g4
PERSER_GRAMMAR_FILE=IdlParser.g4
SOURCE_FILE=onoff.idl

GENERATOR_FILE=generator.py

ANTLR_JAR="/home/dmytro_terletskyi/.local/lib/antlr/antlr-4.10.1-complete.jar"
ANTLR4="java -jar ${ANTLR_JAR}"



function build_g4( )
{
   local G4_FILE=${1}
   ${ANTLR4} \
      -Dlanguage=Python3 \
      ${SOURCE_DIR}/grammar/${G4_FILE} \
      -o ${BUILD_DIR} \
      -listener \
      -visitor
}

function test_action( )
{
   if [ -z ${ACTION+x} ]; then
      print_error "Action is unset";
      exit 1
   elif [ -z ${ACTION} ]; then
      print_error "Action is empty";
      exit 1
   else
      print_info "Action is '${ACTION}'";
   fi
}

function test_target( )
{
   if [ -z ${TARGET+x} ]; then
      print_error "Target is unset";
      # exit 1
   elif [ -z ${TARGET} ]; then
      print_error "Target is empty";
      # exit 1
   else
      print_info "Target is '${TARGET}'";
   fi
}

function main_menu( )
{
   print_header "Main Menu"

   for option in "$@"; do
      case ${option} in
         --action=*)
            ACTION="${option#*=}"
            shift # past argument=value
         ;;
         --target=*)
            TARGET="${option#*=}"
            shift # past argument=value
         ;;
         --source=*)
            SOURCE_FILE="${option#*=}"
            shift # past argument=value
         ;;
         --build=*)
            BUILD_DIR="${option#*=}"
            shift # past argument=value
         ;;
         --gen=*)
            GEN_DIR="${option#*=}"
            shift # past argument=value
         ;;
         --api=*)
            API_DIR="${option#*=}"
            shift # past argument=value
         ;;
         --imp=*)
            IMP_DIR="${option#*=}"
            shift # past argument=value
         ;;
         *)
            print_error "undefined option: '${option}'"
            shift # past argument=value
         ;;
      esac
   done

   test_action
   test_target

   case ${ACTION} in
      build)
         if [ ! -d ${BUILD_DIR} ]; then
            print_info "Creating directory" ${BUILD_DIR}
            mkdir ${BUILD_DIR}
         fi
         build_g4 ${LEXER_GRAMMAR_FILE}
         build_g4 ${PERSER_GRAMMAR_FILE}
      ;;
      clean)
         rm -rf ${BUILD_DIR}
      ;;
      run)
         ${SOURCE_DIR}/${GENERATOR_FILE} \
            --source=${SOURCE_DIR}/idl/${SOURCE_FILE} \
            --import=${BUILD_DIR}

            # --gen=${GEN_DIR}
      ;;
      *)
         print_error "Undefined action:" ${ACTION}
      ;;
   esac
}



main_menu $@
