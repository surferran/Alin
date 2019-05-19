"""
https://docs.python.org/3.3/howto/regex.html

sample by HTTP GET request

"""
import urllib.request
import re
import time 
import matplotlib.pyplot as plt

MEAS_FREQ    = 0.5 # [sec]
NUM_OF_READS = 5
ESP_IP_STRING = "http://10.0.0.79"

desired_keys = ['analog_read', 'read_time']
collected_data = {}
collected_data[desired_keys[0]] = []
collected_data[desired_keys[1]] = []

# ptrn=r'\s=\d([^"\n]+)'
# ptrn = r'(\S+)=(\d+)'
# ptrn = r'=(\d+)'
ptrn = r'(\w+)=(\d+)'

for x in range(0, NUM_OF_READS):
    contents = urllib.request.urlopen(ESP_IP_STRING).read()

    findings = re.findall(ptrn, str(contents))

    collected_data[desired_keys[0]].append(findings[0][1])
    collected_data[desired_keys[1]].append(findings[1][1])

    time.sleep(MEAS_FREQ)

plt.figure()
plt.plot(collected_data[desired_keys[1]], collected_data[desired_keys[0]], marker='.')
plt.title(desired_keys[1] + " vs " + desired_keys[0])
plt.show()