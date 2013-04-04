#include "cmd_flags.h"
#include "NsodNode.h"
#include<stdlib.h>
#include<iostream>
#include<fstream>

#ifndef _H_NSOD_NETWORK
#define _H_NSOD_NETWORK

#define AUTOSAVE_ITERATION 1000

typedef struct 
{
/* numbers of NsodNetwork */
	int			num_of_node;
	int			num_of_cluster;
	int			num_of_my_cluster;
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

        /* for MPI */
	int			myid, pnum;

	/*file process*/
	char			connection_file_name[100];
	FILE*			connection_file;
	std::ifstream * fin;  
	
} NsodNetwork;
/* methods of NsodNetwork */
	NsodNetwork * 		nsod_network_new();
	bool 			nsod_network_main_processor(NsodNetwork * network);
	void 			nsod_network_init_nodes(NsodNetwork * network);
	int 			nsod_network_get_inputs(NsodNetwork * network);
	int 			nsod_network_sent_status_to_shell(NsodNetwork * network);
	int 			nsod_network_main(NsodNetwork * network);
	
	/* what really matters is the following function*/
	int 			nsod_network_compute_cluster_activity_change(NsodNetwork * network, struct CNsodNodeCluster * cluster);
/* NsodNetwork defination end */

#endif
