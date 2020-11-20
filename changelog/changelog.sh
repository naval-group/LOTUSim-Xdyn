#!/bin/sh
echo "# $2 (`git log -1 --date=short --format=%ad $2`)"
echo ""
git log --pretty=oneline $1^..$2 | grep "Merge" | sed  "s/^\(.*\)branch '//g" | sed "s/'\(.*\)'//g" | sed "s/\([0-9]\+\)-\(.*\)/#\1 \2/g" | sed "s/-/ /g" | sed 's/\(#[0-9]\+\) \([a-z]\)/- \1 \u\2/g' | grep "^-"
echo ""
echo ""
