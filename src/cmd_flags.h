// Copyright 2008 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _OPENSOURCE_GLDA_CMD_FLAGS_H__
#define _OPENSOURCE_GLDA_CMD_FLAGS_H__

#include <string>
#include <string.h>

class CmdLineFlags {
 public:
  CmdLineFlags();
  void ParseCmdFlags(int argc, char** argv);
  bool CheckValidity(); 
  bool CheckValidityShell(); 
	
  std::string 			input_width;
	std::string 			input_height;
	std::string 			sample_width;
	std::string 			sample_height;
  int					num_level;
  double      study_speed;
  std::string input_size_;
  int         input_sample_domain_size_;
  int	      	total_iterations_;
  std::string model_size_;
  std::string sample_size_;
  std::string model_file_prefix_; 
  int         model_file_num; 
  std::string input_file;
  std::string output_file_prefix;
};

#endif  // _OPENSOURCE_GLDA_CMD_FLAGS_H__
