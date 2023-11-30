#!/bin/bash

pushd $GSOAP_HOME

sudo apt-get install flex bison
sudo apt-get install libssl-dev

./configure
make
sudo make install exec_prefix=$GSOAP_HOME

popd
