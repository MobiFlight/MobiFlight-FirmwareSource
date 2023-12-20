import os

Import("env")

CUSTOMDEVICES_DIR = env.subst("$PROJECT_DIR/CustomDevices")
CUSTOMDEVICES_TAG = "0.2.0"

if not os.path.exists(CUSTOMDEVICES_DIR):
    print ("Cloning Mobiflight-CustomDevices repo ... ")
    env.Execute(f"git clone --depth 100  --branch {CUSTOMDEVICES_TAG} https://github.com/MobiFlight/MobiFlight-CustomDevices $PROJECT_DIR/CustomDevices")
else:
    print ("Checking for Mobiflight-CustomDevices repo updates ... ")
    env.Execute(f"git --work-tree=$PROJECT_DIR/CustomDevices --git-dir=$PROJECT_DIR/CustomDevices/.git pull origin {CUSTOMDEVICES_TAG} --depth 100")
