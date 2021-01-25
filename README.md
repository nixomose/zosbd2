# zosbd2


zosbd2 is the zendemic object store block device.

It is a kernel module that allows you to implement the backing data source for the block device in userspace.

read more about zendemic and the zendemic object store here: http://zendemic.net


# building

to build the kernel module you must have the kernel headers for you running kernel installed.

ubuntu/debian:
sudo apt install linux-headers-$(uname -r)

raspbian:
sudo apt install raspberrypi-kernel-headers

fedora:
sudo yum -y install kernel-devel kernel-headers

once the headers are installed, you can build the kernel module

```
cd src
make
sudo insmod zosbd2.ko
```

to build the sample ramdisk userspace implementation

```
cd sample_ramdisk
make
```


# sample program

if you run the resulting binary from the build....

```
sudo ./zosbd2_sample_ramdisk
```
the process will create a block device called `/dev/zosbd2ramdisk`  and the process will block handling requests to that block device.

in another terminal you can create a file system on the block device.

```
sudo mkfs.ext4 /dev/zosbd2ramdisk
```
then you can mount it

```
mkdir mount
sudo mount /dev/zosbd2ramdisk mount
```

then you can `cd mount` to use the ramdisk created by zosbd2.

to cleanly exit the block device

unmount the file system
```
umount mount
```

to exit the userspace process cleanly and destroy the block device, run the userspace program with the -d parameter.

```
sudo ./zosbd2_sample_ramdisk -d
```

that will destroy the block device and the zosbd2_sample_ramdisk process in the first terminal will exit.



# how it works

the kernel module once loaded, creates a misc device called `/dev/zosbd2ctl`.
this device is how userspace applications can make requests to the zosbd2 kernel module to create and destroy block devices. the kernel module supports multiple concurrent block devices.

when the userspace program starts, it sends an ioctl to the control device with information about the size and shape of block device to make.
the kernel module makes the block device in the kernel, and creates a queue in its memory to hold requests.

the the userspace program makes an ioctl to the newly created block device, and it blocks waiting for something to appear on the queue.
when a request comes in for the block device, the information about the request (is it a read or a write, and if it's a write, what data needs to be written) is returned to the userspace application that was blocking on the ioctl call.

the userspace application then handles the request either collecting the data for read requests or storing the data for write requests, and then calls the ioctl again with the results of the request that was handled.

the kernel module then sends the results sent from userspace to the kernel for the block device that the initial request came from and then blocks again waiting for something else to do.

this cycle continues until an ioctl is make to the control device to destroy the block device.

when a block device is destroyed, the kernel module sends a special response to the userspace blocking ioctl saying basically there are no more requests coming and the userspace program should not call back.


# more details

there are a number of caveats and limitations that I'm still working on. it's a work in progress but is perfectly stable so far as I've tested it.

it builds on 32 bit and 64 bit machines.

it runs on raspbian and ubuntu 18.04 and 20.04

I've tested it with the 4.15 kernel and the 5.8 kernel and the 5.10 kernel.

because of the way the sample program is written it can only handle one request at a time.
all requests from the block device are handled roughly in the order in which they came in.

it is possible to have multiple userspace programs make the ioctl to get requests from the kernel and run them in parallel. I haven't tested this case yet but it is designed to work that way.

the one case where I know this will work less than optimally is that at the moment the kernel module only expects one userspace program to be pulling off the work queue so when it destroys a block device it will only send one go-away request to userspace, and if there are more they will block until the userspace process is killed manually. On my list of things to fix.

Another problem has to do with clean shutdown.
you need to make sure you have sync sync synced everything to the block device before you call destroy on it, there is currently some ambiguity in the kernel module about how many callers are talking to it, so the cleaner way of exiting is disabled at the moment, and all destroy block device requests are forced, if all the data isn't flushed by destroy time, it will be lost.

I'm currently working on another feature to queue multiple small requests from the kernel, for example when the page cache is painfully slowly feeling the kernel module one 4k block at a time so it can batch up the requests and send them to userspace in one shot. this is not completed yet and is disabled at the moment.





