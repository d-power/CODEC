#!/bin/sh

travFolder()
{
#    echo "travFolder"
    flist=`ls $1`
    cd $1
    #echo $flist
    for f in $flist
    do
        if test -d $f
        then
            travFolder $f
        else
#            echo "file:$f"
#            sed -i 'r/\s//' $f
            dos2unix $f
        fi
    done
    cd ../ 
}

dir=$(pwd)
travFolder $dir
