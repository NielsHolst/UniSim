git status
echo Did you remember to add and commit latest changes?
#wait for confirmation
if [ $# = 0 ] 
then
	echo Missing version argument!
	echo Write ship-snapshot.sh n.n
else
	version=$1
	version_underscored=`echo $1 | sed -e 's/\./_/'`

	# update config.pri with new version before archiving
	
	# Update github
	git tag $version
	git push origin master
	git push origin refs/tags/$version

	# Update ecolmod.org 
	#`sed -e s/unisim_base/${target_folder}/ index_base.html | sed -e s/version_base/$version/ > index.html`
fi
