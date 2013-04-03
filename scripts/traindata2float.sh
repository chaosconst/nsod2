#!/bin/bash
awk 'NR>1&&NR<10000{print $0}' | awk -F , '{

  for (shiftx=0;shiftx<=4;shiftx++) {
  for (shifty=0;shifty<=4;shifty++) {

		for (i=0;i<shiftx;i++){
				for (k=0;k<32;k++){printf "0,"};
		}
	
		for (i=2;i<=NF;i++){
			if ((i-1)%28==1) {
				for (j=0;j<shifty;j++) printf "0,";
			}
			printf $i/255","; 
			if((i-1)%28==0){ 
				for (j=4;j>shifty;j--) printf "0,";
			}
		}
		
		for (i=4;i>shiftx;i--){
			for (k=0;k<32;k++){printf "0,"} 
		}
		printf"\n"

	}}
}' |sed 's///g' | sed 's/,$//g'
