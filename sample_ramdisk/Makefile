PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

OBJS = zosbd2_sample_ramdisk.o

	CFLAGS += -O2

all:	zosbd2_sample_ramdisk

zosbd2_sample_ramdisk:	$(OBJS)
	$(CXX) -o $@ $^

%.o:	$(PROJECT_ROOT)%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<


clean:
	rm -fr zosbd2_sample_ramdisk $(OBJS)
