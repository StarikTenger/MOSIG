import subprocess
import os
import matplotlib.pyplot as plt 
import numpy as np
import re

time_serial = []
time_parallel = []
N_list = []
nr_threads_list = [2, 4, 8, 16]

#type the command to run the right executable
exec_name = "./odd-even.exe"

## start and stop of input parameters
start_range = 5
end_range = 17      # maybe 14-18 for bubble and odd even sort and 20-23 for merge sort

## first do the serial part
for i in range(start_range, end_range):
    N = i
    N_list.append(N)
    result = subprocess.run([exec_name, str(N)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    res_str = result.stdout.decode('utf-8')
    serial_time = re.findall(r"[-+]?\d*\.\d+|\d+ s", res_str)[1]
    time_serial.append(float(serial_time))


## then do the parallel part
for nr_threads in nr_threads_list:
    # set the number of threads
    os.environ["OMP_NUM_THREADS"] = str(nr_threads)
    _time_parallel = []
    for i in range(start_range, end_range):
        N = i
        result = subprocess.run([exec_name, str(N)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        res_str = result.stdout.decode('utf-8')
        parallel_time = re.findall(r"[-+]?\d*\.\d+|\d+ s", res_str)[4]
        _time_parallel.append(float(parallel_time))
    time_parallel.append(_time_parallel)


## plot the results
plt.plot(N_list, time_serial, label="Serial")
for i in range(len(nr_threads_list)):
    plt.plot(N_list, time_parallel[i], label=f"Parallel {nr_threads_list[i]} threads")
plt.xlabel('N')
plt.ylabel('Time (s)')
plt.yscale('log')
plt.legend()
plt.show()
