Import("env")
import os

# Get the version number from the build environment.
core_firmware_version = os.environ.get('VERSION', "")
custom_firmware_version = env.GetProjectOption("custom_firmware_version")

# Clean up the version number
if core_firmware_version == "":
  # When no version is specified default to "0.0.1" for
  # compatibility with MobiFlight desktop app version checks.
  core_firmware_version = "0.0.1"

# Set the output filename to the name of the board and the version
if custom_firmware_version == "":
  custom_firmware_version = core_firmware_version

# Strip any leading "v" that might be on the version and
# any leading or trailing periods.
core_firmware_version = core_firmware_version.lstrip("v")
core_firmware_version = core_firmware_version.strip(".")

print(f'Using version {core_firmware_version} for the build')

# Append the version to the build defines so it gets baked into the firmware
env.Append(CPPDEFINES=[
    f'CORE_BUILD_VERSION={core_firmware_version}'
  ])
if custom_firmware_version == "":
  env.Append(CPPDEFINES=[
    f'BUILD_VERSION={core_firmware_version}'
  ])
else:
  env.Append(CPPDEFINES=[
    f'BUILD_VERSION={custom_firmware_version}'
  ])

# Set the output filename to the name of the board and the version
if custom_firmware_version == "":
  env.Replace(PROGNAME=f'{env["PIOENV"]}_{core_firmware_version.replace(".", "_")}')
else:
  env.Replace(PROGNAME=f'{env["PIOENV"]}_{custom_firmware_version.replace(".", "_")}')
