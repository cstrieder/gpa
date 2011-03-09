#!/bin/sh -e

test -n "$srcdir" || srcdir=`dirname "$0"`
test -n "$srcdir" || srcdir=.
echo "Cristiano Strieder cstrieder@gmail.com" > AUTHORS
echo "Reinaldo R. Rosa reinaldo@lac.inpe.br" >> AUTHORS
echo `date` > NEWS
echo "To install:" > README
echo "" >> README
echo "./configure" >> README
echo "make" >> README
echo "./gpa-install.sh" >> README
echo "" >> README
echo "To make a new release:" >> README
echo "" >> README
echo "./clean.sh" >> README
echo "./autogen.sh" >> README
echo "This is the first release available at svn" > ChangeLog
autoreconf --force --install --verbose "$srcdir"
test -n "$NOCONFIGURE" || "$srcdir/configure" "$@"
