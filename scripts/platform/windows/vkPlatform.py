import os
from pathlib import Path
from io import BytesIO
from zipfile import ZipFile
from urllib.request import urlopen

def installVulkanSDK(sdk_file_path: Path, vulkan_sdk_download_dir: Path, ae_vulkan_version: str):
    print("Running Vulkan SDK installer...")
    try:
        os.startfile(os.path.abspath(str(sdk_file_path))) # os.startfile is Windows-specific
        print("Please follow the installer prompts.")
        print("Re-run the main script or your project's configuration script after installation is complete.")
    except Exception as e:
        print(f"Failed to start Vulkan SDK installer: {e}")
        print(f"Please try running {os.path.abspath(str(sdk_file_path))} manually.")

# def areSDKDebugLibsInstalled(sdk_path: Path) -> bool:
#     lib_dir = sdk_path / "Lib"
#     shadercdLib = lib_dir / "shaderc_sharedd.lib"
#
#     if not shadercdLib.exists():
#         print(f"Vulkan SDK debug lib (shaderc_sharedd.lib) not found in {lib_dir}")
#         if vk_sdk_debug_libs_url:
#             print(f"Attempting to download debug libraries from {vk_sdk_debug_libs_url}")
#             try:
#                 extract_target_dir = vulkan_sdk_download_dir
#                 extract_target_dir.mkdir(parents=True, exist_ok=True)
#
#                 with urlopen(vk_sdk_debug_libs_url) as resp:
#                     with ZipFile(BytesIO(resp.read())) as f:
#                         f.extractall(extract_target_dir)
#                 print(f"Vulkan SDK debug libs downloaded and extracted into {extract_target_dir}.")
#
#                 if not shadercdLib.exists(): # Re-check after extraction
#                      print(f"Debug lib {shadercdLib} still not found after download. Ensure it's in the expected SDK path.")
#                      print(f"Debug libs were extracted to {extract_target_dir}. Your VULKAN_SDK is {sdk_path}.")
#                      print("You may need to manually integrate these files or adjust your VULKAN_SDK path.")
#                      return False
#             except Exception as e:
#                 print(f"Failed to download or extract Vulkan SDK debug libraries: {e}")
#                 return False
#         else:
#             print("Vulkan SDK debug libraries URL is not configured for Windows (VK_SDK_DEBUG_LIBS_URL is None).")
#             return False
#     print(f"Vulkan SDK debug libs (e.g., shaderc_sharedd.lib) presumed present in {lib_dir}")
#     return True

