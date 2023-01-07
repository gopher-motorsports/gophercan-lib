import sys
from pathlib import Path
import yaml
from jinja2 import Template

# python build-names.py configs/[filename].yaml

config_path = sys.argv[1]

if not Path(config_path).exists():
    print('ERROR: Invalid configuration path')
    sys.exit()

with open(config_path) as config_file:
    config = yaml.safe_load(config_file)

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

    data = {
        'parameters': config['parameters'],
        'type_names': type_names
    }

    print('Generating GopherCAN_names.h...')
    template_path = Path('templates/GopherCAN_names.h.jinja2')
    with open(template_path) as template_file:
        template = Template(template_file.read())
        output = template.render(data)
        output_path = Path('../GopherCAN_names.h')
        with open(output_path, "w") as output_file:
            output_file.write(output)

    print('Generating GopherCAN_names.c...')
    template_path = Path('templates/GopherCAN_names.c.jinja2')
    with open(template_path) as template_file:
        template = Template(template_file.read())
        output = template.render(data)
        output_path = Path('../GopherCAN_names.c')
        with open(output_path, "w") as output_file:
            output_file.write(output)