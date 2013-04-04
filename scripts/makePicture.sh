#!/bin/bash
base=$(dirname $0)/../
bin=$base/bin/
src=$base/src/
data=$base/data/

autosave_dir=$data/con/autosave/

mkdir -p $autosave_dir 
cp -r $autosave_dir/img $autosave_dir/img_$(date +%s) 
mkdir -p $autosave_dir/img 
mkdir -p $autosave_dir/txt

filenum=$(ls $data/con/autosave/con.*| wc -l)
$bin/NsodShell --model_file_prefix $autosave_dir/con. --model_file_num $filenum --output_file_prefix $autosave_dir/txt/s_

echo "start txt -> png convert"
filenum=$(ls $autosave_dir/txt/s_*| wc -l)
for ((i=0;i<$filenum;i++)) {
  convert $autosave_dir/txt/s_$i.txt $autosave_dir/img/s_$i.png
}

cp -r $autosave_dir/img /Applications/MAMP/htdocs/nsod_shell/ 
