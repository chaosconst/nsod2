#include "cmd_flags.h"
#include "NsodNode.h"
#include<stdlib.h>
#include<iostream>
#include<fstream>

#ifndef _H_NSOD_SHELL
#define _H_NSOD_SHELL

typedef struct 
{
/* numbers of NsodShell */
	int			num_of_node;
	int			num_of_cluster;
	struct  CNsodNodeCluster **	clusters;
	int 		network_level;
	int *		model_cluster_input_num_width;
	int *		model_cluster_input_num_height;
	int *		model_cluster_num_width; 
	int *		model_cluster_num_height;
	int *		model_cluster_sample_num_width; 
	int *		model_cluster_sample_num_height;

	int 			num_of_basic_inputs;
	int 			iterations;
	int 			total_iterations;
	double *	basic_inputs;
	double *	activity_all_node;
	double *	activity_all_node_change;
	int				length_of_activity_history;
	CmdLineFlags *		flags;
	
  double **	connect_all_node_strengh;
  int **	connect_all_node_pos;
  
  NsodNode **	node;

	/*file process*/
	std::ifstream * fin;  
	
} NsodShell;
/* methods of NsodShell */
	NsodShell * 		nsod_shell_new();
	int 			nsod_shell_init_nodes(NsodShell * shell);
	int 			nsod_shell_print_clusters(NsodShell * shell);
	int 			nsod_shell_fill_image(NsodShell * shell, int node_id, double fire, double * image);
	
/* NsodShell defination end */

#endif

