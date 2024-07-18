
# $(info to setup the kernel headers directory to point to your running kernel)
# $(info cd /usr/src)
# $(info ln -s /usr/src/linux-headers-$$(uname -r) linux-headers)
# $(info to install kernel headers on a pi apt install raspberrypi-kernel-headers )
# $(info )
# $(info )
# $(info do this before make )
# $(info cd /lib/modules/`uname -r` )
# $(info ln -s /usr/src/linux-headers-`uname -r`  build )

# you want to end up with this:
# pi@pi205:/lib/modules/4.19.97-v7l+$ l build
# lrwxrwxrwx 1 root root 35 Feb 12 08:33 build -> /usr/src/linux-headers-4.19.97-v7l+


# makefile seems to be processed a few times after make clean, and $src isn't always set
# if src isn't set, cd src will go to ~/ and git rev-parse will fail
ifdef src
  BUILD_VERSION := $(shell bash -c 'cd $(src); if [ -e install_version.txt ]; then cat install_version.txt; else echo `date +%Y%m%d_%H%M_``git rev-parse --short HEAD`; fi')

  $(info version ${BUILD_VERSION})
  #BUILD_SRC_DIR := $(shell echo $(src))
  #$(info srcdir ${BUILD_SRC_DIR})
endif 

# 2/25/2023 removing all these pre 5.19 kernel HAVE_* feature flags 
#           I'm going to start all over with 5.19 and make new feature 
#           flags if more things break over time

# # 1/7/2021 do some feature tests because the block device interface changed in kernel 5.8
# HAVE_COMBINED_BLK_ALLOC_QUEUE := $(shell /bin/bash -c 'grep "blk_alloc_queue.*make_request_fn" /usr/src/linux-headers-`uname -r`/include/linux/blkdev.h > /dev/null; echo $$(( 1 - $$? ))')
# $(info HAVE_COMBINED_BLK_ALLOC_QUEUE = ${HAVE_COMBINED_BLK_ALLOC_QUEUE})
# 
# HAVE_BLK_ALLOC_QUEUE_NODE := $(shell /bin/bash -c 'grep "blk_alloc_queue.*(int node_id)" /usr/src/linux-headers-`uname -r`/include/linux/blkdev.h > /dev/null; echo $$(( 1 - $$? ))')
# $(info HAVE_BLK_ALLOC_QUEUE_NODE = ${HAVE_BLK_ALLOC_QUEUE_NODE})
# 
# HAVE_BIO_START_IO_ACCT        := $(shell /bin/bash -c 'grep "bio_start_io_acct.*struct bio"    /usr/src/linux-headers-`uname -r`/include/linux/blkdev.h > /dev/null; echo $$(( 1 - $$? ))')
# $(info HAVE_BIO_START_IO_ACCT = ${HAVE_BIO_START_IO_ACCT})
# 
# # 1/31/2021 they changed the name of queue flag set
# HAVE_BLK_QUEUE_FLAG_SET        := $(shell /bin/bash -c 'grep "blk_queue_flag_set"    /usr/src/linux-headers-`uname -r`/include/linux/blkdev.h > /dev/null; echo $$(( 1 - $$? ))')
# $(info HAVE_BLK_QUEUE_FLAG_SET = ${HAVE_BLK_QUEUE_FLAG_SET})
# 
# # 12/24/2021 they got rid of hd_struct somewhere around 5.11
# HAVE_HD_STRUCT                 := $(shell /bin/bash -c 'grep "struct hd_struct"      /usr/src/linux-headers-`uname -r`/include/linux/genhd.h > /dev/null; echo $$(( 1 - $$? ))')
# $(info HAVE_HD_STRUCT = ${HAVE_HD_STRUCT})
# 
# # 1/28/2022 they seem to have replaced bi->disk (gendisk) with bi->bdev (block_device) in 5.13
# HAVE_BIO_BI_BDEV               := $(shell /bin/bash -c 'grep "struct.*block_device.*bi_bdev" /usr/src/linux-headers-`uname -r`/include/linux/blk_types.h > /dev/null; echo $$(( 1 - $$? ))')
# $(info HAVE_BIO_BI_BDEV = ${HAVE_BIO_BI_BDEV})
# 
# # 1/28/2022 disk_part_iter seems to have gone away as well in 5.13
# HAVE_DISK_PART_ITER            := $(shell /bin/bash -c 'grep "disk_part_iter" /usr/src/linux-headers-`uname -r`/include/linux/genhd.h > /dev/null; echo $$(( 1 - $$? ))')
# $(info HAVE_DISK_PART_ITER = ${HAVE_DISK_PART_ITER})
# 
# # 7/2/2022 5.14 did away with blk_alloc_queue, the queue is now made when we allocate a gendisk
# HAVE_BLK_ALLOC_DISK            := $(shell /bin/bash -c 'grep "blk_alloc_disk" /usr/src/linux-headers-`uname -r`/include/linux/genhd.h > /dev/null; echo $$(( 1 - $$? ))')
# $(info HAVE_BLK_ALLOC_DISK = ${HAVE_BLK_ALLOC_DISK})

# 7/2/2022 5.14 did away with blk_alloc_queue, the queue is now made when we allocate a gendisk
HAVE_GENHD_FL_NO_PART            := $(shell /bin/bash -c 'grep "GENHD_FL_NO_PART\s"             /usr/src/linux-headers-`uname -r`/include/linux/blkdev.h > /dev/null; echo $$(( 1 - $$? ))')
$(info HAVE_GENHD_FL_NO_PART = ${HAVE_GENHD_FL_NO_PART})

HAVE_VOID_SUBMIT_BIO             := $(shell /bin/bash -c 'grep "void (.submit_bio).struct bio" /usr/src/linux-headers-`uname -r`/include/linux/blkdev.h > /dev/null; echo $$(( 1 - $$? ))')
$(info HAVE_VOID_SUBMIT_BIO  = ${HAVE_VOID_SUBMIT_BIO})

HAVE_QUEUE_FLAG_DISCARD_REMOVED  := $(shell /bin/bash -c 'grep "QUEUE_FLAG_DISCARD" /usr/src/linux-headers-`uname -r`/include/linux/blkdev.h > /dev/null; echo $$(( $$? ))')
$(info HAVE_QUEUE_FLAG_DISCARD_REMOVED  = ${HAVE_QUEUE_FLAG_DISCARD_REMOVED})

HAVE_BLOCK_DEVICE_OPERATIONS_OPEN_GENDISK  := $(shell /bin/bash -c 'grep "int...open.struct gendisk" /usr/src/linux-headers-`uname -r`/include/linux/blkdev.h > /dev/null; echo $$(( $$? ))')
$(info BLOCK_DEVICE_OPERATIONS_OPEN_GENDISK  = ${BLOCK_DEVICE_OPERATIONS_OPEN_GENDISK})


obj-m += zosbd2.o
# breakpoint.o
zosbd2-y := zosbd2tools.o zosbd2main.o

# breakpoint-y := breakpointsrc.o

# for when debugging the kernel
# MY_CFLAGS += -g -DDEBUG -O0
# for release mode
# MY_CFLAGS += -g -DDEBUG 
# MY_CFLAGS += -DHAVE_COMBINED_BLK_ALLOC_QUEUE=${HAVE_COMBINED_BLK_ALLOC_QUEUE}
# MY_CFLAGS += -DHAVE_BIO_START_IO_ACCT=${HAVE_BIO_START_IO_ACCT}
# MY_CFLAGS += -DHAVE_BLK_ALLOC_QUEUE_NODE=${HAVE_BLK_ALLOC_QUEUE_NODE}
# MY_CFLAGS += -DHAVE_BLK_QUEUE_FLAG_SET=${HAVE_BLK_QUEUE_FLAG_SET}
# MY_CFLAGS += -DHAVE_HD_STRUCT=${HAVE_HD_STRUCT}
# MY_CFLAGS += -DHAVE_BIO_BI_BDEV=${HAVE_BIO_BI_BDEV}
# MY_CFLAGS += -DHAVE_DISK_PART_ITER=${HAVE_DISK_PART_ITER}
# MY_CFLAGS += -DHAVE_BLK_ALLOC_DISK=${HAVE_BLK_ALLOC_DISK}
MY_CFLAGS += -DHAVE_GENHD_FL_NO_PART=${HAVE_GENHD_FL_NO_PART}
MY_CFLAGS += -DHAVE_VOID_SUBMIT_BIO=${HAVE_VOID_SUBMIT_BIO}
MY_CFLAGS += -DHAVE_QUEUE_FLAG_DISCARD_REMOVED=${HAVE_QUEUE_FLAG_DISCARD_REMOVED}
MY_CFLAGS += -DHAVE_BLOCK_DEVICE_OPERATIONS_OPEN_GENDISK=${HAVE_BLOCK_DEVICE_OPERATIONS_OPEN_GENDISK}

ccflags-y += ${MY_CFLAGS} -O2
ccflags-y += -DBUILD_VERSION=\"$(BUILD_VERSION)\"

CC += ${MY_CFLAGS}


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules EXTRA_CFLAGS="${MY_CFLAGS}"

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

