#Outputs the pilot test data in the passed data file as an ampisand separated string for each value of Counter
echo $1
COUNTER=1
while [  $COUNTER -lt 10000000 ]; do
   echo "\n$COUNTER"
   egrep "^($COUNTER & )" $1 | sed "s/$COUNTER & / /" | tr '\n' '&'
   let COUNTER=COUNTER*10 
done
