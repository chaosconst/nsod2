/* Nsod2 - Network Sea Of Dream With Sparsity and Learning Ability 
 * Copyright (C) 2013 ChaosConst
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
   An example running of this program:

   mpiexec -n 2 ./nosd \
   --study_speed 0.1 \
   --input_size 32x32 \
   --input_sample_domain_size 1 \
   --model_size 4x4,4x4,2x2 \
   --sample_size 8x8,10x10,8x8 \
   --model_file_prefix autosave.con. \
   --input_file train.32x32 \
   --total_iterations 10000 
   */

//#define _DEBUG
//#define _SINGLE_THREAD

#ifndef _SINGLE_THREAD
#include "mpi.h"
#endif

#include "NsodNetwork.h"
#include "NsodCommon.h"
#include "NsodNode.h"
#include "cmd_flags.h"

#include <iostream>

#include <algorithm>
#include <fstream>
#include <set>
#include <vector>
#include <sstream>
#include <string>
#include <math.h>

using namespace std;

NsodNetwork * nsod_network_new(int argc,char * argv[])
{
  NsodNetwork * network;
  network=(NsodNetwork *)malloc(sizeof(NsodNetwork));
  network->num_of_cluster=0;

#ifndef _SINGLE_THREAD
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &(network->myid));
  MPI_Comm_size(MPI_COMM_WORLD, &(network->pnum));
#else
  network->myid=1;
  network->pnum=2;
#endif


  if (nsod_network_main_processor(network)) {
    /*Welcome*/
    printf("Network Sea of Dream: Network Processor v2.0 (C) 2006-2013 ChaosConst\n");
  }

  network->flags = new CmdLineFlags();
  network->flags->ParseCmdFlags(argc, argv);
  if (nsod_network_main_processor(network)) 
  {
    if (!network->flags->CheckValidity()) {
      return NULL;
    }
  }

  sprintf(network->connection_file_name,"%s_%d",network->flags->model_file_prefix_.c_str(),network->myid);

  /*initilize network and nodes*/
  network->num_of_basic_inputs = 0;	
  network->num_of_cluster = 0;	
  network->num_of_my_cluster = 0;	
  network->num_of_node = 0;	
  network->basic_inputs = NULL;
  network->total_iterations = network->flags->total_iterations_;

  network->fin = new ifstream(network->flags->input_file.c_str());

  return network;
}

int nsod_network_get_inputs(NsodNetwork * network)
{
  string line;
  if (getline(*(network->fin),line))
  {
    istringstream iss(line);
    int i=0;
    string sub;
    while (getline(iss,sub,',')){
      network->activity_all_node_change[i++]=atof(sub.c_str());
    };
    return 1;
  }
  else return 0;
}

//engrSom functions
//TODO:move to common.cpp
double getDistance(double * weight, double * input_vector, int dimension)
{
  double sum=0;
  for (int i=0;i<dimension;i++)
  {
    sum+=(weight[i]-input_vector[i])*(weight[i]-input_vector[i]);
    //sum+=(weight[i]-0.5)*(input_vector[i]-0.5)*2;
  }
  return sum;
}

double theta(double distance)
{
  if (distance<0.0000001) return 2;
  else return 1/distance;
}

int nsod_network_compute_cluster_activity_change(NsodNetwork * network, NsodNodeCluster * cluster)
{
  //==============================================
  //version1: engrSom
  //==============================================
  int input_dimension = cluster->node[0]->connect_dimension;
  double input_vector[input_dimension];
  int sample_dimension = cluster->sample_dimension_width * cluster->sample_dimension_height;

  /*step0 build input vector*/
  for (int i=0;i<input_dimension;i++) {
    input_vector[i] = network->activity_all_node[cluster->node[0]->connect[i]];
  }

  /*step1 find the BFU(Best Fired Unit)*/
  int BFU=0;
  double min_distance=10000;
  for (int i=0;i<sample_dimension;i++)	
  {
    if (min_distance>getDistance(cluster->node[i]->strengh,input_vector,input_dimension))
    {
      min_distance=getDistance(cluster->node[i]->strengh,input_vector,input_dimension);
      BFU=i;
    }
  }

  /*step2 change weight*/
  for (int i=0;i<sample_dimension;i++)	
  {
    double distances=theta(pow(cluster->node[i]->x-cluster->node[BFU]->x,2)+pow(cluster->node[i]->y-cluster->node[BFU]->y,2));
    network->activity_all_node_change[cluster->node[i]->absolute_position]=distances/2;

    for (int j=0;j<input_dimension;j++)
    {
      cluster->node[i]->strengh[j]=cluster->node[i]->strengh[j]+distances*network->flags->study_speed*(input_vector[j]-cluster->node[i]->strengh[j]);
    }
  }

  return 0; 
}

int nsod_network_sent_status_to_shell(NsodNetwork * network)
{

  int i=network->num_of_basic_inputs;
  int level_start=1;

  i=network->num_of_node-144;
  level_start=3;

#ifdef _DEBUG
  i=0;
  level_start=0;
#endif

  cout<<"---------------------------iteration "<<network->iterations<<"-------------------------------------"<<endl;

  for (int level=level_start;level<network->network_level;level++) {
    cout<<"-----------------------------level "<<level<<"---------------------------------------"<<endl;
    for (int y=0;y<network->model_cluster_num_height[level];y++) {
      for (int x=0;x<network->model_cluster_num_width[level];x++) {
        for (int sy=0;sy<network->model_cluster_sample_num_height[level];sy++) {
          for (int sx=0;sx<network->model_cluster_sample_num_width[level];sx++) {
            if (network->activity_all_node[i]>0.5) printf("\033[31;49;1m"); 
            printf("%4.2f ",network->activity_all_node[i]);
            if (network->activity_all_node[i]>0.5) printf("\033[31;49;0m");
            i++;	
          }
          cout<<endl;
        }
        cout<<endl;
      }
      cout<<endl;
    }
    cout<<endl;
  }

  return 0;
}

void say(NsodNetwork * network, string debug_info) {
  if (nsod_network_main_processor(network)) cout<<debug_info<<endl;
}

int nsod_network_init_nodes(NsodNetwork * network)
{
  // if no model file spesified, create a new model
  if (network->flags->model_file_prefix_.empty()) {

    say(network,string("No Specific Model File, We are going to build a new model.\n"));

    //parse variablies from input
    network->network_level = network->flags->num_level;
    int network_level = network -> network_level;

    //allocate
    network->model_cluster_num_width = (int*) malloc(sizeof(int)*network_level) ;
    network->model_cluster_num_height = (int*) malloc(sizeof(int)*network_level) ;

    network->model_cluster_input_num_width = nsod_common_get_int_array_from_string(network->flags->input_width,network_level);
    network->model_cluster_input_num_height = nsod_common_get_int_array_from_string(network->flags->input_height,network_level);
    network->model_cluster_sample_num_width = nsod_common_get_int_array_from_string(network->flags->sample_width,network_level);
    network->model_cluster_sample_num_height = nsod_common_get_int_array_from_string(network->flags->sample_height,network_level);

    //count node numbers
    for (int i=0; i<network_level; i++) {
      network->model_cluster_num_width[i] = i==0 ? network->model_cluster_input_num_width[i]: network->model_cluster_num_width[i-1] / network->model_cluster_input_num_width[i];
      network->model_cluster_num_height[i] = i==0 ? network->model_cluster_input_num_height[i] : network->model_cluster_num_width[i-1] / network->model_cluster_input_num_height[i];

      network->num_of_cluster += network->model_cluster_num_width[i] * network->model_cluster_num_height[i];
      cout<< "#level "<<i<<" cluster = " << network->model_cluster_num_width[i] * network->model_cluster_num_height[i] << endl;

      network->num_of_node += network->model_cluster_num_width[i] * network->model_cluster_num_height[i] 
        * network->model_cluster_sample_num_width[i] * network->model_cluster_sample_num_height[i];
      cout<< "#level "<<i<<" node = " << network->model_cluster_num_width[i] * network->model_cluster_num_height[i] 
        * network->model_cluster_sample_num_width[i] * network->model_cluster_sample_num_height[i] << endl;
    }

    cout<< "#sum cluster = " << network -> num_of_cluster << endl;
    cout<< "#sum node = " << network -> num_of_node << endl;


    network->clusters= (NsodNodeCluster **) malloc(sizeof(NsodNodeCluster *)*(network->num_of_cluster/(network->pnum-1))+1);

    network -> num_of_basic_inputs = network->model_cluster_num_width[0] * network->model_cluster_num_height[0] 
      * network->model_cluster_sample_num_width[0] * network->model_cluster_sample_num_height[0]; 

    //init_activities
    network -> activity_all_node = (double*) malloc(network->num_of_node * sizeof(double)); 
    network -> activity_all_node_change = (double*) malloc(network->num_of_node * sizeof(double));
    for (int i=0;i<network->num_of_node;i++) {
      network->activity_all_node[i]=0;
      network->activity_all_node_change[i]=0;
    }

    //init connection
    int i=network->num_of_basic_inputs;
    int absolute_postion_level=i;
    for (int level=1;level<network_level;level++) {

      for (int y=0;y<network->model_cluster_num_height[level];y++) {
        for (int x=0;x<network->model_cluster_num_width[level];x++) {

          //if this processor should handle this cluster
          if ((i%(network->pnum-1))+1 == network->myid) {

#ifdef _DEBUG
            cout<<network->myid<<"|"<<i<<endl;
#endif
            network->clusters[network->num_of_my_cluster]=nsod_node_cluster_new(
                network->model_cluster_sample_num_width[level],
                network->model_cluster_sample_num_height[level]);

            NsodNodeCluster *cluster_iter = network->clusters[network->num_of_my_cluster];


            int * connected_node_pos_tmp = (int*)malloc(sizeof(int)
                *network->model_cluster_input_num_height[level]
                *network->model_cluster_input_num_width[level]
                *network->model_cluster_sample_num_height[level-1]
                *network->model_cluster_sample_num_width[level-1]);

            int absolute_postion_last_level = absolute_postion_level - network->model_cluster_num_width[level-1]*network->model_cluster_num_height[level-1]
              *network->model_cluster_sample_num_width[level-1]
              *network->model_cluster_sample_num_height[level-1];

            int connect_dimension = network->model_cluster_input_num_height[level] * network->model_cluster_input_num_width[level]
              * network->model_cluster_sample_num_width[level-1] * network->model_cluster_sample_num_height[level-1];
            int sample_dimension = network->model_cluster_sample_num_width[level-1] * network->model_cluster_sample_num_height[level-1];

            int index_node_pos=0;
            for (int iy=0;iy<network->model_cluster_input_num_height[level];iy++) {
              for (int ix=0;ix<network->model_cluster_input_num_width[level];ix++) {
                int start_pos	= absolute_postion_last_level 
                  + (y * network->model_cluster_num_width[level] * network->model_cluster_input_num_width[level] * network->model_cluster_input_num_height[level]
                      + x * network->model_cluster_input_num_width[level]
                      + iy * network->model_cluster_input_num_width[level] * network->model_cluster_num_width[level]
                      + ix)*sample_dimension;
                for (int sample_idx=0;sample_idx<sample_dimension;sample_idx++) {
                  connected_node_pos_tmp[index_node_pos]=start_pos+sample_idx;
                  index_node_pos++;
                }

              }
            }

            int position_cluster=0;
            for (int sx=0;sx<network->model_cluster_sample_num_width[level];sx++) {
              for (int sy=0;sy<network->model_cluster_sample_num_height[level];sy++) {
                //read weight vector
                double * connection_strengh = (double*)malloc(sizeof(double)*connect_dimension);
                for (int con_idx=0;con_idx<connect_dimension;con_idx++)
                {
                  connection_strengh[con_idx]=(double)(rand()%10000)/(double)10000;
                }

                cluster_iter->node[position_cluster]=nsod_node_new(sx,sy,
                    network->model_cluster_input_num_width[level],
                    network->model_cluster_input_num_height[level],
                    network->model_cluster_sample_num_width[level-1],
                    network->model_cluster_sample_num_height[level-1], 
                    absolute_postion_level
                    +y*network->model_cluster_num_width[level]*network->model_cluster_sample_num_width[level]*network->model_cluster_sample_num_height[level]
                    +x*network->model_cluster_sample_num_width[level]*network->model_cluster_sample_num_height[level]+position_cluster,
                    connect_dimension,
                    connected_node_pos_tmp,connection_strengh);

#ifdef _DEBUG
                nsod_node_print_self_info(cluster_iter->node[position_cluster]);
#endif
                position_cluster++;

              }
            }

            network->num_of_my_cluster++;
          }
          i++;
        }
      }
      absolute_postion_level += network->model_cluster_num_width[level] * network->model_cluster_num_height[level] 
        * network->model_cluster_sample_num_width[level] * network->model_cluster_sample_num_height[level];

    }
  } else {
    //load model file from disk

    char s[10];
    sprintf(s,"%d",network->myid);
    string filename = network->flags->model_file_prefix_ + s;

    cout<<"load model from "<<filename<<endl;

    FILE *file=fopen(filename.c_str(),"rb");

    //read variablies from file
    fread(&network->iterations,sizeof(int),1,file);
    fread(&network->network_level,sizeof(int),1,file);
    int network_level = network -> network_level;
    
    //allocate
    network->model_cluster_num_width = (int*) malloc(sizeof(int)*network_level) ;
    network->model_cluster_num_height = (int*) malloc(sizeof(int)*network_level) ;
    network->model_cluster_input_num_width = (int*) malloc(sizeof(int)*network_level) ;
    network->model_cluster_input_num_height = (int*) malloc(sizeof(int)*network_level) ;
    network->model_cluster_sample_num_width = (int*) malloc(sizeof(int)*network_level) ;
    network->model_cluster_sample_num_height = (int*) malloc(sizeof(int)*network_level) ;

    fread(network->model_cluster_num_width,sizeof(int)*network->network_level,1,file);
    fread(network->model_cluster_num_height,sizeof(int)*network->network_level,1,file);
    fread(network->model_cluster_input_num_width,sizeof(int)*network->network_level,1,file);
    fread(network->model_cluster_input_num_height,sizeof(int)*network->network_level,1,file);
    fread(network->model_cluster_sample_num_width,sizeof(int)*network->network_level,1,file);
    fread(network->model_cluster_sample_num_height,sizeof(int)*network->network_level,1,file);

    //count node numbers
    for (int i=0; i<network_level; i++) {
      network->num_of_cluster += network->model_cluster_num_width[i] * network->model_cluster_num_height[i];
      cout<< "#level "<<i<<" cluster = " << network->model_cluster_num_width[i] * network->model_cluster_num_height[i] << endl;

      network->num_of_node += network->model_cluster_num_width[i] * network->model_cluster_num_height[i] 
        * network->model_cluster_sample_num_width[i] * network->model_cluster_sample_num_height[i];
      cout<< "#level "<<i<<" node = " << network->model_cluster_num_width[i] * network->model_cluster_num_height[i] 
        * network->model_cluster_sample_num_width[i] * network->model_cluster_sample_num_height[i] << endl;
    }

    cout<< "#sum cluster = " << network -> num_of_cluster << endl;
    cout<< "#sum node = " << network -> num_of_node << endl;

    network->clusters= (NsodNodeCluster **) malloc(sizeof(NsodNodeCluster *)*(network->num_of_cluster/(network->pnum-1))+1);

    network -> num_of_basic_inputs = network->model_cluster_num_width[0] * network->model_cluster_num_height[0] 
      * network->model_cluster_sample_num_width[0] * network->model_cluster_sample_num_height[0]; 

    //read_activities
    network -> activity_all_node = (double*) malloc(network->num_of_node * sizeof(double)); 
    network -> activity_all_node_change = (double*) malloc(network->num_of_node * sizeof(double));

    fread(network->activity_all_node,network->num_of_node*sizeof(double),1,file); 
    fread(network->activity_all_node_change,network->num_of_node*sizeof(double),1,file); 

    //read connection
    int i=network->num_of_basic_inputs;
    int absolute_postion_level=i;
    for (int level=1;level<network_level;level++) {

      for (int y=0;y<network->model_cluster_num_height[level];y++) {
        for (int x=0;x<network->model_cluster_num_width[level];x++) {

          //if this processor should handle this cluster
          if ((i%(network->pnum-1))+1 == network->myid) {

#ifdef _DEBUG
            cout<<network->myid<<"|"<<i<<endl;
#endif
            network->clusters[network->num_of_my_cluster]=nsod_node_cluster_new(
                network->model_cluster_sample_num_width[level],
                network->model_cluster_sample_num_height[level]);

            NsodNodeCluster *cluster_iter = network->clusters[network->num_of_my_cluster];


            int position_cluster=0;
            for (int sx=0;sx<network->model_cluster_sample_num_width[level];sx++) {
              for (int sy=0;sy<network->model_cluster_sample_num_height[level];sy++) {
                //init a random weight vector
                int absolute_pos;
                int connect_dimension;
                fread(&absolute_pos,sizeof(int),1,file);
                fread(&connect_dimension,sizeof(int),1,file);

                int * connected_node_pos_tmp = (int*)malloc(sizeof(int)*connect_dimension);
                double * connection_strengh = (double*)malloc(sizeof(double)*connect_dimension);
    
                fread(connected_node_pos_tmp,connect_dimension*sizeof(int),1,file); 
                fread(connection_strengh,connect_dimension*sizeof(double),1,file); 


                cluster_iter->node[position_cluster]=nsod_node_new(sx,sy,
                    network->model_cluster_input_num_width[level],
                    network->model_cluster_input_num_height[level],
                    network->model_cluster_sample_num_width[level-1],
                    network->model_cluster_sample_num_height[level-1], 
                    absolute_pos,
                    connect_dimension,
                    connected_node_pos_tmp,connection_strengh);

#ifdef _DEBUG
                nsod_node_print_self_info(cluster_iter->node[position_cluster]);
#endif
                position_cluster++;

              }
            }

            network->num_of_my_cluster++;
          }
          i++;
        }
      }
      absolute_postion_level += network->model_cluster_num_width[level] * network->model_cluster_num_height[level] 
        * network->model_cluster_sample_num_width[level] * network->model_cluster_sample_num_height[level];

    }

    fclose(file);

  }
  return 0;
}

int nsod_network_sync_nodes_activity(NsodNetwork * network)
{
  for (int i=0; i<network->num_of_node; i++) {
    network->activity_all_node[i] = network->activity_all_node_change[i];
    network->activity_all_node_change[i]=0;
  }

#ifndef _SINGLE_THREAD
  //boardcast activity_all_node
  double* tmp_buf = new double[network->num_of_node];
  MPI_Allreduce(network->activity_all_node, tmp_buf, network->num_of_node, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  static int datatype_size = sizeof(*(network->activity_all_node));
  memcpy(network->activity_all_node, tmp_buf, datatype_size * network->num_of_node);
  delete[] tmp_buf;
#endif

  return 0;
}

bool nsod_network_main_processor(NsodNetwork * network)
{

#ifdef _SINGLE_THREAD 
  return true;
#endif

  if (network->myid == 0) {
    return true;
  } else {
    return false;
  }
}

void nsod_network_save_connection(NsodNetwork * network) {
  if (network->iterations % AUTOSAVE_ITERATION != 0) return;
  if (network->flags->model_file_prefix_.empty()) {
    network->flags->model_file_prefix_="autosave.con.";
  }
  char s[10];
  sprintf(s,"%d",network->myid);
  string filename = network->flags->model_file_prefix_ + s;

  FILE *file=fopen(filename.c_str(),"wb+");

  fwrite(&network->iterations,sizeof(int),1,file);
  fwrite(&network->network_level,sizeof(int),1,file);
  fwrite(network->model_cluster_num_width,sizeof(int)*network->network_level,1,file);
  fwrite(network->model_cluster_num_height,sizeof(int)*network->network_level,1,file);
  fwrite(network->model_cluster_input_num_width,sizeof(int)*network->network_level,1,file);
  fwrite(network->model_cluster_input_num_height,sizeof(int)*network->network_level,1,file);
  fwrite(network->model_cluster_sample_num_width,sizeof(int)*network->network_level,1,file);
  fwrite(network->model_cluster_sample_num_height,sizeof(int)*network->network_level,1,file);

  fwrite(network->activity_all_node,network->num_of_node*sizeof(double),1,file); 
  fwrite(network->activity_all_node_change,network->num_of_node*sizeof(double),1,file); 

  for (int i=0;i<network->num_of_my_cluster;i++) {
    for (int j=0;j<network->clusters[i]->sample_dimension_width*network->clusters[i]->sample_dimension_height;j++) {
      nsod_cluster_con2txt(network->clusters[i], network);
      fwrite(&network->clusters[i]->node[j]->absolute_position,sizeof(int),1,file);
      fwrite(&network->clusters[i]->node[j]->connect_dimension,sizeof(int),1,file);
      fwrite(network->clusters[i]->node[j]->connect,network->clusters[i]->node[j]->connect_dimension*sizeof(int),1,file);
      fwrite(network->clusters[i]->node[j]->strengh,network->clusters[i]->node[j]->connect_dimension*sizeof(double),1,file);
    }
  }
  fclose(file);
  printf("iteration %d saved\n",network->iterations);

  return;
}

int nsod_network_main(NsodNetwork * network)
{
  while (network->iterations++ < network->total_iterations)
  {
    /* get the information about how basic input units are fired */
    if (nsod_network_main_processor(network)) {
      //get input, if failed then exit
      if (!nsod_network_get_inputs(network)) {
        return 0;
      }
    }

    /* update activity vector and broadcast to every computer */
    nsod_network_sync_nodes_activity(network);

    /*show the network status, picture it out, or voice it out*/
    if (nsod_network_main_processor(network)) {
      nsod_network_sent_status_to_shell(network);
    }

    for (int i=0;i<network->num_of_my_cluster;i++)  /*for every cluster in this thread*/
    {
      /* compute how much the activity should be change base on the history of the network */
      /* majoy job of the network will be done here */
      nsod_network_compute_cluster_activity_change(network,network->clusters[i]); 
    }

    nsod_network_save_connection(network);

  }
}

int main(int argc, char *argv[])
{
  NsodNetwork * network;

  /* read config files, get the gui address, zero some variables, or something like that */
  /* argv should specify the connection file */
  network = nsod_network_new(argc, argv);
  if (network == NULL) {
#ifndef _SINGLE_THREAD
    MPI_Finalize();
#endif
    return -1;
  }

  /* read conncetion files, and build the data structure of the network */
  nsod_network_init_nodes(network);

  /* main loop of the network */
  nsod_network_main(network); 

#ifndef _SINGLE_THREAD
  MPI_Finalize();
#endif
  return 0;
}

