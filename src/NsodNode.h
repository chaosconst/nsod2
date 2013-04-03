#ifndef _H_NSOD_NODE 
#define _H_NSOD_NODE

#include<stdio.h> 
#include"NsodNetwork.h"
#include<stdlib.h>

typedef struct CNsodNode
{
	int			x,y;
	int			iw,ih;
	int			isw,ish;
	int			absolute_position;
	int			connect_dimension;

	int *			connect;
	double *		strengh;
} NsodNode;
	NsodNode * nsod_node_new(int x, int y, int iw, int ih, int isw, int ish, int absolute_position,
			         int connect_dimension, int * pos, double * strength);
  void nsod_node_print_self_info(NsodNode * node); 

typedef struct CNsodNodeCluster
{
	int			sample_dimension_width;
	int			sample_dimension_height;

	NsodNode**		node; 
} NsodNodeCluster;
	NsodNodeCluster * 	nsod_node_cluster_new(int width, int height);
  void nsod_cluster_con2txt(NsodNodeCluster * node, NsodNetwork * network); 

#endif
