if [ $# = 0 ] 
then
	echo Missing version argument!
else
	version=$1
	git push origin :$version
fi
