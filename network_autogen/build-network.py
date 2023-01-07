import sys
from pathlib import Path
import yaml
from jinja2 import Template

# python build-network.py configs/[filename].yaml

config_path = sys.argv[1]

if not Path(config_path).exists():
    print('ERROR: Invalid configuration path')
    sys.exit()

with open(config_path) as config_file:
    config = yaml.safe_load(config_file)

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

    # rebuild parameter dictionary with IDs as keys
    parameters = {}
    for name, value in config['parameters'].items():
        value['name'] = name
        parameters[value['id']] = value

    # build group blueprints
    groups = {}
    for id, value in config['groups'].items():
        blueprint = ['EMPTY'] * 8
        for param in value:
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
        'param_structs': param_structs
    }

    print('Generating GopherCAN_network.h...')
    template_path = Path('templates/GopherCAN_network.h.jinja2')
    with open(template_path) as template_file:
        template = Template(template_file.read())
        output = template.render(data)
        output_path = Path('../GopherCAN_network.h')
        with open(output_path, "w") as output_file:
            output_file.write(output)

    print('Generating GopherCAN_network.c...')
    template_path = Path('templates/GopherCAN_network.c.jinja2')
    with open(template_path) as template_file:
        template = Template(template_file.read())
        output = template.render(data)
        output_path = Path('../GopherCAN_network.c')
        with open(output_path, "w") as output_file:
            output_file.write(output)