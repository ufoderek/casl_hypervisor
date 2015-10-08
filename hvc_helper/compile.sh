arm-none-linux-gnueabi-gcc-4.6.1 -Os -march=armv7-a -mcpu=cortex-a15 -fomit-frame-pointer hvc_helper.c -o hvc_helper -static
arm-none-linux-gnueabi-objdump hvc_helper -xd > dump.txt
arm-none-linux-gnueabi-strip hvc_helper
arm-none-linux-gnueabi-objdump hvc_helper -xd > dump2.txt
