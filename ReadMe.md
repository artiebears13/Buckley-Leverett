# Buckley-Leverett equation solver for two-phase filtration (oil-water)
___________________

# Install

```bash
git clone git@github.com:artiebears13/Buckley-Leverett.git
```

```bash
cd Buckley-Leverett/BL_cpp
```

```bash
mkdir build
cd build
```
```bash
cmake ..
make
```

```bash
./BL_cpp
```
________________
# Result examples

program generates file 
```
../results/res_DD-MM-YYYY_HH-MM-SS
```
where after initial params goes solution:

```angular2html
x sw(x) so(x) ko(sw) kw(sw) pc(sw)
```

in file [BLproblem.cpp](BL_cpp/source/BLproblem.cpp)
you can change functions

 - ```double BLproblem::k_rw(double &sw)``` (line 5) 
 - ```double BLproblem::k_ro(double &sw)``` (line 10)
 -  ```double BLproblem::pc(double &sw)```  (line 14)
______________
# Example of solution
For default data:

### Saturation:

![saturation](BL_cpp/results/res_15-03-2023_18-54-14_SW.png)

### $$K_{rw}(sw)$$ and $$k_{ro}(sw)

![permeability](BL_cpp/results/res_15-03-2023_18-54-14_OFP.png)

