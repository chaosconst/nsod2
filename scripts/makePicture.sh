#!/bin/bash
base=$(dirname $0)/../
bin=$base/bin/
src=$base/src/
data=$base/data/
html=$base/html/

autosave_dir=$1

if [[ $autosave_dir == "" ]]; then
  autosave_dir=$data/con/autosave/
fi

mkdir -p $autosave_dir 
cp -r $autosave_dir/img $autosave_dir/img_$(date +%s) 
mkdir -p $autosave_dir/img 
mkdir -p $autosave_dir/txt

filenum=$(ls $autosave_dir/con.*| wc -l)
set -x 
$bin/NsodShell --model_file_prefix $autosave_dir/con. --model_file_num $filenum --output_file_prefix $autosave_dir/txt/s_
set +x 

echo "start txt -> png convert"
filenum=$(ls $autosave_dir/txt/s_*| wc -l)
for ((i=0;i<$filenum;i++)) {
  convert $autosave_dir/txt/s_$i.txt $autosave_dir/img/s_$i.png  
}

html_root=/Applications/MAMP/htdocs/nsod_shell/
html_url_pre="http://127.0.0.1/nsod_shell/img/s_"

cp -r $autosave_dir/img $html_root/

rm $html/body
for ((i=0;i<$filenum;i++)) {
  echo "            <a href=\"$html_url_pre$i.png\">" >> $html/body
  echo "                <img data-title=\"S$i\" " >> $html/body
  echo "                     src=\"$html_url_pre$i.png\">" >> $html/body
  echo "            </a>" >> $html/body
}

cat $html/index_head.html > $html/index.html
cat $html/body >> $html/index.html
cat $html/index_foot.html >> $html/index.html

cp -r $html/index.html $html_root/
