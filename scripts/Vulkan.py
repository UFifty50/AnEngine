import os
import sys
from io import BytesIO
from pathlib import Path

import requests
from zipfile import ZipFile
from urllib.request import urlopen

import Utils
from Utils import Platform, UnsupportedPlatformError


VULKAN_SDK = os.environ.get('VULKAN_SDK')
VULKAN_SDK_DOWNLOAD_DIR = Path('AnEngine/vendor/VulkanSDK')

if sys.platform.startswith('win'):
    from platform.windows import vkPlatform

    VULKAN_VERSION = requests.get("https://vulkan.lunarg.com/sdk/latest/windows.json").json().get('windows')
    VULKAN_SDK_INSTALLER_URL = f'https://sdk.lunarg.com/sdk/download/{VULKAN_VERSION}/linux/vulkan_sdk.exe'
    VULKAN_SDK_FILENAME = 'vulkan_sdk.exe'

elif sys.platform.startswith('linux'):
    from platform.linux import vkPlatform

    VULKAN_VERSION = requests.get("https://vulkan.lunarg.com/sdk/latest/linux.json").json().get('linux')
    VULKAN_SDK_INSTALLER_URL = f'https://sdk.lunarg.com/sdk/download/{VULKAN_VERSION}/linux/vulkan_sdk.tar.xz'
    VULKAN_SDK_FILENAME = 'vulkan_sdk.tar.xz'

else:
    raise UnsupportedPlatformError(f"Unsupported platform: {sys.platform}", Platform.WINDOWS | Platform.LINUX)

VULKAN_SDK_FILE_PATH = VULKAN_SDK_DOWNLOAD_DIR / VULKAN_SDK_FILENAME


def InstallVulkanSDK():
    if not VULKAN_SDK_INSTALLER_URL or not VULKAN_SDK_FILE_PATH:
        print(f"Vulkan SDK installation is not supported on this platform ({sys.platform}) or configuration is missing.")
        return

    if not vkPlatform:
        print(f"Vulkan SDK installation helpers not available for platform {sys.platform}.")
        return

    VULKAN_SDK_DOWNLOAD_DIR.mkdir(parents=True, exist_ok=True)

    print(f'Downloading {VULKAN_SDK_INSTALLER_URL} to {VULKAN_SDK_FILE_PATH}')
    Utils.DownloadFile(VULKAN_SDK_INSTALLER_URL, str(VULKAN_SDK_FILE_PATH))
    print("Download Done!")

    # Call platform-specific installation
    vkPlatform.installVulkanSDK(VULKAN_SDK_FILE_PATH, VULKAN_SDK_DOWNLOAD_DIR, VULKAN_VERSION)

def InstallVulkanPrompt():
    print("Would you like to install the Vulkan SDK?")
    install = None
    while install is None:
        reply = str(input('[Y/N]: ')).lower().strip()
        if reply and reply[0] == 'y':
            install = True
        elif reply and reply[0] == 'n':
            install = False
        else:
            print("Invalid input. Please enter Y or N.")

    if install:
        InstallVulkanSDK()
        return True # Indicates an installation attempt was made or user was guided
    return False # Indicates user chose not to install


def isSDKInstalled():
    global VULKAN_SDK # VULKAN_SDK is a global read at the start of the script

    if VULKAN_SDK is None:
        print("Vulkan SDK path couldn't be found in environment variables (VULKAN_SDK).")
        if InstallVulkanPrompt():
            # User attempted install. Advise re-run as VULKAN_SDK won't be updated in this session automatically.
            print("Please ensure the VULKAN_SDK environment variable is set (e.g., by sourcing setup-env.sh on Linux or via installer on Windows) and re-run the script.")
            return True
        else: # User chose not to install
            return False

    sdk_path_obj = Path(VULKAN_SDK)
    if not sdk_path_obj.is_dir():
        print(f"VULKAN_SDK is set to '{VULKAN_SDK}', but this path does not exist or is not a directory.")
        # Optionally, offer to guide installation again
        # if InstallVulkanPrompt(): ...
        return False

    # Version check based on path string (heuristic)
    # Assumes VULKAN_SDK path (set by user or setup-env.sh) contains the version string.
    # e.g., /opt/VulkanSDK/1.3.268.0/x86_64 or C:\\VulkanSDK\\1.3.268.0
    if VULKAN_VERSION not in VULKAN_SDK:
        print(f"Warning: Located Vulkan SDK at {VULKAN_SDK}")
        print(f"Your VULKAN_SDK path does not seem to explicitly contain the expected version string '{VULKAN_VERSION}'.")
        print(f"AnEngine requires Vulkan v{VULKAN_VERSION}.")
        print("Please ensure your VULKAN_SDK environment variable points to the correct SDK version installation.")
        # This is a warning, not necessarily a failure if the SDK is otherwise functional.
        # A more robust check would involve querying `vulkaninfo` or a version file.

    print(f"Vulkan SDK found at {VULKAN_SDK} (version string '{VULKAN_VERSION}' presence in path: {'yes' if VULKAN_VERSION in VULKAN_SDK else 'no/ambiguous'}).")
    return True


# def areSDKDebugLibsInstalled():
#     if not VULKAN_SDK:
#         print("VULKAN_SDK environment variable not set. Cannot check for debug libraries.")
#         return False
#
#     if not vkPlatform:
#         print(f"Vulkan SDK debug library check not available for platform {sys.platform}.")
#         return False # Or True, depending on desired behavior for unsupported platforms
#
#     sdk_path = Path(VULKAN_SDK)
#
#     if sys.platform.startswith('win'):
#         return vkPlatform.areSDKDebugLibsInstalled(sdk_path, VK_SDK_DEBUG_LIBS_URL, VULKAN_SDK_DOWNLOAD_DIR)
#     elif sys.platform.startswith('linux'):
#         return vkPlatform.areSDKDebugLibsInstalled(sdk_path)
#     else:
#         print(f"Debug library check not implemented for platform {sys.platform}.")
#         return True # Defaulting to true for unhandled platforms to be less disruptive.

def getIncludeDir(module=""):
    if VULKAN_SDK:
        sdk_path = Path(VULKAN_SDK)
        if sdk_path.is_dir():
            include_path = sdk_path / "include"
            if module:
                return str(include_path / module)
            return str(include_path)

    print("Warning: VULKAN_SDK not set. Using local include directory.")
    return str(Path(f'AnEngine/vendor/VulkanSDK/{VULKAN_VERSION}/x86_64/include/') / module)

def getLibDir():
    libDirName = "Lib" if sys.platform.startswith('win') else "lib"
    if VULKAN_SDK:
        sdk_path = Path(VULKAN_SDK)
        if sdk_path.is_dir():
            return str(sdk_path / libDirName)

    print("Warning: VULKAN_SDK not set. Using local include directory.")
    return str(Path(f'AnEngine/vendor/VulkanSDK/{VULKAN_VERSION}/x86_64/{libDirName}'))

def getDebugLibDir():
    # For Vulkan SDK, debug and release libraries are often in the same directory.
    # On Windows, .lib files for debug may have 'd' suffix or similar, but reside in same "Lib" folder.
    # On Linux, .so files in "lib" may have debug symbols included or stripped.
    # Separate debug symbol files (.debug) might exist but are not typically linked against directly.
    return getLibDir()


if __name__ == '__main__':
    if isSDKInstalled():
      #  areSDKDebugLibsInstalled()
        print(f"Include Dir: {getIncludeDir()}")
        print(f"Lib Dir: {getLibDir()}")
    else:
        print("Vulkan SDK not configured.")
