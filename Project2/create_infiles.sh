#!/bin/bash

echo "I was called with $# parameters"

NUM=$#

#check that the correct args where given
if [ $# != 3 ]
then
	echo "Use:./create_infiles.sh inputFile input_dir numFilesPerDirectory"
	exit 1
fi

#Create the first folder
echo "Creating input_dir directory"
indir=$2
numperfile=$3

if [[ -d "$indir" ]]
then
    echo "$indir exists on your filesystem."
	exit 1
fi

mkdir -p $indir


#error check
if [ $? != 0 ]
then
	echo "[Fail]"
	exit 1
else
	echo "[Success]"
fi

#get the file name
FILE1=$1

#read citizenrecords file
#to create the required subfolders
while IFS= read -r line; do
	set -- $line
	#create a dir for each line you read
	echo "Creating a subfolder $4"
	if [ ! -d ./"$indir"/"$4" ];
	then
		mkdir ./"$indir"/"$4"
		touch ./"$indir"/"$4"/"$4.txt"
	fi
	echo "$line" >> ./"$indir"/"$4"/"$4.txt"
	# #error check
	# if [ $? != 0 ]
	# then
	# 	echo "[Fail]"
	# 	continue
	# fi	
done < $1

# create text files in each folder
# for each folder create files
for dir in "$indir"/*
do
	echo "Sharing records in sub-dir "$dir" "
	for temp in $dir/*; do
		while IFS= read -r line; do
			set -- $line
		done < $temp
	done
	split -d --additional-suffix=.txt -nr/$numperfile ./"$dir"/"$4.txt" ./"$dir"/"$4-"
	rm ./"$dir"/"$4.txt"
	find ./"$dir" -size 0 -delete
done