#!/bin/bash

pushd $GSOAP_HOME

sudo cp ./gsoap/typemap.dat ./bin

WSDLS=""

while read line; do
  WSDLS="$WSDLS $line"
done < ONVIF_wsdls.txt

./bin/wsdl2h -O4 -P -x -o onvif.h "$WSDLS"

./bin/soapcpp2 -2 -I ./gsoap/import onvif.h

popd
