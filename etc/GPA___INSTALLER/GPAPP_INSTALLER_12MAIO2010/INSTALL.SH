instfolder=~/GPA++
oldinstfolder=~/gpa++
desklink=~/Desktop/gpa++.desktop

echo ""

rm -Rf $oldinstfolder
rm -Rf $instfolder
rm -Rf ~/gpa++
rm $desklink


mkdir ~/GPA++
echo "Copying files to $instfolder..."
cp -R * ~/GPA++
chmod +x ~/GPA++/bin/gpa++
rm ~/GPA++/install.sh

echo "Create link to java in ~/GPA++/bin/java. Change if necessary: ln -s target java"
JAVALOCAL=`which java`
ln -s $JAVALOCAL ~/GPA++/bin/java

JAVA=~/GPA++/bin/java
jarfile=$instfolder/bin/gpa++GUI.jar
icon=$instfolder/bin/gpa++.xpm


echo "[Desktop Entry]" > gpa++.desktop
echo "Comment=" >> gpa++.desktop
echo "Comment[en_US]=" >> gpa++.desktop
echo "Exec=$JAVA -jar '$jarfile'" >> gpa++.desktop
echo "GenericName=" >> gpa++.desktop
echo "GenericName[en_US]=" >> gpa++.desktop
echo "Icon=$icon" >> gpa++.desktop
echo "MimeType=" >> gpa++.desktop
echo "Name=gpa++GUI" >> gpa++.desktop
echo "Name[en_US]=gpa++GUI" >> gpa++.desktop
echo "StartupNotify=false" >> gpa++.desktop
echo "Terminal=false" >> gpa++.desktop
echo "TerminalOptions=" >> gpa++.desktop
echo "Type=Application" >> gpa++.desktop
echo "" >> gpa++.desktop

mv gpa++.desktop ~/Desktop

echo "~/GPA++/bin/java -jar ~/GPA++/bin/gpa++GUI.jar" > ~/gpa++GUI
chmod +x ~/gpa++GUI

ln -s ~/GPA++/bin/gpa++ ~/gpa++

echo "*multiClickTime: 400" >> ~/.Xresources
xrdb -merge ~/.Xresources

echo "All done. Try '~/gpa++ ~/GPA++/data/paper/4c.dat' or '~/gpa++GUI' for the GUI version."
echo "Verify that gnuplot gv and java exist. These are the required packages."
echo "May be sudo apt-get update, apt-get install them is necessary."
echo ""

