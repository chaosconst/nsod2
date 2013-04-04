#!/bin/bash
base=$(dirname $0)/../
bin=$base/bin/
src=$base/src/
data=$base/data/
scripts=$base/scripts/

while TRUE; do 
  $scripts/makePicture.sh
  sleep 120;
done;
