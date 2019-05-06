#/bin/sh

username=$1
initialdate=$2
finaldate=$3
key=4aa273cce953a2a7768967667a602280ea9dbdc3

echo $finaldate
echo $username

rm outputFile.xml
rm outputFile
echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?><all>" > outputFile.xml

for i in {0..10000} 
do

response=$(curl -v GET --insecure -H "X-Redmine-API-Key: $key" "https://vcis-redmine.f4e.europa.eu/projects/marte2/issues.xml?offset=$i&limit=1&created_on=%3E%3D2011-01-01&status_id=*")  

position=$(echo $response | grep -aob '<id>' | grep -oE '[0-9]+')

echo $response
echo position=$position

if [[ -n $position ]] ; then

index=$position+4
token=${response:index:4}
storyid=$(echo $token | grep -oE '[0-9]+')
echo $storyid

info=$(curl -v GET --insecure -H "X-Redmine-API-Key: $key" "https://vcis-redmine.f4e.europa.eu/issues/$storyid.xml?include=journals")
catched=$(echo $info | grep $username)

#if the output contains the username append the result to the output file.
if [[ -n $catched ]] ; then
echo $info >> outputFile
fi

#break if there are no more userstories in the date range
else
break
fi

done

sed s/"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"//g outputFile >> outputFile.xml


echo "</all>" >> outputFile.xml

echo "
<html>
<title>REDMINE REPORT $username </title>
<head>
<script src=\"string.js\"></script>
<script src=\"PrintTable.js\"></script>
<style>

table {
  border-collapse: separate;
  border-spacing: 0;
}
th,
td {
  padding: 10px 15px;
}
thead {
  background: #395870;
  color: #fff;
}
tbody tr:nth-child(even) {
  background: #f0f0f2;
}
td {
  border-bottom: 1px solid #cecfd5;
  border-right: 1px solid #cecfd5;
}
td:first-child {
  border-left: 1px solid #cecfd5;
}

}
</style>
</head>
<body>
<h1>REDMINE REPORT $username</h1>
<script>
PrintTable(\"$username\", \"$initialdate\",\"$finaldate\");
</script> 

</body>
</html>" > report_redmine.html


rm outputFile




