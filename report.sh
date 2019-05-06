#/bin/sh

username=$1
initialdate=$2
finaldate=$3

echo $finaldate
echo $username

echo "<all>" > ouputFile.xml

for i in {0..100} 
do

response=$(curl -v GET --insecure -H "X-Redmine-API-Key: 4aa273cce953a2a7768967667a602280ea9dbdc3" "https://vcis-redmine.f4e.europa.eu/projects/resttesting/issues.xml?offset=$i&limit=1&created_on=%3E%3C$initialdate|$finaldate")  

position=$(echo $response | grep -aob '<id>' | grep -oE '[0-9]+')

echo $response
echo position=$position

if [[ -n $position ]] ; then

index=$position+4
token=${response:index:4}
storyid=$(echo $token | grep -oE '[0-9]+')
echo $storyid

info=$(curl -v GET --insecure -H "X-Redmine-API-Key: 4aa273cce953a2a7768967667a602280ea9dbdc3" "https://vcis-redmine.f4e.europa.eu/issues/$storyid.xml?include=journals")
catched=$(echo $info | grep $username)

#if the output contains the username append the result to the output file.
if [[ -n $catched ]] ; then
echo $info >> outputFile.xml
fi

#break if there are no more userstories in the date range
else
break
fi

done


echo "</all>" >> outputFile.xml



