#/bin/sh


username=$1
initialdate=$2
finaldate=$3



git log --after=$initialdate --before=$finaldate --author=$username > gitOutput

sed s/\'/"\\\'"/g gitOutput > tempOut
sed s/'\"'/'\\\"'/g tempOut > gitOutput
sed ':a;N;$!ba;s/\n/<br>/g' gitOutput > tempOut
sed '$s/$/\")/g' tempOut > gitOutput
sed s/commit/'<\/tr><\/td><tr><td>commit'/g gitOutput > tempOut
mv tempOut gitOutput


echo -e "
<html>
<title>GIT REPORT $username </title>
<head>
<script src=\"string.js\"></script>
<script src=\"PrintTable.js\"></script>
<meta content=\"text/html;charset=utf-8\" http-equiv=\"Content-Type\">
<meta content=\"utf-8\" http-equiv=\"encoding\">

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


</style>
</head>
<body>
<h1>GIT commits done by Giuseppe Ferrò from $initialdate to $finaldate</h1>
<script>

document.write(\"<table><tr><th>GIT COMMITS</th></tr>\");
document.write(\"<tr><td>\");
document.write(\"\\" > report_git.html

cat gitOutput >> report_git.html

echo "
document.write(\"</tr></td>\");

document.write(\"</table>\");

</script> 

</body>
</html>" >> report_git.html

