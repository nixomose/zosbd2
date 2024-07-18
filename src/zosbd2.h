
// SPDX-License-Identifier: GPL-2.0-only

/*
 * Copyright (C) 2020-2024 stu mark
 */
#pragma once

#include "zosbd2share.h"

typedef struct zosbd2_context_t
  {

    pil_mutex_t response_lock; 
    pil_cv_t response_cv; 
    int response_cv_triggered_flag; 
    u32 processed; 

    zosbd2_operation op; 

    struct list_head bio_segments_op_state_list;
    int dir; 
    int operation; 
    u64 total_data_in_all_segments; 
    u32 number_of_segments_in_list; 

    struct list_head list_anchor; 

  } zosbd2_context;

  typedef struct
  {

      struct page *bv_page; 
      unsigned long offset;

      u64 request_start;
      u64 request_length;
      u64 id; 

      struct list_head list_anchor; 

  } zosbd2_operation_state;

int magic = sizeof(read_request_param_t) +
            sizeof(read_response_param_t) +
            sizeof(zosbd2_operation) +
            sizeof(zosbd2_context); 
