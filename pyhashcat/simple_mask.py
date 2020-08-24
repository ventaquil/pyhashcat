#!/usr/bin/env python

import os
import sys
from time import sleep
from pyhashcat import Hashcat

def cracked_callback(sender):
	print("CRACKED-", id(sender), "EVENT_CRACKER_HASH_CRACKED")

def warning_callback(sender):
    print("WARNING-", id(sender), sender.status_get_status_string())
    print(sender.hashcat_status_get_log())

def error_callback(sender):
    print("ERROR-", id(sender), sender.status_get_status_string())
    print(sender.hashcat_status_get_log())

def finished_callback(sender):
	print("FIN-", id(sender), "EVENT_CRACKER_FINISHED")

def any_callback(sender):
    status = sender.status_get_status_string()
    print("ANY-", id(sender), status)
    print(sender.status_get_event_ctx())
    if status == "Aborted":
        print(sender.status_get_event_ctx())

print("-------------------------------")
print("---- Simple pyhashcat Test ----")
print("-------------------------------")

hc = Hashcat()
# To view event types
# hc.event_types
print("[!] Hashcat object init with id: ", id(hc))
print("[!] cb_id error: ", hc.event_connect(callback=error_callback, signal="EVENT_LOG_ERROR"))
print("[!] cb_id warning: ", hc.event_connect(callback=warning_callback, signal="EVENT_LOG_WARNING"))
print("[!] cb_id cracked: ", hc.event_connect(callback=cracked_callback, signal="EVENT_CRACKER_HASH_CRACKED"))
print("[!] cb_id finished: ", hc.event_connect(callback=finished_callback, signal="EVENT_CRACKER_FINISHED"))
#print("[!] cb_id any: ", hc.event_connect(callback=any_callback, signal="ANY"))


#hc.hash = "8743b52063cd84097a65d1633f5c74f5"
hc.hash = 'admin::N46iSNekpT:08ca45b7d7ea58ee:88dcbe4446168966a153a0064958dac6:5c7830315c7830310000000000000b45c67103d07d7b95acd12ffa11230e0000000052920b85f78d013c31cdb3b92f5d765c783030'
hc.mask = "?l?l?l?l?l?l?l"
hc.quiet = True
hc.username = True
hc.potfile_disable = True
hc.outfile = os.path.join(os.path.expanduser('~'), "outfile.txt")
print("[+] Writing to ", hc.outfile)
hc.attack_mode = 3
hc.hash_mode = 5600
hc.workload_profile = 2

cracked = []
print("[+] Running hashcat")
if hc.hashcat_session_execute() >= 0:
	# hashcat should be running in a background thread
	# wait for it to finishing cracking
	i = 0
	while True:
		# do something else while cracking
		i += 1
		if i%4 == 0:
			ps = '-'
		elif i%4 == 1:
			ps = '\\'
		elif i%4 == 2:
			ps = '|'
		elif i%4 == 3:
			ps = '/'
		sys.stdout.write("%s\r" % ps)
		sys.stdout.flush()

		if hc.status_get_status_string() == "Cracked":
			break
		if hc.status_get_status_string() == "Aborted":
			break

else:
	print("STATUS: ", hc.status_get_status_string())

#sleep(5)
