# cncppr

Rcpp wrappers for the CNCpp `Point` and `Block` classes.

Install from the repository root after CMake has fetched the C++ dependencies:

```sh
cmake -B build -G Ninja
R CMD INSTALL R
```

Example:

```r
library(cncppr)

p <- Point(1, 2, 3)
point_length(p)

b0 <- Block("N1 G00 X0 Y0 Z0")
block_parse(b0)

b1 <- Block("N2 G01 X100 Y100 Z0 F1000", b0)
block_parse(b1)

block_summary(b1)
head(block_walk(b1))
```
