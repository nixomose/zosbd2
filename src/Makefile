



ifdef src
  BUILD_VERSION := $(shell bash -c 'cd $(src); if [ -e install_version.txt ]; then cat install_version.txt; else echo `date +%Y%m%d_%H%M_``git rev-parse --short HEAD`; fi')

  $(info version ${BUILD_VERSION})
  #BUILD_SRC_DIR := $(shell echo $(src))
  #$(info srcdir ${BUILD_SRC_DIR})
endif 

HAVE_COMBINED_BLK_ALLOC_QUEUE := $(shell /bin/bash -c 'grep "blk_alloc_queue.*make_request_fn" /usr/src/linux-headers-`uname -r`/include/linux/blkdev.h > /dev/null; echo $$(( 1 - $$? ))')
$(info HAVE_COMBINED_BLK_ALLOC_QUEUE = ${HAVE_COMBINED_BLK_ALLOC_QUEUE})

HAVE_BLK_ALLOC_QUEUE_NODE := $(shell /bin/bash -c 'grep "blk_alloc_queue.*(int node_id)" /usr/src/linux-headers-`uname -r`/include/linux/blkdev.h > /dev/null; echo $$(( 1 - $$? ))')
$(info HAVE_BLK_ALLOC_QUEUE_NODE = ${HAVE_BLK_ALLOC_QUEUE_NODE})

HAVE_BIO_START_IO_ACCT        := $(shell /bin/bash -c 'grep "bio_start_io_acct.*struct bio"    /usr/src/linux-headers-`uname -r`/include/linux/blkdev.h > /dev/null; echo $$(( 1 - $$? ))')
$(info HAVE_BIO_START_IO_ACCT = ${HAVE_BIO_START_IO_ACCT})

HAVE_BLK_QUEUE_FLAG_SET        := $(shell /bin/bash -c 'grep "blk_queue_flag_set"    /usr/src/linux-headers-`uname -r`/include/linux/blkdev.h > /dev/null; echo $$(( 1 - $$? ))')
$(info HAVE_BLK_QUEUE_FLAG_SET = ${HAVE_BLK_QUEUE_FLAG_SET})



obj-m += zosbd2.o
zosbd2-y := zosbd2tools.o zosbd2main.o


MY_CFLAGS += -DHAVE_COMBINED_BLK_ALLOC_QUEUE=${HAVE_COMBINED_BLK_ALLOC_QUEUE}
MY_CFLAGS += -DHAVE_BIO_START_IO_ACCT=${HAVE_BIO_START_IO_ACCT}
MY_CFLAGS += -DHAVE_BLK_ALLOC_QUEUE_NODE=${HAVE_BLK_ALLOC_QUEUE_NODE}
MY_CFLAGS += -DHAVE_BLK_QUEUE_FLAG_SET=${HAVE_BLK_QUEUE_FLAG_SET}
ccflags-y += ${MY_CFLAGS} -O2
ccflags-y += -DBUILD_VERSION=\"$(BUILD_VERSION)\"

CC += ${MY_CFLAGS}


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules EXTRA_CFLAGS="${MY_CFLAGS}"

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

