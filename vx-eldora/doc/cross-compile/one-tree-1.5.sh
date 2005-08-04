#!/bin/sh
# Create a single source tree from the various development tools.
# Version 1.5.
# Send comments or questions to devans@cygnus.com.
#
# The script needs a binutils, gcc, and newlib release.
# It looks for them in the current directory.
# Syntax: sh ./one-tree-1.5.sh
#
# The tree is faked, using symlinks.  The goal is to keep it simple while
# providing a working example.
#
# After running this script:
# 1) mkdir build
# 2) cd build
# 3) ../src/configure --target=[your-target] [other options]
# 4) make all info
# 5) make install install-info
#
# If you are going to be building with gcc, do this instead.
#
# 1) mkdir build
# 2) cd build
# 3) CC=gcc ../src/configure --target=[your-target] [other options]
# 4) make all info CC=gcc
# 5) make install install-info
#
# The CC=gcc on line 4 is present for the sake of older releases
# and doesn't hurt to specify it again (normally, setting CC=gcc while
# running configure is enough).
#
# Building in a separate directory assumes of course that you have a version
# of `make' that understands VPATH.  GNU Make is such a critter and
# it is not recommended trying it with other makes.
#
# That's it.

# Find source trees for binutils, gcc, and newlib.
for pkg in binutils gcc newlib
do
    dir=`ls -1d $pkg-[0-9].[0-9].[0-9] | tail -1`
    if [ ! -f "$dir/Makefile.in" ] ; then
	echo "Unable to find a $pkg source tree."
	echo "You need to install a $pkg release in this directory."
	exit 1
    fi

    eval $pkg=$dir
    echo "Using $pkg release $dir."
done

# We only support binutils 2.[6789].*.
# We will probably support newer ones as well but until one has
# tried it one can't actually know.
case $binutils in
binutils-2.[6789] | binutils-2.[6789].*) ;;
*) echo "$binutils is not supported by this script." ; exit 1 ;;
esac

# We only support newlib 1.[78].*.
# We will probably support newer ones as well but until one has
# tried it one can't actually know.
case $newlib in
newlib-1.[78].*) ;;
*) echo "$binutils is not supported by this script." ; exit 1 ;;
esac

# We only support gcc 2.[78].*.
# We will probably support newer ones as well but until one has
# tried it one can't actually know.
case $gcc in
gcc-2.[78].*) ;;
*) echo "$gcc is not supported by this script." ; exit 1 ;;
esac

mkdir src
cd src

# Most of the toplevel stuff we take from binutils
for f in ../$binutils/*
do
    ln -s $f .
done

# Create a symlink to the gcc tree.
ln -s ../$gcc gcc

# Create symlinks to newlib.
ln -s ../$newlib/newlib .
ln -s ../$newlib/libgloss .
ln -s ../$newlib/COPYING.NEWLIB .

# Target library configuration has undergone lots of changes recently
# [post binutils-2.6].  If we're using binutils-2.6, use the configury
# from newlib-1.7 [or greater].

if [ x"$binutils" = binutils-2.6 ] ; then
    for f in configure configure.in config-ml.in Makefile.in
    do
	rm -f $f
	ln -s ../$newlib/$f .
    done
fi

exit 0

