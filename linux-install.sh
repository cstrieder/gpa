instfolder=~/GPA++
oldinstfolder=~/gpa++
gpa_desktop=gpa++.desktop
desklink=~/Desktop/$gpa_desktop

echo ""

rm -Rf $oldinstfolder
rm -Rf $instfolder
rm -Rf ~/gpa++
rm -f $desklink
rm -f ~/gpa++GUI

mkdir $instfolder
# echo "Copying files to $instfolder..."
cp -R bin $instfolder
cp -R data $instfolder
cp gpa $instfolder/bin/gpa++
chmod +x $instfolder/bin/gpa++
#rm $instfolder/install.sh

# echo "Create link to java in ~/GPA++/bin/java. Change if necessary: ln -s target java"
JAVALOCAL=`which java`
ln -s $JAVALOCAL $instfolder/bin/java

JAVA=$instfolder/bin/java
jarfile=$instfolder/bin/gpa++GUI.jar
icon=$instfolder/bin/gpa++.xpm

echo "[Desktop Entry]" > $gpa_desktop
echo "Comment=" >> $gpa_desktop
echo "Comment[en_US]=" >> $gpa_desktop
echo "Exec=$JAVA -jar '$jarfile'" >> $gpa_desktop
echo "GenericName=" >> $gpa_desktop
echo "GenericName[en_US]=" >> $gpa_desktop
echo "Icon=$icon" >> $gpa_desktop
echo "MimeType=" >> $gpa_desktop
echo "Name=gpa++GUI" >> $gpa_desktop
echo "Name[en_US]=gpa++GUI" >> $gpa_desktop
echo "StartupNotify=false" >> $gpa_desktop
echo "Terminal=false" >> $gpa_desktop
echo "TerminalOptions=" >> $gpa_desktop
echo "Type=Application" >> $gpa_desktop
echo "" >> $gpa_desktop

#mv gpa++.desktop ~/Desktop
mv $gpa_desktop $instfolder

echo "$instfolder/bin/java -jar $instfolder/bin/gpa++GUI.jar" > $instfolder/bin/gpa++GUI
chmod +x $instfolder/bin/gpa++GUI

#ln -s ~/GPA++/bin/gpa++ ~/gpa++

echo "*multiClickTime: 400" > ~/.Xresources
xrdb -merge ~/.Xresources

echo "GPA++ foi instalado em $instfolder/bin/gpa++"
echo "Execute com '~/GPA++/bin/gpa++ file.dat'"
echo ""

