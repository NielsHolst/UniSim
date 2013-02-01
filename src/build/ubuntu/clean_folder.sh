#!/bin/bash

#for folders and files under ../../library/usbase
pushd ../../library/usbase/

if [ -d ./debug ]; then
  rm -Rf ./debug
fi

if [ -d ./output ]; then
  rm -Rf ./output
fi

if [ -d ./release ]; then
  rm -Rf ./release
fi

if [ -d ./temp ]; then
  rm -Rf ./temp
fi

if [ -f makefile*.* ]; then
  rm -Rf makefile*.*
fi

if [ -f ./makefile ]; then
  rm -Rf ./makefile
fi

if [ -f ./object_script*.* ]; then
  rm -Rf ./object_script*.*
fi

if [ -f ./*.user ]; then
  rm -Rf ./*.user
fi
popd

#for folders and files under ../../library/usengine
pushd ../../library/usengine/
if [ -d ./debug ]; then
  rm -Rf ./debug
fi

if [ -d ./output ]; then
  rm -Rf ./output
fi

if [ -d ./release ]; then
  rm -Rf ./release
fi

if [ -d ./temp ]; then
  rm -Rf ./temp
fi

if [ -f ./makefile*.* ]; then
  rm -Rf ./makefile*.*
fi

if [ -f ./makefile ]; then
  rm -Rf ./makefile
fi

if [ -f ./object_script*.* ]; then
  rm -Rf ./object_script*.*
fi

if [ -f ./*.user ]; then
  rm -Rf ./*.user
fi
popd





