Import("env")
import os, zipfile, shutil
from pathlib import Path

zip_file_name = 'Mobiflight-Connector'
build_path = './_build'
build_path_fw = build_path + '/firmware'
build_path_json = build_path + '/Boards'
distrubution_path = './_dist'
source_folders = ['./_Boards']

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

# The community devices have a slightly different version where the
# core firmware version is defined within there .ini file
# For the core FW both versions are always the same
core_firmware_version = firmware_version

print(f'Using version {firmware_version} for the build')
print(f'Using version {core_firmware_version} as core version')

# Append the version and core version to the build defines so it gets baked into the firmware
env.Append(CPPDEFINES=[
  f'BUILD_VERSION={firmware_version}', f'CORE_BUILD_VERSION={core_firmware_version}'
])

# Set the output filename to the name of the board and the version
env.Replace(PROGNAME=f'{env["PIOENV"]}_{firmware_version.replace(".", "_")}')


def copy_fw_files (source, target, env):
    fw_file_name=str(target[0])

    if os.path.exists(build_path_fw) == False:
        os.makedirs(build_path_fw)
    if os.path.exists(build_path_json) == False:
        os.makedirs(build_path_json)

    if fw_file_name[-3:] == "bin":
        fw_file_name=fw_file_name[0:-3] + "uf2"

    # Copy build FW file
    shutil.copy(fw_file_name, build_path_fw)

    # Copy reset/uf2/json files
    file_extension = '.hex'
    copy_files_by_extension(source_folders, build_path_fw, file_extension)
    file_extension = '.uf2'
    copy_files_by_extension(source_folders, build_path_fw, file_extension)
    file_extension = '.json'
    copy_files_by_extension(source_folders, build_path_json, file_extension)

    # Create ZIP file and add files from distrubution folder
    zip_file_path = distrubution_path + '/' + zip_file_name + '_' + firmware_version + '.zip'
    createZIP(build_path, zip_file_path, zip_file_name)


def copy_files_by_extension(source_folders, target_folder, file_extension):
    for source_folder in source_folders:
        for root, dirs, files in os.walk(source_folder):
            for file in files:
                if file.endswith(file_extension):
                    source_path = os.path.join(root, file)
                    target_path = os.path.join(target_folder, file)
                    shutil.copy2(source_path, target_path)


def createZIP(original_folder_path, zip_file_path, new_folder_name):
    if os.path.exists(distrubution_path) == False:
        os.mkdir(distrubution_path)
    with zipfile.ZipFile(zip_file_path, 'w') as zipf:
        for root, dirs, files in os.walk(original_folder_path):
            for file in files:
                # Create a new path in the ZIP file
                new_path = os.path.join(new_folder_name, os.path.relpath(os.path.join(root, file), original_folder_path))
                # Add the file to the ZIP file
                zipf.write(os.path.join(root, file), new_path)


env.AddPostAction("$BUILD_DIR/${PROGNAME}.hex", copy_fw_files)
env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", copy_fw_files)
