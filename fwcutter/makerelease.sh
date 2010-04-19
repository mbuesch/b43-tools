#!/bin/bash
set -e

project="b43-fwcutter"

origin="$(dirname "$0")"
[ "${origin:0:1}" = "/" ] || origin="$PWD/$origin"

do_git_tag=1
[ "$1" = "--notag" ] && do_git_tag=0

version="$(cat $origin/Makefile | grep -e VERSION | head -n1 | cut -d' ' -f3)"
if [ -z "$version" ]; then
	echo "Could not determine version!"
	exit 1
fi
tmpdir="/tmp"
release_name="$project-$version"
tarball="$release_name.tar.bz2"
tagname="$project-$version-release"
tagmsg="$project-$version release"

export GIT_DIR="$origin/../.git"

rm -Rf "$tmpdir/$release_name"
echo "Creating target directory"
mkdir -p "$tmpdir/$release_name"
cd "$tmpdir/$release_name"
echo "git checkout"
git checkout -f
mv fwcutter $release_name
targetdir="$tmpdir/$release_name/$release_name"

rm "$targetdir/makerelease.sh"

echo "creating tarball"
cd "$targetdir/.."
tar cjf "$tarball" "$release_name"
mv "$tarball" "$origin"

echo "running testbuild"
cd "$targetdir"
make

echo "removing testbuild"
rm -R "$tmpdir/$release_name"

if [ "$do_git_tag" -ne 0 ]; then
	echo "Tagging GIT"
	cd "$origin"
	git tag -m "$tagmsg" -a "$tagname"
fi

echo
echo "built release $version"
