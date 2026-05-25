# This script imports a yaml file and generates a dbc
# Run: python DBC_converter_v2.py configs/[filename].yaml

print('Running DBC_converter script...')
import re
import yaml
from yaml import SafeLoader
import sys
import os
import math

def calc_min_max(signal_type, length_bits, scale, offset):
    # Determine raw integer range
    raw_min = 0
    raw_max = (2 ** length_bits) - 1

    physical_min = raw_min * scale + offset
    physical_max = raw_max * scale + offset

    return physical_min, physical_max

def file_out(out, a, params, id, nums):
	msg_id = int(str(id), 0)
	msg_name = f"ID_{msg_id}"
	out.write(f"BO_ {msg_id} {msg_name}: 8 Vector__XXX\n")
	for b in a['parameters']:
		type=params[b['name']]['type']
		sign='+'
		aemunit=str(params[b['name']]['unit'])
		bit=1
		start=b['start']*8
		length=b['length']*8
		scale = params[b['name']]['scale']
		offset = params[b['name']]['offset']
		min_val, max_val = calc_min_max(type, length, scale, offset)
		if params[b['name']]['encoding']=="MSB":
			bit=0
			#if length>8:
			start=start+7
		if nums != None: 
			out.write(' SG_ '+b["name"]+"_"+str(nums)+' : '+str(start)+'|'+str(length)+'@'+str(bit)+sign+' ('+str(params[b['name']]['scale'])+','+str(params[b['name']]['offset'])+') ['+str(min_val)+'|'+str(max_val)+'] "'+aemunit+'" Vector__XXX\n')
		else: 
			out.write(' SG_ '+b["name"]+' : '+str(start)+'|'+str(length)+'@'+str(bit)+sign+' ('+str(params[b['name']]['scale'])+','+str(params[b['name']]['offset'])+') ['+str(min_val)+'|'+str(max_val)+'] "'+aemunit+'" Vector__XXX\n')
	out.write('\n')

	
arg=str(sys.argv[1])
print("[=] File \'"+arg+"\' selected")

def stopp():
	print("[-] Exiting...")
	exit()
if os.path.isfile(arg):
	chk=re.search(".yaml$",arg)
	if not chk:
		print("[-] \'"+arg+"\' isn't a yaml file")
		stopp()
else:
	print("[-] \'"+arg+"\' isn't a file")
	stopp()
with open(arg) as f:
	odict=yaml.safe_load(f)
	groups=odict["groups"]
	params=odict["parameters"]
		
	outname = re.sub("yaml", "dbc", str(os.path.basename(sys.argv[1])))
	
	out = open(os.path.join(arg, "../../", "dbcs", outname), "w+")
	out.write('VERSION ""\n\n\nNS_ :\n\tNS_DESC_\n\tCM_\n\tBA_DEF_\n\tBA_\n\tVAL_\n\tCAT_DEF_\n\tCAT_\n\tFILTER\n\tBA_DEF_DEF_\n\tEV_DATA_\n\tENVVAR_DATA_\n\tSGTYPE_\n\tSGTYPE_VAL_\n\tBA_DEF_SGTYPE_\n\tBA_SGTYPE_\n\tSIG_TYPE_REF_\n\tVAL_TABLE_\n\tSIG_GROUP_\n\tSIG_VALTYPE_\n\tSIGTYPE_VALTYPE_\n\tBO_TX_BU_\n\tBA_DEF_REL_\n\tBA_REL_\n\tBA_DEF_DEF_REL_\n\tBU_SG_REL_\n\tBU_EV_REL_\n\tBU_BO_REL_\n\tSG_MUL_VAL_\n\nBS_:\n\nBU_:\n\n\n')
		
	for a in groups:
		id = str(a["id"])
		file_out(out, a, params, id, None)

	out.close()
