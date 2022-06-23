#!/bin/sh
#cp ../doc/user_fr/tutorial*.md ../code/build/executables/tutorial*.yml .
#echo "# Tutorials\n\n" > tutorials.md
#cat ../doc/user_fr/tutorial_01.md >> tutorials.md
#cat ../doc/user_fr/tutorial_02.md >> tutorials.md
#cat ../doc/user_fr/tutorial_03.md >> tutorials.md
#cat ../doc/user_fr/tutorial_06.md >> tutorials.md
#
#./report.sh -p tutorials.md -r tutorials.html
#rm tutorial_*.md tutorial*.yml
#tar -czvf tutorials.tar.gz *.png *.svg tutorials.html
#rm -rf *.png *.svg tutorials.html

./report.sh -p rapport_validation.md -r rapport_validation.html
tar -czvf rapport_validation.tar.gz *.png *.svg rapport_validation.html
#rm -rf *.png *.svg rapport_validation.html
