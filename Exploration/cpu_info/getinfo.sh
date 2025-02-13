#!/bin/bash

# System Information
echo "--- Raspberry Pi System Information ---" > rpi_metrics.log
echo "SoC Information:" >> rpi_metrics.log
cat /proc/cpuinfo | grep "Hardware\|Model\|Revision" >> rpi_metrics.log

echo "OS and Kernel Version:" >> rpi_metrics.log
uname -a >> rpi_metrics.log

echo "Available RAM:" >> rpi_metrics.log
free -h >> rpi_metrics.log

echo "Cache Sizes:" >> rpi_metrics.log
lscpu | grep cache >> rpi_metrics.log

echo "Boot Time:" >> rpi_metrics.log
systemd-analyze >> rpi_metrics.log

echo "Compiler Version:" >> rpi_metrics.log
gcc --version >> rpi_metrics.log

echo "Memory Copy Speed:" >> rpi_metrics.log
dd if=/dev/zero of=/dev/null bs=1M count=1000 2>> rpi_metrics.log

echo "Filesystem Write Speed:" >> rpi_metrics.log
dd if=/dev/zero of=testfile bs=1M count=1000 conv=fdatasync 2>> rpi_metrics.log

echo "Power Consumption and Temperature:" >> rpi_metrics.log
vcgencmd measure_volts core >> rpi_metrics.log
vcgencmd measure_temp >> rpi_metrics.log

echo "Benchmarking Completed. Results saved in rpi_metrics.log"
