# 1 - Ensure the following programs are installed
# If STM_Programmer_CLI is not found, try adding it to your PATH variables (Should happen automatically)

STM32CubeProgrammer 2.19
Jinja2==3.1.2
PyYAML==6.0

# 2 - To auto generate hex and bin build outputs, add the following to the bottom of your CMakeLists.txt:

# Convert output to hex and binary
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
)

# 3 - Enable CAN bootloading in gophercan in your GopherCAN_config.h

#define ENABLE_BOOTLOADER

# 4A (VSCode)   - Add the following to your task.json 
#               - Press Ctrl+Shift+B and select "Run CAN Bootload" to auto build, start the bootloader, and run STM32_Programmer
#               - Modify the lines commented below to make sure the task detail matches your project

{
    "type": "shell",
    "label": "Build GopherCAN",
    "command": "python",
    "args": [
        "autogen.py",
        "configs/go4-24e.yaml"
    ],
    "options": {
        "cwd": "${workspaceFolder}/../gophercan-lib/network_autogen"
    },
    "problemMatcher": []
},
{
    "type": "shell",
    "label": "Build Gopher Sense",
    "command": "python",
    "args": [
        "gsense_auto_gen.py",
        "./../${workspaceFolderBasename}/${workspaceFolderBasename}_config.yaml"
    ],
    "options": {
        "cwd": "${workspaceFolder}/../Gopher_Sense"
    },
    "problemMatcher": []
},
{
    "type": "shell",
    "label": "Start CAN Bootloader",
    "command": "python",
    "args": [
        "../gophercan-lib/gcan_bootloader/src/gcan_bootloader.py",
# Replace "go4-24e" with the correct car config
        "go4-24e" // Set vehicle config here
    ],
    "options": {
        "cwd": "${workspaceFolder}"
    },
    "problemMatcher": []
},
{
    "type": "shell",
    "label": "CubeProg: Flash project (CAN)",
    "command": "STM32_Programmer_CLI",
    "args": [
        "-c",
        "port=CAN",
        "br=1000",
        "-d",
        "./build/Debug/${workspaceFolderBasename}.bin",
        "0x08000000"
    ],
    "options": {
        "cwd": "${workspaceFolder}"
    },
    "problemMatcher": []
},
{
    "label": "Run CAN Bootloader",
    "dependsOrder": "sequence",
    "dependsOn": [
        "Build GopherCAN",
        "Build Gopher Sense",
        "CMake: clean rebuild",
        "Start CAN Bootloader",
        "CubeProg: Flash project (CAN)"
    ],
    "problemMatcher": [],
    "group": {
        "kind": "build",
        "isDefault": true
    },
    "detail": "Run gopherCAN Bootloader"
}


# 4B (STM32CubeIDE) - Uncomment gcan_bootloader.py lines 64-110
#                   - Go to project properties: C/C++ Bulild->Settings->Tool Settings->MCU Post Build outputs, and enable "Convert to #                     binary file (-O binary)"
#                   - Run the following command from your project directory to run the bootloader, replacing "go4-25e" with the 
#                   - correct car path

python ../gophercan-lib/gcan_bootloader/src/gcan_bootloader.py go4-25e






