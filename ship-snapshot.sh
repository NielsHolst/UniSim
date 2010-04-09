git status
echo Did you remember to add and commit latest changes?
#wait for confirmation
if [ $# = 0 ] 
then
	echo Missing version argument!
	echo Write ship-snapshot.sk 1.7
else
	version=$1
	version_underscored=`echo $1 | sed -e 's/\./_/'`
	target_folder=unisim_${version_underscored}
	zip_file=${target_folder}.zip

	# update config.pri with new version before archiving
	
	# Update github
	git tag $version
	git push origin master
	git push --tags

	# Update ecolmod.org 
	git archive --format=zip --prefix=${target_folder}/ HEAD > ${zip_file}
	cd ship/upload
	`sed -e s/unisim_base/${target_folder}/ ftp-upload-base.txt > ftp-upload.txt`
	`sed -e s/unisim_base/${target_folder}/ index_base.html | sed -e s/version_base/$version/ > index.html`
	ftp -s:ftp-upload.txt
fi
