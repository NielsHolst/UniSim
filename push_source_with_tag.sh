if [ $# = 0 ] 
then
	echo Missing version argument!
else
	version=$1
	git tag $1
	git push origin master
	git push origin refs/tags/$version
fi
