#!/bin/bash
set -v off #echo off

for D in ../../applications/*build-desktop*
do 
  rm -Rf $D 
done

for D in ../../applications/unisim/*build-desktop*
do 
  rm -Rf $D 
done

for D in ../../library/*build-desktop*
do 
  rm -Rf $D 
done

for D in ../../plugins/*build-desktop*
do 
  rm -Rf $D 
done

for D in ../../own_tools/*build-desktop*
do 
  rm -Rf $D 
done



