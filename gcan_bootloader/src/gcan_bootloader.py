import os
import sys
import yaml
import subprocess

# Verify car was selected by user
if (len(sys.argv) < 2):
    print('Need to provide a vehicle config from the network_autogen folder of gophercan-lib')
    print('Example: python select_car.py go4-25')
    sys.exit()

# Get current working directory and workspace dir
proj_dir = os.getcwd()
main_dir = os.path.dirname(proj_dir)
dir_name = os.path.basename(proj_dir)

# Get gophercan-lib and car config path
gcannon_path = os.path.join(main_dir,'gophercan-lib','network_autogen')
gcannon_config_file_path = os.path.join(gcannon_path,'configs',sys.argv[1]+'.yaml')

# Check for module config yaml file in project directory
if not os.path.exists(os.path.join(proj_dir,dir_name + '_config.yaml')):
    print('Module config not found, run from a working project directory')
    sys.exit()

# Get module config path
module_config_file_path = os.path.join(proj_dir,dir_name + '_config.yaml')

# Open module config yaml and extract module name
with open(module_config_file_path, "r") as module_config_file:
    module_config = yaml.safe_load(module_config_file)

module_name = module_config.get("module_name", "").upper()
if not module_name:
    print("module_name not found in config file!")
    sys.exit()

# Open gcan car config yaml and extract module id
with open(gcannon_config_file_path, "r") as gcannon_config_file:
    gcannon_config = yaml.safe_load(gcannon_config_file)

if module_name not in gcannon_config["modules"]:
    print(f"Module '{module_name}' not found in modules.yaml!")
    sys.exit()

module_info = gcannon_config["modules"][module_name]
module_id = module_info["id"]

# Get path to gcan_bootloader
bootloader_path = os.path.join(main_dir,'gophercan-lib','gcan_bootloader', 'src')
bootloader_start_path = os.path.join(bootloader_path,'bootloader_can_bridge.exe')

# Run the start bootloader script, sending the start command over GCAN
result = subprocess.run([bootloader_start_path, str(module_id)], capture_output=True, text=True)

# Print the program's output
print(result.stdout.strip())

# Check return code
if result == 1:
    print("Failed to start GCAN bootloader, aborting!")
    sys.exit()

####### Uncomment this section to auto run STM32_Programmer_CLI #######

# # Get path to project binary 
# firmware_file = os.path.join(proj_dir, 'build', 'Debug', dir_name + '.bin')

# # Construct STM32_Programmer_CLI command
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
# os.system("chcp 850 > nul")

# process = subprocess.Popen(
#     cmd,
#     stdout=subprocess.PIPE,
#     stderr=subprocess.STDOUT,
#     text=True,
#     bufsize=1,
#     encoding="cp850",
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
