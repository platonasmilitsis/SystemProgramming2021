#!/bin/bash
echo "I was called with $# parameters"

NUM=$#

#check that the correct args where given
if [ $# != 4 ]
then
	echo "Use: ./testFile.sh virusesFile countriesFile numLines duplicatesAllowed"
	exit 1
fi

FILE1=$1
FILE2=$2

while IFS= read -r line; do

	#error check
	if [ $? != 0 ]
	then
		echo "[Fail]"
		continue
	fi
done < $1

while IFS= read -r line; do

	#error check
	if [ $? != 0 ]
	then
		echo "[Fail]"
		continue
	fi

done < $2

touch citizenRecordsFile.txt
ID=0
#add records to the file	
for (( j=0; j < $3; ++j ))
	do

            if [ $4 == 0 ]
            then
                ID=$(($ID+1))
            else
                ID=$((( RANDOM % 9999 )+1))
            fi
            #get the name and the last-name	
            #name length
            LEN=$(( (RANDOM % 5) + 3 ))
            NEW_NAME=$(shuf -zer -n $LEN  {A..Z} {a..z} | tr -d '\0');

                
            #last name length
            LEN=$(( (RANDOM % 5) + 3 ))
            NEW_LAST=$(shuf -zer -n $LEN  {A..Z} {a..z} | tr -d '\0');


            #get a random age
            AGE=$(((RANDOM % 120)+1))

            #get the disease
            DIS=$(shuf -n 1 $1) 
            COUNTRY=$(shuf -n 1 $2)
            VAC=$(( (RANDOM % 2) ))
            if [ $VAC == 1 ]
            then
                VAC="NO"
                DAY=""
                MON=""
                YR=""
            else
                VAC="YES"

                #max day is 30
                DAY=$(( (RANDOM % 30) + 1 ))
                #max mon is 12
                MON=$(( (RANDOM % 12) + 1 ))
                #years between 1995 and 2020
                YR=$(( (RANDOM % 25) + 1995 ))


                if [ $DAY -lt 10 ]
		        then
                    #echo "$DAY"
                    #echo "CHANGE"
                    DAY=$(echo "0"$DAY"")
                    #echo "$DAY"	
		        fi

            #fix months
            if [ $MON -lt 10 ]
            then
                #echo "$MON"
                #echo "CHANGE"
                MON=$(echo "0"$MON"")
                #echo "$MON"	
		    fi
            fi
           
        if [ $VAC == "YES" ]
        then
            echo ""$ID"  "$NEW_NAME" "$NEW_LAST" "$COUNTRY" "$AGE" "$DIS" "$VAC" "$DAY"-"$MON"-"$YR"" >> citizenRecordsFile.txt
        else
            echo ""$ID"  "$NEW_NAME" "$NEW_LAST" "$COUNTRY" "$AGE" "$DIS" "$VAC""  >> citizenRecordsFile.txt
        fi
done