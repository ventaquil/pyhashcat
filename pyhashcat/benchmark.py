#!/usr/bin/env python
import json
import sys
from time import sleep
from pyhashcat import Hashcat

def cracked_callback(sender):
    print(id(sender), "EVENT_CRACKER_HASH_CRACKED")

def finished_callback(sender):
    global finished
    finished = True

def jobfinish_callback(sender):
    print('Hash Type: ', sender.hash_mode)
    print('Speed (formatted): ', sender.hashcat_status_get_status()['Speed All'])
    print('Speed: ', sender.hashcat_status_get_status()['Speed Raw'])
    bench_dict[str(sender.hash_mode).strip()] = sender.hashcat_status_get_status()['Speed Raw']

def benchmark_status(sender):
    print(sender.hashcat_status_get_status())

bench_dict = {}
print("-------------------------------")
print("----  pyhashcat Benchmark  ----")
print("-------------------------------")

finished = False
hc = Hashcat()
hc.benchmark = True
hc.benchmark_all = True
hc.workload_profile = 4

print("[!] Hashcat object init with id: ", id(hc))
print("[!] cb_id finished: ", hc.event_connect(callback=finished_callback, signal="EVENT_OUTERLOOP_FINISHED"))
print("[!] cb_id benchmark_status: ", hc.event_connect(callback=jobfinish_callback, signal="EVENT_CRACKER_FINISHED"))
print("[!] Starting Benchmark Mode")

print("[+] Running hashcat")
if hc.hashcat_session_execute() >= 0:
    sleep(5)
    # hashcat should be running in a background thread
    # wait for it to finishing cracking
    while True:
        if finished:
            break
print('[+] Writing System Benchmark Report to: sys_benchmark.txt')
if len(sys.argv) > 1:
    bench_file = sys.argv[1]
else:
    bench_file = 'sys_benchmark.txt'
with open(bench_file, 'w') as fh_bench:
    fh_bench.write(json.dumps(bench_dict))

