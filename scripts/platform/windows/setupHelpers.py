from pathlib import Path
import subprocess


def removeDependencyLink(linkPath: Path) -> None:
    if linkPath.exists():
        print(f"Deleting existing link at {linkPath}")
        if linkPath.is_symlink() or linkPath.is_file():
            linkPath.unlink()
        elif linkPath.is_dir():
            try:
                linkPath.rmdir()  # Remove empty directory
            except OSError as e:
                print(f"Failed to remove directory {linkPath}: {e}")


def createDependencyLink(linkPath: Path, targetDir: Path) -> str:
    # Windows specific symlink creation (requires admin or developer mode)
    # For simplicity, this example assumes junction for directories if symlink fails or not preferred
    # Python's os.symlink on Windows might require specific privileges or Python 3.8+ for unprivileged user symlinks.
    # Junctions are a common alternative for directories.
    try:
        # Attempt to create a symlink first
        linkPath.symlink_to(targetDir, target_is_directory=True)
    except OSError as e:
        print(f"Failed to create symlink: {e}. Attempting to create a junction...")
        try:
            subprocess.check_call(['cmd', '/c', 'mklink', '/J', str(linkPath), str(targetDir)], shell=True)
        except subprocess.CalledProcessError as sub_e:
            print(f"Failed to create junction: {sub_e}. Please ensure you have permissions or try running as administrator.")
            return ""
        except FileNotFoundError:
            print("'cmd' not found. Cannot create junction.") # Shouldn't happen on Windows
            return ""

    print(f"Successfully created link/junction at {linkPath} pointing to {targetDir}")
    return str(linkPath.resolve())

