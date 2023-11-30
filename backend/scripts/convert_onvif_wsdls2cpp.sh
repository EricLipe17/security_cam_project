#!/bin/bash

pushd $GSOAP_HOME

sudo cp ./gsoap/typemap.dat ./bin

WSDLS=""

while read line; do
  WSDLS="$WSDLS $line"
done < ONVIF_wsdls.txt

sudo $GSOAP_HOME/bin/wsdl2h -O4 -P -x -o onvif.h $WSDLS

sudo $GSOAP_HOME/bin/soapcpp2 -2 -I $GSOAP_HOME/gsoap/import onvif.h

popd
