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

print('Loading configuration...')
with open(config_path) as config_file:
    config = yaml.load(config_file)

print('Filling IDs...')
id = 1
for name in config['parameters']:
    config['parameters'][name]['id'] = id
    id += 1

print('Updating configuration...')
with open(config_path, 'w') as config_file:
    yaml.dump(config, config_file)

print('Done')