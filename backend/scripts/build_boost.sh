#!/bin/bash

pushd $BOOST_HOME
./bootstrap.sh --prefix=$BOOST_BUILD
./b2 --build-dir=$BOOST_BUILD --prefix=$BOOST_BUILD link=static runtime-link=static
popd
