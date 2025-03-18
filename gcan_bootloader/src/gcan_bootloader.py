import os
import sys
import yaml
import subprocess

# main code
if (len(sys.argv) < 2):
    print('Need to provide a vehicle config from the network_autogen folder of gophercan-lib')
    print('Example: python select_car.py go4-23e')
    sys.exit()

# Get current working directory and gcan library path
proj_dir = os.getcwd()
main_dir = os.path.dirname(proj_dir)
dir_name = os.path.basename(proj_dir)

gcannon_path = os.path.join(main_dir,'gophercan-lib','network_autogen')
gcannon_config_file_path = os.path.join(gcannon_path,'configs',sys.argv[1]+'.yaml')

# Check for yaml config file in working directory
if not os.path.exists(os.path.join(proj_dir,dir_name + '_config.yaml')):
    print('Module config not found, run from a working project directory')
    sys.exit()
module_config_file_path = os.path.join(proj_dir,dir_name + '_config.yaml')

# Open module config yaml and extract name
with open(module_config_file_path, "r") as module_config_file:
    module_config = yaml.safe_load(module_config_file)

module_name = module_config.get("module_name", "").upper()
if not module_name:
    print("module_name not found in config file!")
    sys.exit()

# Open gcan config yaml and extract module id
with open(gcannon_config_file_path, "r") as gcannon_config_file:
    gcannon_config = yaml.safe_load(gcannon_config_file)

if module_name not in gcannon_config["modules"]:
    print(f"Module '{module_name}' not found in modules.yaml!")
    sys.exit()

module_info = gcannon_config["modules"][module_name]
module_id = module_info["id"]

bootloader_path = os.path.join(main_dir,'gophercan-lib','gcan_bootloader')
bootloader_start_path = os.path.join(bootloader_path,'bootloader_can_bridge.exe')

result = subprocess.run([bootloader_start_path, str(module_id)], capture_output=True, text=True)

# Print the program's output
print(result.stdout.strip())

# Check return code
if result == 1:
    print("Failed to start GCAN bootloader, aborting!")
    sys.exit()

sys.exit()

# Uncomment this section to auto run STM32_Programmer_CLI

# # Construct CLI command
# firmware_file = os.path.join(proj_dir, 'build', 'Debug', dir_name + '.bin')

# cmd = [
#     "STM32_Programmer_CLI",
#     "-c", 
#     "port=CAN",
#     "br=1000",
#     "-d", 
#     firmware_file,
#     "0x08000000"
# ]

# # Set correct console encoding before running the CLI
# os.system("chcp 850 > nul")  # Change to Windows CP850 (or try 437)

# process = subprocess.Popen(
#     cmd,
#     stdout=subprocess.PIPE,
#     stderr=subprocess.STDOUT,
#     text=True,
#     bufsize=1,
#     encoding="cp850",  # Adjust to match `chcp` output
#     errors="replace"
# )

# while True:
#     output = process.stdout.read(1)  # Read one character at a time
#     if output == "" and process.poll() is not None:
#         break
#     sys.stdout.write(output)
#     sys.stdout.flush()  # Ensure real-time updates

# sys.stdout.write("\n")  # Ensure a newline after completion
# sys.stdout.flush()

# process.wait()

# # Check exit code
# if process.returncode == 0:
#     print("Bootloading successful!")
# else:
#     print("Bootloading failed!")






