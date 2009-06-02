#!/bin/bash
set -e

project="b43-fwcutter"


origin="$(pwd)"
version="$(cat $origin/Makefile | grep -e VERSION | head -n1 | cut -d' ' -f3)"
if [ -z "$version" ]; then
	echo "Could not determine version!"
	exit 1
fi
release_name="$project-$version"
tarball="$release_name.tar.bz2"

export GIT_DIR="$origin/../.git"

cd /tmp/
rm -Rf "$release_name"
echo "Creating target directory"
mkdir "$release_name"
cd "$release_name"
echo "git checkout"
git checkout -f
mv fwcutter $release_name
cd $release_name

rm makerelease.sh

echo "creating tarball"
cd ..
tar cjf "$tarball" "$release_name"
mv "$tarball" "$origin"

echo "running testbuild"
cd "$release_name"
make

echo "removing testbuild"
cd ../..
rm -R "$release_name"

echo
echo "built release $version"
