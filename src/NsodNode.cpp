#include "NsodNode.h"

NsodNodeCluster * nsod_node_cluster_new(int width, int height) {
	NsodNodeCluster * cluster = (NsodNodeCluster*)malloc(sizeof(NsodNodeCluster));
	cluster->sample_dimension_width = width;
	cluster->sample_dimension_height = height;

	cluster->node = (NsodNode **) malloc(sizeof(NsodNode*)* width*height);
	return cluster;
}
	
NsodNode * nsod_node_new(int x, int y, 
		                     int iw,int ih, 
		                     int isw,int ish, 
		                     int absolute_position,int connect_dimension, 
												 int * connect, double * strengh) {
	NsodNode * node = (NsodNode*)malloc(sizeof(NsodNode));
  node->x = x;
  node->y = y;
  node->iw = iw;
  node->ih = ih;
  node->isw = isw;
  node->ish = ish;
  node->absolute_position = absolute_position;
	node->connect_dimension = connect_dimension;
	node->connect = connect;
	node->strengh = strengh;

	return node;
}

void nsod_node_print_self_info(NsodNode * node) {
	printf("(%d,%d)\t|%d|\t[%d]\n",node->x,node->y,
			node->absolute_position,node->connect_dimension);
	for (int i=0;i<node->connect_dimension;i++) {
		printf("(%d:%4.2f) ",node->connect[i],node->strengh[i]);
    int len = node->iw * node->isw * node->ish;
		if (i%(len)==len-1) printf("\n");
	}
	printf("\n");
}	

void nsod_cluster_con2txt(NsodNodeCluster * cluster, NsodNetwork * network) {
}	

