# MADS links

## Install MADS

Fetch the latest MADS release here: <https://git.new/mads>

---

On Linux, install as:

```bash
wget <URL copied from the MADS release page>
sudo dpkg -i <downloaded file>
```

---

On MacOS, install as:

```bash
wget <URL copied from the MADS release page>
chmod a+x <downloaded file>
./<downloaded file> --prefix=${HOME} --skip-license
```

then add the following line to your `.bashrc` or `.zshrc` (`nano ~/.bashrc` or `nano ~/.zshrc`):

```sh
export PATH="${HOME}/usr/local/bin:$PATH"
```

then restart your terminal.

---

On Windows, download the .exe and run it. It will install MADS and add it to your PATH.

On Windows we are using some agents within WSL, some within Windows itself, so you need to install MADS in both environments. In WSL, you can follow the Linux instructions above. 


## MADS Visual Studio Code Extension

In the Visual Studio Code marketplace, search for "MADSCode" and install the extension. This will provide direct access and utilities for working with MADS.

The extension also provides links to MADS guides and API documentation.

## Machine tool simlator

We are using two MADS agents for simulating the machine tool:

* [mt_plugin](https://github.com/mads-net/mt_plugin): it uses Rerun to view and log the machine tool state in action.
* [FMU_agent](https://github.com/mads-net/FMU_agent): it uses the Functional Mock-up Interface (FMI) to simulate the machine tool dynamics.

You are also needing an installed version of Python3 (both in Windows and WSL).

The `mt_plugin` is available as a compiled binary: download it from the Releases section in the GitHub page, then unzip it in a handy position. There is no installation process. Follow the instructions in the `README.md` document.

The `FMU_agent`, conversely, must be compiled and installed. Follow the instructions in the `README.md` document, which involves running a few commands in the terminal.