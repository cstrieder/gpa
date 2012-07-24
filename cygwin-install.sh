instfolder=~/GPA++

echo ""

mkdir $instfolder
cp -R bin $instfolder
cp -R data $instfolder
cp gpa $instfolder/bin/gpa++
chmod +x $instfolder/bin/gpa++

cc -O2 -o 3angle triangle/triangle.c -lm
# cc -O2 -I/usr/local/include -o xowme triangle/showme.c -L/usr/X11R6/lib -lX11
cp 3angle $instfolder/bin/
chmod +x $instfolder/bin/3angle
# cp xowme $instfolder/bin/
# chmod +x $instfolder/bin/xowme
rm $instfolder/bin/3angle

echo "GPA++ foi instalado em $instfolder/bin/gpa++"
echo "Execute com '~/GPA++/bin/gpa++ ~/GPA++/data/paper/3a.dat'"
echo ""

