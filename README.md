# zosbd2


zosbd2 is the zendemic object store block device.

It is a kernel module that allows you to implement the backing data source for the block device in uesrspace.


# building

to build the kernel module

```
cd src
make
insmod zosbd2.ko
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



# how it works.

