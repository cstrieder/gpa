#!/bin/sh -e

test -n "$srcdir" || srcdir=`dirname "$0"`
test -n "$srcdir" || srcdir=.
echo "Cristiano Strieder cstrieder@gmail.com" > AUTHORS
echo "Reinaldo R. Rosa reinaldo@lac.inpe.br" >> AUTHORS
echo `date` > README
echo "This is the first release available at svn" > ChangeLog
touch NEWS
autoreconf --force --install --verbose "$srcdir"
test -n "$NOCONFIGURE" || "$srcdir/configure" "$@"
