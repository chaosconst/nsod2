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

#include "cmd_flags.h"

#include <iostream>
#include <sstream>

CmdLineFlags::CmdLineFlags() {
  // Assign all flags invalid values, so CheckValidity will enforce
  // users provide valid values.
  study_speed = 0.13;
  input_size_ = "32x32";
  input_sample_domain_size_ = 1;
  model_size_ = "4x4,4x4,2x2";
  sample_size_ = "8x8,10x10,8x8";
  model_file_prefix_ = "data/con/autosave/con.";
  input_file = "";
  total_iterations_ = 1000000000;
	input_width = "32,4,4,2";
	input_height = "32,4,4,2";
	sample_width = "1,8,10,12";
  sample_height = "1,8,10,12";
  num_level = 4;
  model_file_num = 5;
  output_file_prefix = "cimage/c_";
}

void CmdLineFlags::ParseCmdFlags(int argc, char** argv) {
  for (int i = 1; i < argc; ++i) {
    if (0 == strcmp(argv[i], "--study_speed")) {
      std::istringstream(argv[i+1]) >> study_speed;
      ++i;
		} else if (0 == strcmp(argv[i], "--model_file_num")) {
      std::istringstream(argv[i+1]) >> model_file_num;
      ++i;
		} else if (0 == strcmp(argv[i], "--num_level")) {
      std::istringstream(argv[i+1]) >> num_level;
      ++i;
    } else if (0 == strcmp(argv[i], "--input_width")) {
      input_width = argv[i+1];
      ++i;
    } else if (0 == strcmp(argv[i], "--input_height")) {
      input_height = argv[i+1];
      ++i;
    } else if (0 == strcmp(argv[i], "--sample_width")) {
      sample_width = argv[i+1];
      ++i;
    } else if (0 == strcmp(argv[i], "--sample_height")) {
      sample_height = argv[i+1];
      ++i;
    } else if (0 == strcmp(argv[i], "--input_size")) {
      input_size_ = argv[i+1];
      ++i;
    } else if (0 == strcmp(argv[i], "--input_sample_domain_size")) {
      std::istringstream(argv[i+1]) >> input_sample_domain_size_;
      ++i;
    } else if (0 == strcmp(argv[i], "--model_size")) {
      model_size_ = argv[i+1];
      ++i;
    } else if (0 == strcmp(argv[i], "--sample_size")) {
      sample_size_ = argv[i+1];
      ++i;
    } else if (0 == strcmp(argv[i], "--input_file")) {
      input_file = argv[i+1];
      ++i;
    } else if (0 == strcmp(argv[i], "--model_file_prefix")) {
      model_file_prefix_ = argv[i+1];
      ++i;
    } else if (0 == strcmp(argv[i], "--output_file_prefix")) {
      output_file_prefix = argv[i+1];
      ++i;
    } else if (0 == strcmp(argv[i], "--total_iterations")) {
      std::istringstream(argv[i+1]) >> total_iterations_;
      ++i;
    }
  }
}

bool CmdLineFlags::CheckValidityShell() {
  bool ret = true;
  if (study_speed <= 0) {
    std::cerr << "study_speed must > 0.\n";
    ret = false;
  }
  if (model_file_prefix_.empty()) {
    std::cerr << "must spesify model file name: using --model_file_prefix filename\n";
    ret = false;
  }
  return ret;

}

bool CmdLineFlags::CheckValidity() {
  bool ret = true;
  if (study_speed <= 0) {
    std::cerr << "study_speed must > 0.\n";
    ret = false;
  }
  if (input_file.empty()) {
    std::cerr << "must spesify input file name\n";
    ret = false;
  }
  return ret;

}
