#!/bin/bash
set -v off #echo off

if [ -f ../../applications/*.exe ]; then 
rm -f ../../applications/*.exe 2> /dev/null
fi

if [ -f ../../applications/engine*.dll ]; then 
rm -f ../../applications/engine*.dll 2> /dev/null
fi

if [ -f ../../applications/base*.dll ]; then 
rm -f ../../applications/base*.dll 2> /dev/null
fi

if [ -f ../../library/*.dll ]; then 
rm -f ../../library*.dll 2> /dev/null
fi

if [ -f ../../library/*.a ]; then 
rm -f ../../library*.a 2> /dev/null
fi

if [ -f ../../plugins/*.dll ]; then 
rm -f ../../plugins*.dll 2> /dev/null
fi

if [ -f ../../plugins/*.a ]; then 
rm -f ../../plugins*.a 2> /dev/null
fi

if [ -f ../own_tools/*.exe ]; then 
rm -f ../own_tools/*.exe 2> /dev/null
fi

