if [[ -s output ]] ; then
echo ciao
else
echo nonciao
fi

bad=$(echo ciao)
echo $bad


response=$(curl -v GET --insecure -H "X-Redmine-API-Key: 4aa273cce953a2a7768967667a602280ea9dbdc3" "https://vcis-redmine.f4e.europa.eu/projects/resttesting/issues.xml?offset=$i&limit=1&created_on=%3E%3C2012-09-03|2015-09-03") 

position=$(echo $response | grep -aob '<id>' | grep -oE '[0-9]+')

echo $response
echo $position

i1=$position+4
i2=$position+6
id=${response:i1:4}

res=$(echo $id | grep -oE '[0-9]+')

echo $res

