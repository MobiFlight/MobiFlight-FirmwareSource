Import("env")
import os

# Get the version number from the build environment. If no version
# number is supplied from the build system then use "dev" as the
# version as this is a local development build.
firmware_version = os.environ.get('VERSION', "")

print(firmware_version)

if firmware_version == "":
  firmware_version = "dev"
else:
  # Github versions are formatted like this: v1.0.0. To use in the filename
  # and keep consistent with pre-PlatformIO file names strip out the v
  # and convert the periods to underscores.
  firmware_version = firmware_version.replace("v", "").replace(".", "_")

print(f'Using version {firmware_version} for the build')

# Set the output filename to the name of the board and the version
env.Replace(PROGNAME=f'mobiflight_{env["PIOENV"]}_{firmware_version}')
