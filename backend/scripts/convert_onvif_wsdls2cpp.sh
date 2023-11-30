#!/bin/bash

pushd $GSOAP_HOME

sudo cp ./gsoap/typemap.dat ./bin

WSDLS=""

while read line; do
  WSDLS="$WSDLS $line"
done < ONVIF_wsdls.txt

$GSOAP_HOME/bin/wsdl2h -O4 -P -x -o onvif.h $WSDLS

$GSOAP_HOME/bin/soapcpp2 -2 -C -I $GSOAP_HOME/gsoap/import -I $GSOAP_HOME/gsoap -j -x onvif.h

$GSOAP_HOME/bin/soapcpp2 -a -x -L -pwsdd -I $GSOAP_HOME/gsoap/import $GSOAP_HOME/gsoap/import/wsdd5.h

popd
