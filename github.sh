#!/bin/bash

git init
git add .
git commit -a -m "First commit"
git remote add origin $1
git push -u origin master -f

