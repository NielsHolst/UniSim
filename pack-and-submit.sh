if [ $# = 0 ] 
then
	echo Missing version argument!
else
	version=$1
	version_underscored=$(echo $1 | sed -e 's/\./_/')
	target_folder=unisim_${version_underscored}
	zip_file=${target_folder}.zip
	#echo $target_folder
	#echo $zip_file
	git tag $version
	#git push origin master
	#git push tags
	#git archive --format=zip --prefix=${target_folder}/ HEAD > ${zip_file}
fi
