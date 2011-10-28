#!/bin/bash
OS=`uname`
if [ $OS == 'Darwin' ]; then
	echo "Mac OS detected."
	echo "Proceed? [y]"
	read CONFIRM
	if [ $CONFIRM == 'y' ]; then
		sh mac-install.sh
	else
		echo "Installation aborted."
	fi
elif [ $OS == 'Linux' ]; then
	echo "Linux OS detected."
	echo "Proceed? [y]"
        read CONFIRM
        if [ $CONFIRM == 'y' ]; then
                sh linux-install.sh
        else
                echo "Installation aborted."
        fi
else
	echo "Nor a Linux or Mac OS detected. Please contact cstrieder@gmail.com and inform your OS."
fi
