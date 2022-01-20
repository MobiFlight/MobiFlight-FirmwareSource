import os
Import("env")

# Get the version number from the build environment.
firmware_version = os.environ.get('VERSION', "")

# Clean up the version number
if firmware_version == "":
    # When no version is specified default to "0.0.1" for
    # compatibility with MobiFlight desktop app version checks.
    firmware_version = "0.0.1"

# Strip any leading "v" that might be on the version and
# any leading or trailing periods.
firmware_version = firmware_version.lstrip("v")
firmware_version = firmware_version.strip(".")

print(f'Using version {firmware_version} for the build')

# Append the version to the build defines so it gets baked into the firmware
env.Append(CPPDEFINES=[
    f'BUILD_VERSION={firmware_version}'
])

# The Pico board has to build differently on Linux so its environment name
# has _linux on the end. Get rid of it so the firmware filename just says
# "pico".
board_base_name = env["PIOENV"].replace("_linux", "")

# Set the output filename to the name of the board and the version
env.Replace(
    PROGNAME=f'mobiflight_{board_base_name}_{firmware_version.replace(".", "_")}')
