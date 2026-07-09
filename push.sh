#!/data/data/com.termux/files/usr/bin/bash

git add .

if [ -n "$1" ]; then
    git commit -m "$1"
else
    git commit -m "Update"
fi

git push
