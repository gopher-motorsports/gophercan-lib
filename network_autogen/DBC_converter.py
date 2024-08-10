print('???')
import re
import yaml
from yaml import SafeLoader
import sys
import os
import math

def file_out_mult_bus(out, a, params, id, nums):
	out.write('BO_ '+id+' '+id+': 8 Vector__XXX\n')
	for b in a['parameters']:
		type=params[b['name']]['type']
		sign='+'
		if type in ('SIGNED16','FLOATING'):
			sign='-'
		units={"Nm":"torque:N.m","psi":"pressure:psi","mph":"speed:mph","rpm":"angular_speed:rpm","%":"fraction:%","LA":"afr:LA","dBTDC":"unitless:","C":"temperature:C","kPa":"pressure:kPa","V":"voltage:V","":"unitless:","deg":"angle:deg","ft":"distance:ft","mm":"distance:mm","inH2O":"pressure:inH2O","L/min":"volume_flow:L/min","deg/s":"angular_speed:deg/s","G":"acceleration:G","Hz":"frequency:Hz","kHz":"frequency:kHz","A":"current:A","Wb":"flux:Wb","state":"current_state:state", "s":"time:s","unit":"unit:unit"}
		aemunit=units[params[b['name']]['unit']]
		bit=1
		start=b['start']*8
		length=b['length']*8
		if params[b['name']]['encoding']=="MSB":
			bit=0
			#if length>8:
			start=start+7
		if nums != None: 
			out.write(' SG_ _'+b["name"]+"_"+str(nums)+' : '+str(start)+'|'+str(length)+'@'+str(bit)+sign+' ('+str(params[b['name']]['scale'])+','+str(params[b['name']]['offset'])+') [0|0] "'+aemunit+'" Vector__XXX\n')
		else: 
			out.write(' SG_ _'+b["name"]+' : '+str(start)+'|'+str(length)+'@'+str(bit)+sign+' ('+str(params[b['name']]['scale'])+','+str(params[b['name']]['offset'])+') [0|0] "'+aemunit+'" Vector__XXX\n')
	out.write('\n')

	
arg=str(sys.argv[1])
print("[=] File \'"+arg+"\' selected")

def stopp():
	print("[-] Exiting...")
	exit()
if os.path.isfile(arg):
	chk=re.search("\.yaml$",arg)
	if not chk:
		print("[-] \'"+arg+"\' isn't a yaml file dumbass")
		stopp()
else:
	print("[-] \'"+arg+"\' isn't a file dumbass")
	stopp()
with open(arg) as f:
	odict=yaml.safe_load(f)
	groups=odict["groups"]
	params=odict["parameters"]
	modules=odict["modules"]
	id_arr = []
	for mod_name, mod_data in modules.items():
		id_arr.append({"id": int(mod_data["id"]), "buses":mod_data["buses"]})
		
	outname1 = "dbc_GCAN0_"+re.sub("yaml","dbc",str(os.path.basename(sys.argv[1])))
	outname2 = "dbc_GCAN1_"+re.sub("yaml","dbc",str(os.path.basename(sys.argv[1])))
	outname3 = "dbc_GCAN2_"+re.sub("yaml","dbc",str(os.path.basename(sys.argv[1])))
	# if os.path.isfile(outname1):
	# 	print("[-] \'"+outname1+"\' is already a file dumbass")
	# 	stopp()
	# if os.path.isfile(outname2):
	# 	print("[-] \'"+outname2+"\' is already a file dumbass")
	# 	stopp()
	# 		# wont need this when we switch to two buses
	# if os.path.isfile(outname3):
	# 	print("[-] \'"+outname3+"\' is already a file dumbass")
	# 	stopp()
	
	out1=open(os.path.join(arg, "../../", "dbcs", outname1),"w+")
	out1.write('VERSION ""\n\n\nNS_ :\n\tNS_DESC_\n\tCM_\n\tBA_DEF_\n\tBA_\n\tVAL_\n\tCAT_DEF_\n\tCAT_\n\tFILTER\n\tBA_DEF_DEF_\n\tEV_DATA_\n\tENVVAR_DATA_\n\tSGTYPE_\n\tSGTYPE_VAL_\n\tBA_DEF_SGTYPE_\n\tBA_SGTYPE_\n\tSIG_TYPE_REF_\n\tVAL_TABLE_\n\tSIG_GROUP_\n\tSIG_VALTYPE_\n\tSIGTYPE_VALTYPE_\n\tBO_TX_BU_\n\tBA_DEF_REL_\n\tBA_REL_\n\tBA_DEF_DEF_REL_\n\tBU_SG_REL_\n\tBU_EV_REL_\n\tBU_BO_REL_\n\tSG_MUL_VAL_\n\nBS_:\n\nBU_:\n\n\n')
	
	out2=open(os.path.join(arg, "../../", "dbcs", outname2),"w+")
	out2.write('VERSION ""\n\n\nNS_ :\n\tNS_DESC_\n\tCM_\n\tBA_DEF_\n\tBA_\n\tVAL_\n\tCAT_DEF_\n\tCAT_\n\tFILTER\n\tBA_DEF_DEF_\n\tEV_DATA_\n\tENVVAR_DATA_\n\tSGTYPE_\n\tSGTYPE_VAL_\n\tBA_DEF_SGTYPE_\n\tBA_SGTYPE_\n\tSIG_TYPE_REF_\n\tVAL_TABLE_\n\tSIG_GROUP_\n\tSIG_VALTYPE_\n\tSIGTYPE_VALTYPE_\n\tBO_TX_BU_\n\tBA_DEF_REL_\n\tBA_REL_\n\tBA_DEF_DEF_REL_\n\tBU_SG_REL_\n\tBU_EV_REL_\n\tBU_BO_REL_\n\tSG_MUL_VAL_\n\nBS_:\n\nBU_:\n\n\n')
	
	# wont need these two lines when we switch to two buses
	out3=open(os.path.join(arg, "../../", "dbcs", outname3),"w+")
	out3.write('VERSION ""\n\n\nNS_ :\n\tNS_DESC_\n\tCM_\n\tBA_DEF_\n\tBA_\n\tVAL_\n\tCAT_DEF_\n\tCAT_\n\tFILTER\n\tBA_DEF_DEF_\n\tEV_DATA_\n\tENVVAR_DATA_\n\tSGTYPE_\n\tSGTYPE_VAL_\n\tBA_DEF_SGTYPE_\n\tBA_SGTYPE_\n\tSIG_TYPE_REF_\n\tVAL_TABLE_\n\tSIG_GROUP_\n\tSIG_VALTYPE_\n\tSIGTYPE_VALTYPE_\n\tBO_TX_BU_\n\tBA_DEF_REL_\n\tBA_REL_\n\tBA_DEF_DEF_REL_\n\tBU_SG_REL_\n\tBU_EV_REL_\n\tBU_BO_REL_\n\tSG_MUL_VAL_\n\nBS_:\n\nBU_:\n\n\n')
		
	for a in groups: 
		id=str(a["id"])
		if int(id) >= 372 and int(id) <= 380:
			buses = "GCAN0"
		elif int(id) == 16:
			buses = "GCAN1"
		elif int(id) >= 160 and int(id) <= 193:
			buses = "GCAN1"
		else:
			temp_id = math.floor(int(id)/256) 
			#print(temp_id)
			buses = id_arr[temp_id]["buses"]
			
		### all of the following would be deleted for two can buses
		if "GCAN0" in buses and "GCAN1" in buses and "GCAN2" in buses:
			file_out_mult_bus(out1, a, params, id, 0)
			file_out_mult_bus(out2, a, params, id, 1)
			file_out_mult_bus(out3, a, params, id, 2)
		elif "GCAN0" in buses and "GCAN1" in buses:
			file_out_mult_bus(out1, a, params, id, 0)
			file_out_mult_bus(out2, a, params, id, 1)
		elif "GCAN1" in buses and "GCAN2" in buses: 
			file_out_mult_bus(out2, a, params, id, 1)
			file_out_mult_bus(out3, a, params, id, 2)
		elif "GCAN0" in buses and "GCAN2" in buses:
			file_out_mult_bus(out1, a, params, id, 0)
			file_out_mult_bus(out3, a, params, id, 2)
		elif "GCAN0" in buses:
			file_out_mult_bus(out1, a, params, id, None)
		elif "GCAN1" in buses:
			file_out_mult_bus(out2, a, params, id, None)
		elif "GCAN2" in buses:
			file_out_mult_bus(out3, a, params, id, None)


		### This is the code that is needed for the updated .yaml files with only two GCAN buses
  
		# if "GCAN0" in buses and "GCAN1" in buses:
			# file_out_mult_bus(out1, a, params, id, 0)
			# file_out_mult_bus(out2, a, params, id, 1)
		# elif "GCAN0" in buses:
		# 	file_out_mult_bus(out1, a, params, id, None)
		# elif "GCAN1" in buses: 
		# 	file_out_mult_bus(out2, a, params, id, None)

	out1.close()
	out2.close()
	out3.close() # wont need this when we switch to the two buses
            




