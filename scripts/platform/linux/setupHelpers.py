import os
from pathlib import Path


def removeDependencyLink(linkPath: Path) -> None:
    if linkPath.exists():
        print(f"Deleting existing link at {linkPath}")
        if linkPath.is_symlink():
            linkPath.unlink()
        else:
            print(f"Warning: {linkPath} exists but is not a symlink.")

def createDependencyLink(linkPath: Path, target_dir: Path) -> str:
    try:
        linkPath.symlink_to(target_dir, target_is_directory=True)
        print(f"Successfully created symlink at {linkPath} pointing to {target_dir}")
    except OSError as e:
        print(f"Failed to create symlink: {e}. Check permissions and if target exists.")
        return ""
    return str(linkPath.resolve())

