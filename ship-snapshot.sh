if [ $# = 0 ] 
then
	echo Missing version argument!
else
	version=$1
	version_underscored=`echo $1 | sed -e 's/\./_/'`
	target_folder=unisim_${version_underscored}
	zip_file=${target_folder}.zip

	# Update github
	#git tag $version
	#git push origin master
	#git push --tags

	# Update ecolmod.org 
	#git archive --format=zip --prefix=${target_folder}/ HEAD > ${zip_file}
	cd ship/upload
	`sed -e s/unisim_base/${target_folder}/ ftp-upload-base.txt > ftp-upload.txt`
	`sed -e s/unisim_base/${target_folder}/ index_base.html | sed -e s/version_base/$version/ > index.html`
	ftp -s:ftp-upload.txt
fi
