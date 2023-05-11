import sys
from pathlib import Path
from ruamel.yaml import YAML

# This script imports a .yaml config file and fills in parameter IDs
# starting with 1 and incrementing by 1 for each parameter

# Run: python fill_ids.py configs/[filename].yaml

config_path = sys.argv[1]

if not Path(config_path).exists():
    print('ERROR: Invalid configuration path')
    sys.exit()

yaml = YAML()
yaml.indent(mapping=4, sequence=4, offset=4)
yaml.allow_duplicate_keys = True

print('Loading configuration...')
with open(config_path, 'r') as config_file:
    rawTxtConfig = config_file.readlines()

#store groups data in a list with nice formatting
data_after_groups_formatted = [] 
groupsNotFound = False
for line in rawTxtConfig:
        if line.startswith("groups: ["):
             groupsNotFound = True
        if groupsNotFound == True:
            data_after_groups_formatted.append(line)

#run yaml script
with open(config_path, 'r') as config_file:
     config = yaml.load(config_file)

print('Filling IDs...')
id = 1
for name in config['parameters']:
    config['parameters'][name]['id'] = id
    id += 1

#dump config with new IDs and bad groups formatting
with open(config_path, 'w') as config_file:
    yaml.dump(config, config_file)

#open up the manipulated yaml 
with open(config_path, 'r') as config_file_after_script:
    raw_txt_after_script = config_file_after_script.readlines()

#store data with new ID's before groups in a list
data_before_groups = []
groupsFound = False
for line in raw_txt_after_script:
        if line.startswith("groups: ["):
            groupsFound = True
        if groupsFound == False:
            data_before_groups.append(line)

#stich together good scripted data with well formatted groups 
print('Reformatting Groups')
with open(config_path, 'w') as config_file:
    for line in data_before_groups:
         config_file.write(line)
    
    for line in data_after_groups_formatted:
         config_file.write(line) 

print('Done')