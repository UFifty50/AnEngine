import os
import subprocess
import sys
from io import BytesIO
from pathlib import Path
from zipfile import ZipFile
from urllib.request import urlopen

import Utils


VULKAN_SDK = os.environ.get('VULKAN_SDK')
AE_VULKAN_VERSION = '1.3.268.0'       
VULKAN_SDK_INSTALLER_URL = f'https://sdk.lunarg.com/sdk/download/{AE_VULKAN_VERSION}/windows/vulkan_sdk.exe'
VULKAN_SDK_EXE_PATH = 'AnEngine/vendor/VulkanSDK/VulkanSDK.exe'
                                         
vkSDKdebugLibsURL = f'https://sdk.lunarg.com/sdk/download/{AE_VULKAN_VERSION}/windows/VulkanSDK-{AE_VULKAN_VERSION}-DebugLibs.zip'


def InstallVulkanSDK():                  
    print(f'Downloading {VULKAN_SDK_INSTALLER_URL} to {VULKAN_SDK_EXE_PATH}')
    Utils.DownloadFile(VULKAN_SDK_INSTALLER_URL, VULKAN_SDK_EXE_PATH)
    print("Done!")                       

    print("Running Vulkan SDK installer...")
    os.startfile(os.path.abspath(VULKAN_SDK_EXE_PATH))
    print("Re-run this script after installation")

def InstallVulkanPrompt():
    print("Would you like to install the Vulkan SDK?")

    install = None
    while install == None:
        reply = str(input('[Y/N]: ')).lower().strip()
        if reply[:1] == 'y':
            install = True
        elif reply[:1] == 'n':
            install = False

    if (install):
        InstallVulkanSDK()
    
    sys.exit()

def isSDKInstalled():
    if (VULKAN_SDK is None):
        print("Vulank SDK path couldn't be found in environment variables.")
        InstallVulkanPrompt()
        return False

    elif (AE_VULKAN_VERSION not in VULKAN_SDK):
        print(f"Located Vulkan SDK at {VULKAN_SDK}")
        print(f"You have the wrong Vulkan SDK version! AnEngine requires Vulkan v{AE_VULKAN_VERSION}.")
        InstallVulkanPrompt()
        return False

    print(f"Correct Vulkan SDK version found at {VULKAN_SDK}")
    return True


def areSDKDebugLibsInstalled():
    shadercdLib = Path(f"{VULKAN_SDK}/Lib/shaderc_sharedd.lib")

    if (not shadercdLib.exists()):
        print(f"No Vulkan SDK debug libs found in {shadercdLib.parent}")
        print(f"Downloading {vkSDKdebugLibsURL}")

        with urlopen(vkSDKdebugLibsURL) as resp:
            with ZipFile(BytesIO(resp.read())) as f:
                f.extractall("AnEngine/vendor/VulkanSDK")

    print(f"Vulkan SDK debug libs found in AnEngine/vendor/VulkanSDK/Lib")
    return True

def getIncludeDir():
    return f"{VULKAN_SDK}/Include"

def getLibDir():
    return f"{VULKAN_SDK}/Lib"

def getDebugLibDir():
    return f"{VULKAN_SDK}/Lib"
