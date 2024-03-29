import sys
from pathlib import Path
import yaml
from jinja2 import Template

# This script imports a .yaml config file and generates resources for the GopherCAN library
# Run: python autogen.py configs/[filename].yaml
# Generates:
# ../GopherCAN_network.h
# ../GopherCAN_network.c

param_structs = {
    "UNSIGNED8" : "U8_CAN_STRUCT",
    "UNSIGNED16" : "U16_CAN_STRUCT",
    "UNSIGNED32" : "U32_CAN_STRUCT",
    "UNSIGNED64" : "U64_CAN_STRUCT",
    "SIGNED8" : "S8_CAN_STRUCT",
    "SIGNED16" : "S16_CAN_STRUCT",
    "SIGNED32" : "S32_CAN_STRUCT",
    "SIGNED64" : "S64_CAN_STRUCT",
    "FLOATING" : "FLOAT_CAN_STRUCT"
}

filenames = [
    'GopherCAN_network.h',
    'GopherCAN_network.c'
]

config_path = sys.argv[1]

if not Path(config_path).exists():
    print('ERROR: Invalid configuration path')
    sys.exit()

print('Loading configuration...')
with open(config_path) as config_file:
    config = yaml.safe_load(config_file)

# rebuild parameter dictionary with IDs as keys
parameters = {}
for name, param in config['parameters'].items():
    # check for duplicate ID
    if param['id'] in parameters:
        print(f"ERROR: \"{name}\" has the same ID as \"{parameters[param['id']]['name']}\"")
        sys.exit()

    parameters[param['id']] = param
    parameters[param['id']]['name'] = name

# build group blueprints
groups = {}
for group in config['groups']:
    param_ids = ['EMPTY_ID'] * 8
    for param in group['parameters']:
        # find the parameter with this name
        param_id = -1
        for id in parameters:
            if param['name'] == parameters[id]['name']:
                param_id = id
                break

        # make sure parameter exists
        if param_id == -1:
            print(f"ERROR in group \"{group['id']}\": no parameter with name \"{param['name']}\"")
            sys.exit()

        parameter = parameters[param_id]

        # fill parameter info
        parameter['group_id'] = group['id'] # NOTE: This will select the last instance of the parameter when it is in multiple groups. Make sure to fill out the YAML accordingly
        parameter['length'] = param['length']

        # fill parameter ids at starting position
        param_ids[param['start']] = parameter['name'].upper() + "_ID"

    groups[group['id']] = param_ids

data = {
    'buses': config['buses'],
    'modules': config['modules'],
    'parameters': parameters,
    'groups': groups,
    'commands': config['commands'],
    'errors': config['errors'],
    'param_structs': param_structs
}

for filename in filenames:
    print(f'Generating {filename}...')
    template_path = Path(f'templates/{filename}.jinja2')
    with open(template_path) as template_file:
        template = Template(template_file.read())
        output = template.render(data)
        output_path = Path(f'../{filename}')
        with open(output_path, "w") as output_file:
            output_file.write(output)

print('Done')