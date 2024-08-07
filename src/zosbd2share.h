
// SPDX-License-Identifier: LGPL-2.1-or-later

/*
 * Copyright (C) 2020-2024 stu mark
 */
#pragma once

#define kmod_block_size 4096 

#include "zosbd2userkerneldef.h"

#define MAX_DEVICE_NAME_LENGTH (DISK_NAME_LEN-1) 

#define CREATE_DEVICE_MIN_NUMBER_BLOCK_COUNT 1024 

#define CREATE_DEVICE_MAX_NUMBER_BLOCK_COUNT 3221225472ULL 

#define CREATE_DEVICE_MIN_TIMEOUT_SECONDS 1
#define CREATE_DEVICE_MAX_TIMEOUT_SECONDS 1200 

#define CREATE_DEVICE_MAX_ACTIVE_BLOCK_DEVICES 50 

#define MAX_BIO_SEGMENTS_PER_REQUEST 256 
#define MAX_BIO_SEGMENTS_BUFFER_SIZE_PER_REQUEST (MAX_BIO_SEGMENTS_PER_REQUEST * 4096) 

#define IOCTL_CONTROL_PING 12  
#define IOCTL_CONTROL_CREATE_DEVICE 21 
#define IOCTL_CONTROL_DESTROY_DEVICE_BY_ID 22 
#define IOCTL_CONTROL_DESTROY_DEVICE_BY_NAME 23
#define IOCTL_CONTROL_DESTROY_ALL_DEVICES 25
#define IOCTL_CONTROL_GET_HANDLE_ID_BY_NAME 26

#define IOCTL_CONTROL_STATUS_DEVICE_LIST 60 
#define IOCTL_CONTROL_STATUS_DEVICE_STATUS 61 

#define IOCTL_DEVICE_PING 53  

#define IOCTL_DEVICE_OPERATION 58 

#define DEVICE_OPERATION_NO_RESPONSE_BLOCK_FOR_REQUEST 30    
#define DEVICE_OPERATION_READ_RESPONSE 31        
#define DEVICE_OPERATION_WRITE_RESPONSE 37       
#define DEVICE_OPERATION_DISCARD_RESPONSE 38     

#define DEVICE_OPERATION_STATUS 39       

#define DEVICE_OPERATION_KERNEL_BLOCK_FOR_REQUEST 40 
#define DEVICE_OPERATION_KERNEL_READ_REQUEST 41
#define DEVICE_OPERATION_KERNEL_WRITE_REQUEST 42
#define DEVICE_OPERATION_KERNEL_DISCARD_REQUEST 43

#define DEVICE_OPERATION_KERNEL_USERSPACE_EXIT 8086 

#define DEVICE_OPERATION_KERNEL_USERSPACE_SIGNAL 6502

#define USERSPACE_SIGNAL_REQUEST_SHUTDOWN 86 

typedef struct control_block_device_create_params_t
  {

    unsigned char device_name[MAX_DEVICE_NAME_LENGTH+1]; 
    u32 kernel_block_size; 
    u64 number_of_blocks; 
    u32 device_timeout_seconds; 
    u32 max_segments_per_request; 
    u32 bio_segment_batch_queue_size; 

    u32 handle_id;
    s32 error;
  } control_block_device_create_params;

typedef struct control_block_device_destroy_by_id_params_t
  {

    u32 handle_id;
    s32 error;
    u32 force;
  } control_block_device_destroy_by_id_params;

typedef struct control_block_device_destroy_by_name_params_t
  {

    unsigned char device_name[MAX_DEVICE_NAME_LENGTH+1]; 
    s32 error;
    u32 force;
  } control_block_device_destroy_by_name_params;

typedef struct control_block_device_destroy_all_params_t
  {

    u32 force;
  } control_block_device_destroy_all_params;

typedef struct control_block_device_get_handle_id_by_name_params_t
  {

    unsigned char device_name[MAX_DEVICE_NAME_LENGTH+1]; 
    u32 handle_id; 
    s32 error;
  } control_block_device_get_handle_id_by_name_params;

typedef struct control_ping_t
  {

  } control_ping;

typedef struct control_status_device_list_request_response_t
  { 

  	u32 num_devices;
  	u32 handleid[CREATE_DEVICE_MAX_ACTIVE_BLOCK_DEVICES];
  } control_status_device_list_request_response;

typedef struct control_status_device_request_response_t
  { 
    u32 handle_id_request;

	u64 size; 
	u64 number_of_blocks; 
	u32 kernel_block_size; 
	u32 max_segments_per_request; 
	u32 timeout_milliseconds; 
	u32 handle_id; 
	unsigned char device_name[MAX_DEVICE_NAME_LENGTH+1]; 
  } control_status_device_request_response;

typedef union
  {
    control_block_device_create_params create_params;
    control_block_device_destroy_by_id_params destroy_params_by_id;
    control_block_device_destroy_by_name_params destroy_params_by_name;
    control_block_device_destroy_all_params destroy_params_all;
    control_block_device_get_handle_id_by_name_params get_handle_id_params_by_name;
    control_ping ping_params;
    control_status_device_list_request_response get_device_list;
    control_status_device_request_response get_device_status;
  } control_operation;

typedef struct
{

  u64 total_length_of_all_bio_segments; 
  u32 number_of_segments; 
} read_request_param_t;

typedef struct
{
  u64 response_total_length_of_all_segment_requests; 
} read_response_param_t;

typedef struct
{

  u64 total_length_of_all_bio_segments; 
  u32 number_of_segments; 

} write_request_param_t;

typedef struct
{
    u64 response_total_length_of_all_segment_requests_written; 
} write_response_param_t;

typedef struct
{

  u64 total_length_of_all_bio_segments;
  u32 number_of_segments;

} discard_request_param_t;

typedef struct
{
    u64 response_total_length_of_all_segment_requests_discarded;
} discard_response_param_t;

typedef struct
{
	u32 signal_action;
} userspace_signal_param_t;

typedef union
{
    read_request_param_t read_request;
    read_response_param_t read_response;

    write_request_param_t write_request;
    write_response_param_t write_response;

    discard_request_param_t discard_request;
    discard_response_param_t discard_response;

    userspace_signal_param_t userspace_signal;
} zosbd2_packet;

typedef struct
{
    u32 operation; 
    u64 size; 

    u32 signal_number; 
} zosbd2_header;

typedef struct
{
    u64 start;
    u64 length; 
} zosbd2_bio_segment_metadata;

typedef struct
{
    u32 handle_id; 
    zosbd2_header header;
    zosbd2_packet packet;
    zosbd2_bio_segment_metadata metadata[MAX_BIO_SEGMENTS_PER_REQUEST];

    s32 error; 
    u32 operation_id; 

    unsigned char userspacedatabuffer[0]; 

} zosbd2_operation;

#define  MAX_DISK_GROUP_LENGTH  64 
#define  MAX_OBJECTID_LENGTH  1024 

