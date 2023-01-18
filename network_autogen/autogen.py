import sys
from pathlib import Path
import yaml
from jinja2 import Template

# This script imports a .yaml config file and generates resources for the GopherCAN library
# see configs/example.yaml for an example configuration

# Run: python autogen.py configs/[filename].yaml
# Generates:
# ../GopherCAN_network.h
# ../GopherCAN_network.c
# ../GopherCAN_names.h
# ../GopherCAN_names.c

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

type_names = {
    "UNSIGNED8" : "UINT_8",
    "UNSIGNED16" : "UINT_16",
    "UNSIGNED32" : "UINT_32",
    "UNSIGNED64" : "UINT_64",
    "SIGNED8" : "SINT_8",
    "SIGNED16" : "SINT_16",
    "SIGNED32" : "SINT_32",
    "SIGNED64" : "SINT_64",
    "FLOATING" : "FLOAT"
}

filenames = [
    'GopherCAN_network.h',
    'GopherCAN_network.c',
    'GopherCAN_names.h',
    'GopherCAN_names.c'
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
    if param['id'] in parameters:
        print(f"ERROR: \"{name}\" has the same ID as \"{parameters[param['id']]['name']}\"")
        print('Exiting...')
        sys.exit()
    else:
        parameters[param['id']] = param
        parameters[param['id']]['name'] = name
        
# add more error checking here
# TODO

# build group blueprints
groups = {}
for id, group in config['groups'].items():
    blueprint = ['EMPTY'] * 8
    for param in group:
        for i in range(param['length']):
            blueprint[param['start'] + i] = parameters[param['id']]['name']
    groups[id] = blueprint

data = {
    'buses': config['buses'],
    'modules': config['modules'],
    'parameters': parameters,
    'groups': groups,
    'commands': config['commands'],
    'errors': config['errors'],
    'param_structs': param_structs,
    'type_names': type_names
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