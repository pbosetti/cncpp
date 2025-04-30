# R Wrapper for the C++ library

## How to use

If you are working on WSL, then you must remember that the cncpp_lib library that we develop is compiled under the Linux WSL machine. Therefore, you must also compile the R wrapper under WSL. In turn, this means that R and RStudio must run on WSL and you cannot use a Windows version of R or RStudio. 

To install R and RStudio on WSL:

1. Open WSL and run the following command to install R:
```bash
sudo apt install r-base
```
2. Install RStudio by downloading the latest version from the [RStudio website](https://posit.co/download/rstudio-desktop/). You can use the following command to download and install RStudio:
```bash
wget https://download1.rstudio.org/electron/jammy/amd64/rstudio-2024.12.1-563-amd64.deb
sudo apt install rstudio-2024.12.1-563-amd64.deb
```
3. After installation, you can start RStudio by running the following command:
```bash
rstudio
```
Then, in RStudio open the project in the `R` folder of the cncpp repository, open the `cncpp_test.qmd` file and follow the instructions in there.
