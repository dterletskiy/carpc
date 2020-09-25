#!/bin/sh



reset

rm -R ./delivery

mkdir -p ./delivery/{bin,cfg,include,lib}

cp -R ./framework/api/ ./delivery/include/
cp ./framework_product/lib/libframework.so ./delivery/lib

cp -R ./hooks/api/ ./delivery/include/
cp ./hooks_product/lib/libhooks.so ./delivery/lib

cp ./servicebrocker/servicebrocker.cfg ./delivery/cfg
cp ./servicebrocker_product/exe/servicebrocker ./delivery/bin

cp ./application/application.cfg ./delivery/cfg
cp ./application_product/exe/application ./delivery/bin

cp ./core/core.cfg ./delivery/cfg
cp ./core_product/exe/core ./delivery/bin

cp ./controller/controller.cfg ./delivery/cfg
cp ./controller_product/exe/controller ./delivery/bin

cd delivery

# export LD_LIBRARY_PATH="./lib"

# ./bin/servicebrocker config=./cfg/servicebrocker.cfg &
# ./bin/core config=./cfg/core.cfg
# ./bin/controller config=./cfg/controller.cfg
