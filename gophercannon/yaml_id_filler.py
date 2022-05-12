import argparse
import ruamel.yaml

parser = argparse.ArgumentParser(prog='yaml_id_filler.py',
    description='Program to fill in IDs for the YAML configuration')

parser.add_argument('network_file',
                    metavar='network file',
                    type=str,
                    help='the network description file')

args = parser.parse_args()
filename = args.network_file

print("Welcome to the YAML ID Filler")
print("Loading network definition file \"{0}\".".format(filename))
config, ind, bsi = ruamel.yaml.util.load_yaml_guess_indent(open(filename))

parameters = config['parameters']

counter = 1

for param in parameters:
    parameters[param]['id'] = counter
    counter = counter + 1

yaml = ruamel.yaml.YAML()
yaml.indent(mapping=4, sequence=4, offset=2)
with open(filename, 'w') as fp:
    yaml.dump(config, fp)
    
print("File dump complete")

