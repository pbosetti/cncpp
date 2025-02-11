# Usage notes

This RStudio Project allows to directly interface to the cncpp library to evaluate its algorithms. To do so, it compiles a C++ interface defined in the file `interface.cpp`.

For it to work, it must be compiled and executed on the same OS where cncpp is compiled. If you are working within WSL, it means that you need an RStudio instance installed on the WSL virtual machine.

## How to install RStudio in WSL

First, install the dependencies:

```bash
sudo apt install clang build-essential git cmake cmake-gui gh libssl-dev xsltproc r-base libclang-18-dev libnss3 libasound2t64 libcurl4-gnutils-dev libfontconfig-dev libtiff5-dev libharfbuzz-deb libfribidi-dev
```

Next, download and install RStudio:

```bash
wget https://download1.rstudio.org/electron/jammy/amd64/rstudio-2024.12.0-467-amd64.deb
sudo dpkg -i rstudio-2024.12.0-467-amd64.deb
rm rstudio-2024.12.0-467-amd64.deb
```

Finally, in RStudio install the packages tidyverse, Rcpp and plotly.