#!/bin/bash

#file for bashrc  ~/.bashrc
uDDS_path=$(pwd)
uDDSGen_path=$(pwd)/uDDSGen
output=~/.bashrc
echo $uDDS_path
echo $uDDSGen_path
echo $output

#write in bashrc
export_path="export PATH="$uDDSGen_path"/:$""PATH"
export_home="export uDDSHOME="$uDDS_path

echo $export_path 
echo $export_home

#export_path=${export_path#*application}
#echo $export_path | sed 's:\/:\\\/:g'
#string_path=$(echo $export_path | sed 's#\/#\\\/#g')
#echo $string_path

#export_home=${export_home#*application}
#echo $export_home | sed 's:\/:\\\/:g'
#string_home=$(echo $export_home | sed 's#\/#\\\/#g')
#echo $string_home

delete_path="sed -i '/\/uDDS/d' $output"
#echo $delete_path
echo ${delete_path}|awk '{run=$0;system(run)}'

#delete_home="sed -i '/$string_home/d' $output"
delete_home="sed -i '/export uDDSHOME/d' $output"
#echo $delete_home
echo ${delete_home}|awk '{run=$0;system(run)}'

#echo $export_path >> $output
echo $export_home >> $output

#cat $output
cat $output






