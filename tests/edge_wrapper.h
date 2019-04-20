//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _EDGE_WRAPPER_H_
#define _EDGE_WRAPPER_H_

#include <edge_call.hpp>
#include "keystone.h"

typedef struct packaged_str{
  unsigned long str_offset;
  size_t len;
} packaged_str_t;

typedef struct mem
{
  void* addr;
  long size;
  int prot;
}mem;

typedef struct file_struct
{
  long block_size;
  long num_blocks;
  long offset;
}file_struct;

typedef unsigned char byte;

int edge_init(Keystone* enclave);

void print_buffer_wrapper(void* buffer);
unsigned long print_buffer(char* str);
unsigned long reserve_memory_from_driver(int num_pages);

void print_value_wrapper(void* buffer);
void print_value(long val);
void get_number(void* buffer);
unsigned long input_number();
void set_mem_perm(void* buffer);
void get_pagesize(void* buffer);
void read_from_file_wrapper(void* buffer);
void reserve_page(void* buffer);

void copy_report_wrapper(void* buffer);
void copy_report(void* shared_buffer);

void get_host_string_wrapper(void* buffer);
const char* get_host_string();

#endif /* _EDGE_WRAPPER_H_ */
