import os

Import("env")

CUSTOMDEVICES_DIR = env.subst("$PROJECT_DIR/CustomDevices")

if not os.path.exists(CUSTOMDEVICES_DIR):
    print ("Cloning Mobiflight-CustomDevices repo ... ")
    env.Execute("git clone --depth 100 https://github.com/MobiFlight/MobiFlight-CustomDevices $PROJECT_DIR/CustomDevices")
else:
    print ("Checking for Mobiflight-CustomDevices repo updates ... ")
    env.Execute("git --work-tree=$PROJECT_DIR/CustomDevices --git-dir=$PROJECT_DIR/CustomDevices/.git pull origin main --depth 100")
