//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <iostream>
#include <cstdio>
#include "keystone.h"
#include "edge_wrapper.h"
#include "report.h"
#include "test_dev_key.h"
#include "malloc.h"

#define KEYSTONE_DEV_PATH "/dev/keystone_enclave"

const char* longstr = "hellohellohellohellohellohellohellohellohellohello";

unsigned long print_buffer(char* str){
  printf("Enclave said: %s",str);
  return strlen(str);
}

unsigned long reserve_memory_from_driver(int num_pages)
{
  vaddr_t runtime_max_vaddr = 1;
  vaddr_t enclave_max_vaddr = 2;
  vaddr_t max_vaddr;
  if(runtime_max_vaddr > enclave_max_vaddr)
  {
    max_vaddr = runtime_max_vaddr;
  }
  else
  {
    max_vaddr = enclave_max_vaddr;
  }
  int fd = open(KEYSTONE_DEV_PATH, O_RDWR);
  if (fd < 0) {
    printf("cannot open device file\n");
  }

  struct keystone_ioctl_alloc_vspace vspace;
  vspace.eid = 1;
  vspace.vaddr = max_vaddr - max_vaddr % 4096 + 4096 * 2;
  vspace.size = num_pages* 4096;
  if(ioctl(fd, KEYSTONE_IOC_ALLOC_VSPACE, &vspace)) {
    printf("failed to reserve vspace - ioctl() failed\n");
  }
  close(fd);
  return (unsigned long)vspace.vaddr;
}

void print_value(long val){
  printf("Enclave said value: %ld\n",val);
  return;
}

const char* get_host_string(){
  return longstr;
}

static struct report_t report;

void print_hex(void* buffer, size_t len)
{
  int i;
  for(i = 0; i < len; i+=sizeof(uintptr_t))
  {
    printf("%.16lx ", *((uintptr_t*) ((uintptr_t)buffer + i)));
  }
  printf("\n");
}

void copy_report(void* buffer)
{
  Report report;

  report.fromBytes((unsigned char*)buffer);
  report.printJson();

  if (report.checkSignaturesOnly(_sanctum_dev_public_key))
  {
    printf("Attestation report SIGNATURE is valid\n");
  }
  else
  {
    printf("Attestation report is invalid\n");
  }
}

int main(int argc, char** argv)
{
  if(argc != 3)
  {
    printf("Usage: %s <eapp> <runtime>\n", argv[0]);
    return 0;
  }
  Keystone enclave;
  Params params;

  //params.setEnclaveEntry(0x1000);
  params.setRuntimeStack(128*1024);
  params.setEnclaveStack(128*1024);

  enclave.init(argv[1], argv[2], params);

  edge_init(&enclave);

  enclave.run();

  return 0;
}
