if [ $# = 0 ] 
then
	echo Missing version argument!
else
	version=$1
	git archive --format=zip --prefix=unisim-$1/  HEAD > unisim-$1.zip
fi