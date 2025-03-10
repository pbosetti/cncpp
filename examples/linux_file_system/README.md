# Linux file system navigation

Linux provides powerful commands for navigating the file system efficiently. This guide covers the most essential commands.

## Linux vs. Windows file systems

| Feature          | Windows                                   | Linux                                           |
|-----------------|----------------------------------------|------------------------------------------------|
| Structure       | Drives (C:, D:, etc.) and folders      | Unified tree structure starting from root (/)  |
| Case Sensitivity | Not case-sensitive                     | Case-sensitive                                 |
| File Permissions | Simple (user accounts)                | Granular control (user, group, others)        |
| Common FS Types | NTFS                                  | Ext4 (most common), FAT32, NTFS (sometimes)    |
| Usability       | User-friendly, familiar interface     | Flexible, powerful for advanced users         |

## Essential navigation commands

| Command  | Description |
|----------|------------|
| `pwd`    | Prints the current directory. |
| `ls`     | Lists files and directories. |
| `cd`     | Changes the working directory. |
| `mkdir`  | Creates a new directory. |
| `rmdir`  | Removes an empty directory. |
| `cp`     | Copies files and directories. |
| `mv`     | Moves or renames files and directories. |
| `rm`     | Deletes files or directories. |
| `find`   | Searches for files and directories. |
| `tree`   | Displays directory structure in a tree format. |

## Command breakdown

### `pwd` - Print working directory

To print the current directory path:
```bash
pwd
```
**Output:** Current directory path (e.g., `/home/user/Documents`).

### `ls` - List files and directories

To list files and directories:
```bash
ls
ls -l   # Detailed view
ls -a   # Show hidden files
ls -lh  # Human-readable file sizes
```

### `cd` - Change directory

To navigate to a directory:
```bash
cd Downloads        # Move to 'Downloads' directory
cd /home/user/docs  # Absolute path
cd ..               # Move up one level
cd ~                # Go to home directory
```

### `mkdir` - Create a directory

To create a new directory:
```bash
mkdir NewFolder
mkdir Dir1 Dir2 Dir3  # Create multiple directories
```

### `rmdir` - Remove empty directory

To remove an empty directory:
```bash
rmdir OldFolder
rm -r OldFolder  # Remove non-empty directory
rm -rf OldFolder # Remove directory forcefully
```

### `cp` - Copy files and directories

To copy files and directories:
```bash
cp file.txt /home/user/docs/  # Copy file
cp -r Folder1 Folder2         # Copy entire directory
```

### `mv` - Move/rename files

To move or rename files:
```bash
mv file.txt /home/user/docs/  # Move file
mv oldname.txt newname.txt    # Rename file
```

### `rm` - Delete files and directories

To delete files and directories:
```bash
rm file.txt         # Delete file
rm -r FolderName    # Delete folder and its contents
```
**Warning 1:** Think twice before running this command.
**Warning 2:** `rm -rf /` permanently deletes everything!

### `find` - Search for files

Search for files by name or type:
```bash
find /home/user -name "document.txt"  # Find file by name
find /home/user -type f -name "*.txt"  # Find all .txt files
```

### `tree` - Show directory structure

Show directory structure in a tree format:
```bash
tree
```

If `tree` is not installed:
```bash
sudo apt install tree  # Debian/Ubuntu
sudo yum install tree  # CentOS/RHEL
```

## Shortcut Symbols

| Symbol | Description |
|--------|-------------|
| `~`    | Home directory shortcut. |
| `.`    | Current directory. |
| `..`   | Parent directory. |

## Conclusion

Mastering these commands makes navigating Linux efficient. Use `pwd` to check location, `cd` to move, and `ls` to list files. Shortcuts like `~`, `.`, and `..` speed up navigation. Tools like `find` and `tree` help with locating and visualizing files. Happy navigating!

---

# Introduction to Git

Git is a **distributed version control system (VCS)** used to track changes in source code. It allows multiple developers to collaborate, manage versions, and maintain a history of modifications efficiently.

## Why Use Git?

- **Version Control**: Keeps track of changes over time.
- **Collaboration**: Allows multiple people to work on the same project.
- **Backup & Recovery**: Prevents data loss.
- **Branching & Merging**: Enables working on multiple features in parallel.

## Installing Git

### Linux (Debian/Ubuntu)

```bash
sudo apt install git
```
### macOS (Homebrew)

```bash
brew install git
```
### Windows

Download from [git-scm.com](https://git-scm.com/).

## Basic Git Workflow

### Configure Git
```bash
git config --global user.name "Your Name"
git config --global user.email "your@email.com"
```

### Initialize a repository

```bash
git init
```
Creates a new Git repository in the current directory.

### Check status

```bash
git status
```
Displays the current state of the working directory.

### Add files to staging
```bash
git add filename   # Add a specific file
git add .          # Add all changes
```

### Commit changes
```bash
git commit -m "Commit message"
```
Saves the changes to the local repository.

### View commit history
```bash
git log
```

### Create and switch branches
```bash
git branch feature   # Create a new branch
git checkout feature # Switch to the branch
git switch feature   # Alternative method
```

### Merge branches
```bash
git checkout main
git merge feature
```

### Push to remote repository

```bash
git remote add origin https://github.com/user/repo.git
git push -u origin main
```

### 10. Pull changes from remote

```bash
git pull origin main
```

## Undoing changes

| Action | Command |
|--------|---------|
| Unstage a file | `git reset filename` |
| Undo last commit | `git reset --soft HEAD~1` |
| Hard reset (removes changes) | `git reset --hard HEAD~1` |

## Summary

| Command | Description |
|---------|------------|
| `git init` | Initialize a repository |
| `git status` | Check repository status |
| `git add` | Stage files |
| `git commit` | Commit changes |
| `git log` | View commit history |
| `git branch` | Manage branches |
| `git merge` | Merge branches |
| `git push` | Push to remote repository |
| `git pull` | Fetch and merge changes |
