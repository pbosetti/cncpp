# Introduction

The whole environment is based on MADS to allow inter-process communication.

The system is made of:

1. the MADS `broker` process, which distributes the messages to the other processes and provides a common configuration file
2. the `cncpp` process, which implements the CNC kernel, communicates setpoints to the machine simulator and receives feedback from it
3. a machine simulator, implements the decoupled dynamics of three linear axes implemented as a FMU unit created in Simulink.
4. a machine tool viewer and dashboard, which shows the machine tool and the current state of the axes, also plotting state variables in real time

The viewer requires a 3D-accelerated graphical environment, so if the system is to be run in Windows+WSL, components 1.--3. can run in the WSL, and component 4. is to be run on Windows.

On a Linux or macOS machine, all 4 components can run on the same machine.

> Note that nothing prevents to run all four components on 4 different networked computers.

## MADS installation

Installin MADS is easy anc common to all platforms: head to <https://github.com/pbosetti/mads/releases> and download the latest release for your platform (at least `v3.0.0-beta5`). Follow detailed instructions hereafter for installing it.


# On Windows

## Enabling scripts and custom executables

By default, Windows 11 refuses to run PowerShell scripts (e.g. the Python venv `activate` script used below) and may block executables downloaded from the Internet. To lift these limits, open the Terminal app (PowerShell) **once** and type:

```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

and answer `Y` when asked. This allows local scripts to run, for the current user only.

Files downloaded from the Internet are also marked as *blocked*. After unzipping a downloaded folder, unblock all its files with:

```powershell
Get-ChildItem -Path <unzipped folder> -Recurse | Unblock-File
```

If, when launching an `.exe`, a blue *"Windows protected your PC"* (SmartScreen) window appears, click **More info** and then **Run anyway**. If the executable is still blocked, open *Settings > Privacy & security > Windows Security > App & browser control > Smart App Control* and set it to **Off** (note: once turned off, it cannot be turned back on without reinstalling Windows).

## MADS installation

Just launch the `.exe` installer and follow the instructions. Then veryfy the installation by opening the Terminal app and typing:

```powershell 
mads -v
```

## The machine tool viewer

Go to <https://github.com/mads-net/mt_plugin/releases> and download the latest version as a zip folder. Unzip it and move to a folder where you have easy access: you'll need to open a Terminal shell into that.

The viewer uses [Rerun](https://rerun.io/) which, in turn, requires a Python3 installation. Open a Terminal shell into the `mt_plugin` folder unzipped above and type:

```powershell
python3 -mvenv venv
.\venv\Scripts\activate
pip install rerun-sdk==0.43.2
```

Then type `rerun` and verify that its window opens.



## The machine tool simulator

Go to <https://github.com/mads-net/fmu_agent/releases> and copy the URL of the latest release for Linux (a `.zip` file, pick the `x86_64` version). Then open a WSL shell in the cncpp project root folder and type:

```bash
wget <paste the download url>
unzip <downloaded file>.zip
mv <dwnloaded folder> FMU_agent
```

Now the CNCPP project must also contain:

```
FMU_agent
├── bin
│   └── mads-fmu
└── models
    └── fmu
        ├── DoubleMassSpringDamper.fmu
        ├── linear_axis.fmu
        └── machine_tool.fmu
```

where `mads-fmu` is the executable, `machine_tool.fmu` is the FMU model for the machine tool.


# On Unix

## MADS installation

On Linux: copy the `.deb` file URL, then open a shell and type:

```bash
wget <paste the download url>
sudo dpkg -i <downloaded file>.deb
```

On macOS: download the `.dmg` file, open it and drag the `usr` folder to your user's homem folder. Then open a shell and type:

```bash
xattr -rd com.apple.quarantine ~/usr
echo 'export PATH=$HOME/usr/local/bin:$PATH' >> ~/.zshrc
source ~/.zshrc
```

## The machine tool viewer

Go to <https://github.com/mads-net/mt_plugin/releases> and download the latest version as a zip folder. Unzip it and move to a folder where you have easy access: you'll need to open a Terminal shell into that.

On macOS, remove the protection (once) by typing:

```bash
xattr -rd com.apple.quarantine <unzipped folder>
```

The viewer uses [Rerun](https://rerun.io/) which, in turn, requires a Python3 installation. Open a Terminal shell into the `mt_plugin` folder unzipped aboveand type:

```bash
python3 -mvenv .venv
source .venv/bin/activate
pip install rerun-sdk==0.43.2
```

Then type `rerun` and verify that its window opens.


## The machine tool simulator

Go to <https://github.com/mads-net/fmu_agent/releases> and copy the URL of the latest release for your OS and platform. Then open a shell in the cncpp project root folder and type:

```bash
wget <paste the download url>
unzip <downloaded file>.zip
mv <dwnloaded folder> FMU_agent
```

On macOS, remove the protection (once) by typing:

```bash
xattr -rd com.apple.quarantine FMU_agent
```


Now the CNCPP project must also contain:

```
FMU_agent
├── bin
│   └── mads-fmu
└── models
    └── fmu
        ├── DoubleMassSpringDamper.fmu
        ├── linear_axis.fmu
        └── machine_tool.fmu
```

where `mads-fmu` is the executable, `machine_tool.fmu` is the FMU model for the machine tool.