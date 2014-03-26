#! /usr/bin/python2

import os
import sys
import subprocess
import re

self_path = os.path.abspath(__file__)
tbs_dir = os.path.dirname(self_path)

b2_dir = os.path.join(tbs_dir, "BoostBuild")
b2 = os.path.join(b2_dir, "b2")

# __________CHECK_AND_SETUP_BUILD_SYSTEM_______________________________________
os.environ['BOOST_BUILD_PATH'] = b2_dir

if not os.path.exists(b2):
	b2_bootstrap = os.path.join(b2_dir, 'bootstrap.sh')
	subprocess.call([b2_bootstrap], env=os.environ, cwd=b2_dir)

if not os.path.exists(b2):
	print "b2 bootstrap failed"
	sys.exit(-1)

# ____________CHOOSE_APROPRIATE_USER_CONFIG____________________________________

device_to_user_config = {}
device_to_user_config["linpc"] = "linpc.jam"
device_to_user_config["lindevice"] = "lindevice.jam"
device_to_user_config["tpro"] = "tpro.jam"
device_to_user_config["lin"] = "lin.jam"
device_to_user_config["win"] = "win.jam"

device_re = re.compile(r"^device=(?P<device>.*)")
device = [ device_re.search(param) for param in sys.argv ]
device = [ param.group('device') for param in device if param != None ]
if len(device) > 1:
	print "specification of more than one device is not possible"
	sys.exit(-1)

if len(device) < 1 or device[0] not in device_to_user_config.keys(): 
	print "please specify: device={"+"|".join(device_to_user_config.keys())+"}"
	sys.exit(-1)

device = device[0]

chosen_user_config = os.path.join(tbs_dir, "BJam", "user-config", device_to_user_config[device])
user_config_param = '--user-config="{chosen_user_config}"'.format(**locals())

# ___________RUN_BUILD_________________________________________________________
b2_params = sys.argv[1:] + [user_config_param]

retval = subprocess.call(["./prebuild.py"] + b2_params, cwd=tbs_dir)
if retval != 0: sys.exit(retval)

retval = subprocess.call([b2]+ b2_params, env=os.environ)
if retval != 0: sys.exit(retval)

sys.exit(0)

