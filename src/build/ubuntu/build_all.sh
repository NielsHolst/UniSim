#!/bin/bash

if [ -z $BOOST_ROOT ]; then
 echo "BOOST_ROOT is not set as environmental variable"
 sleep 1m
 exit 1
fi

echo "."
echo ". You are about to build universal simulator"
echo ". with Boost library found in ......$BOOST_ROOT"
echo "."
echo "Press Ctrl + C to break Or press Enter to continue"
read -s  

#Copy authors list to Unisim root
bash copy_authors.sh

#Erase all exe and dll targets
bash clean_targets.sh
bash clean_build_desktops.sh

#Make own tools
pushd ../own_tools/write_unisim_root
qmake write_unisim_root.pro
make
popd

pushd ../own_tools/write_clean_ephemerals
qmake write_clean_ephemerals.pro
make
popd

pushd ../own_tools/write_plugins_project
qmake write_plugins_project.pro
make
popd

#Generate batch and project files
../own_tools/write_unisim_root_app
../own_tools/write_clean_ephemerals_app
../own_tools/write_plugins_project_app

#Clean build folders
bash clean_ephemerals.sh  
bash clean_folder.sh #done but not as function

#Build Unisim
pushd ../..
qmake build_all.pro
echo "."
echo "***Finished qmake***"
echo "."

make

popd
echo "."
echo "***Finished make***"
echo "."

bash clean_ephemerals.sh  #or .bat file under win folder?
bash clean_folder.sh #done not as function
echo "."
echo "***Finished build***"
echo "."

bash get_libraries.sh   
echo "."
echo "***Copied Unisim library files***"
echo "."

#Update registry settings
pushd ../own_tools/update_settings
qmake update_settings.pro
make
popd
../own_tools/update_settings_app
echo "."
echo "***Update registry for Unisim***"
echo "."

read -s
exit 1

