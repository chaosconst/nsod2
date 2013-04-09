/* NsodShell - Print Node Connection 
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

//#define _DEBUG

#include "NsodShell.h"
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

NsodShell * nsod_shell_new(int argc,char * argv[])
{
  NsodShell * shell;
  shell=(NsodShell *)malloc(sizeof(NsodShell));

  shell->flags = new CmdLineFlags();
  shell->flags->ParseCmdFlags(argc, argv);
  if (!shell->flags->CheckValidityShell()) {
    return NULL;
  }

  /*initilize network and nodes*/
  shell->num_of_basic_inputs = 0;	
  shell->num_of_cluster = 0;	
  shell->num_of_node = 0;	
  shell->basic_inputs = NULL;
  shell->total_iterations = shell->flags->total_iterations_;

  return shell; 
}

int nsod_shell_init_nodes(NsodShell * network)
{
  //load model file from disk

  int cluster_idx = 0;

  for (int model_file_id=1;model_file_id < network->flags->model_file_num;model_file_id++) {
    char s[10];
    sprintf(s,"%d",model_file_id);
    string filename = network->flags->model_file_prefix_ + s;

    cout<<"load model from "<<filename<<endl;

    FILE *file=fopen(filename.c_str(),"rb");

    //read variablies from file
    fread(&network->iterations,sizeof(int),1,file);
    fread(&network->network_level,sizeof(int),1,file);
    int network_level = network -> network_level;

    if (model_file_id == 1) {
      //allocate
      network->model_cluster_num_width = (int*) malloc(sizeof(int)*network_level) ;
      network->model_cluster_num_height = (int*) malloc(sizeof(int)*network_level) ;
      network->model_cluster_input_num_width = (int*) malloc(sizeof(int)*network_level) ;
      network->model_cluster_input_num_height = (int*) malloc(sizeof(int)*network_level) ;
      network->model_cluster_sample_num_width = (int*) malloc(sizeof(int)*network_level) ;
      network->model_cluster_sample_num_height = (int*) malloc(sizeof(int)*network_level) ;
    }

    fread(network->model_cluster_num_width,sizeof(int)*network->network_level,1,file);
    fread(network->model_cluster_num_height,sizeof(int)*network->network_level,1,file);
    fread(network->model_cluster_input_num_width,sizeof(int)*network->network_level,1,file);
    fread(network->model_cluster_input_num_height,sizeof(int)*network->network_level,1,file);
    fread(network->model_cluster_sample_num_width,sizeof(int)*network->network_level,1,file);
    fread(network->model_cluster_sample_num_height,sizeof(int)*network->network_level,1,file);

    if (model_file_id == 1) {
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

      network->clusters = (NsodNodeCluster **) malloc(sizeof(NsodNodeCluster *)*(network->num_of_cluster));

      network->num_of_basic_inputs = network->model_cluster_num_width[0] * network->model_cluster_num_height[0] 
        * network->model_cluster_sample_num_width[0] * network->model_cluster_sample_num_height[0]; 

      //read_activities
      network->activity_all_node = (double*) malloc(network->num_of_node * sizeof(double)); 
      network->activity_all_node_change = (double*) malloc(network->num_of_node * sizeof(double));
      network->connect_all_node_strengh = (double**) malloc(network->num_of_node * sizeof(double*));
      network->connect_all_node_pos = (int**) malloc(network->num_of_node * sizeof(int*));
      network->node = (NsodNode**) malloc(network->num_of_node * sizeof(NsodNode*));
    }
    fread(network->activity_all_node,network->num_of_node*sizeof(double),1,file); 
    fread(network->activity_all_node_change,network->num_of_node*sizeof(double),1,file); 

    //read connection
    int i=network->num_of_basic_inputs;
    int absolute_postion_level=i;
    for (int level=1;level<network_level;level++) {

      for (int y=0;y<network->model_cluster_num_height[level];y++) {
        for (int x=0;x<network->model_cluster_num_width[level];x++) {

          //if this processor should handle this cluster
          if ((i%(network->flags->model_file_num-1))+1 == model_file_id) {

#ifdef _DEBUG
            cout<<model_file_id<<"|"<<i<<endl;
#endif
            network->clusters[cluster_idx]=nsod_node_cluster_new(
                network->model_cluster_sample_num_width[level],
                network->model_cluster_sample_num_height[level]);

            NsodNodeCluster *cluster_iter = network->clusters[cluster_idx];


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
                network->connect_all_node_pos[absolute_pos] = connected_node_pos_tmp;
                
                fread(connection_strengh,connect_dimension*sizeof(double),1,file); 
                network->connect_all_node_strengh[absolute_pos] = connection_strengh;


                cluster_iter->node[position_cluster]=nsod_node_new(sx,sy,
                    network->model_cluster_input_num_width[level],
                    network->model_cluster_input_num_height[level],
                    network->model_cluster_sample_num_width[level-1],
                    network->model_cluster_sample_num_height[level-1], 
                    absolute_pos,
                    connect_dimension,
                    connected_node_pos_tmp,connection_strengh);

                network->node[absolute_pos] = cluster_iter->node[position_cluster];

#ifdef _DEBUG
                cout<<"clusterid:"<<cluster_idx<<endl;
                nsod_node_print_self_info(cluster_iter->node[position_cluster]);
#endif
                position_cluster++;

              }
            }

            cluster_idx++;
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
  
int nsod_shell_print_clusters(NsodShell * shell) {
  for (int cluster_idx=0;cluster_idx<shell->num_of_cluster - shell->num_of_basic_inputs;cluster_idx++) {
    NsodNodeCluster * cluster = shell->clusters[cluster_idx];
     
    char filename[100];
    sprintf(filename,"%s%d.txt",shell->flags->output_file_prefix.c_str(),cluster_idx);
    FILE * file = fopen(filename,"w+");

    fprintf(file,"# ImageMagick pixel enumeration: %d,%d,255,srgb\n",shell->model_cluster_num_height[0]*cluster->sample_dimension_height,shell->model_cluster_num_width[0]*cluster->sample_dimension_width);

    //go through nodes of cluster
    for (int node_idx=0;node_idx<cluster->sample_dimension_width*cluster->sample_dimension_height;node_idx++) {
      NsodNode * node = cluster->node[node_idx];
      
      int node_id = node->absolute_position;

      double * image = (double*)malloc(sizeof(double)*shell->num_of_basic_inputs);
      for (int i=0;i<shell->num_of_basic_inputs;i++) {
        image[i]=0;
      }

      nsod_shell_fill_image(shell, node_id, 1, image);

      double max = 0;
      for (int con_idx=0;con_idx<shell->num_of_basic_inputs;con_idx++) {
        if (image[con_idx]>max) max=image[con_idx];
      }

#ifdef _DEBUG
      for (int j=0;j<shell->model_cluster_num_height[0];j++) {
        for (int i=0;i<shell->model_cluster_num_width[0];i++) {
          if (max>1) printf("%5.2f ",image[j*shell->model_cluster_num_height[0]+i]/max);
          else printf("%5.2f ",image[j*shell->model_cluster_num_height[0]+i]);
        }
        printf("\n");
      }
      printf("\n");
#endif

      for (int j=0;j<shell->model_cluster_num_height[0];j++) {
        for (int i=0;i<shell->model_cluster_num_width[0];i++) {
          int color;
          if (max>1) color=255-(int)(255*image[j*shell->model_cluster_num_height[0]+i]/max);
          else color=255-(int)(255*image[j*shell->model_cluster_num_height[0]+i]);
          fprintf(file,"%d,%d: (255,%d,%d)\n",i+node->x*shell->model_cluster_num_width[0],j+node->y*shell->model_cluster_num_height[0],color,color);
        }
      }

    }
    fclose(file);
  }
  return 0;
}

int nsod_shell_fill_image(NsodShell * shell, int node_id, double fire, double * image) {
  if (fire<0.01) {
    return 0;
  }
  
  if (node_id < shell->num_of_basic_inputs) {
    image[node_id] += fire;
    return 0;
  }

  NsodNode * node = shell->node[node_id];

  for (int con_idx=0;con_idx<node->connect_dimension;con_idx++) {
    nsod_shell_fill_image(shell, node->connect[con_idx], fire * node->strengh[con_idx], image);
  }
  return 0;
}

int main(int argc, char *argv[])
{
  NsodShell * shell;

  /* read config files, get the gui address, zero some variables, or something like that */
  /* argv should specify the connection file */
  shell = nsod_shell_new(argc, argv);
  if (shell == NULL) {
    return -1;
  }

  /* read conncetion files, and build the data structure of the network */
  nsod_shell_init_nodes(shell);

  /* build a bitmap result*/
  nsod_shell_print_clusters(shell);

  return 0;
}

