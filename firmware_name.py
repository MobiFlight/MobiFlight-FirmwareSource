Import("env")
import os

# Get the version number from the build environment.
firmware_version = os.environ.get('VERSION', "")

# Clean up the version number
if firmware_version == "":
  # When no version is specified default to "0.0.1" for
  # compatibility with MobiFlight desktop app version checks.
  firmware_version = "0.0.1"
else:
  # Github versions are formatted like this: v1.0.0. The version
  # comes into the script with the v already removed so for filenames
  # all that has to happen is the periods get replaced with underscores.
  firmware_version = firmware_version.replace(".", "_")

print(f'Using version {firmware_version} for the build')

# Set the output filename to the name of the board and the version
env.Replace(PROGNAME=f'mobiflight_{env["PIOENV"]}_{firmware_version}')
