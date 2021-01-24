# zosbd2


zosbd2 is the zendemic object store block device.

It is a kernel module that allows you to implement the backing data source for the block device in uesrspace.


# building

to build the kernel module

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

in another terminal you can create a filesystem on the block device.

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

unmount the filesystem
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
the kernel module then sends the results sent from userspace to the block device handler that made the initial request and then blocks again waiting for something else to do.


