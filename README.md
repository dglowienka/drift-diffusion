# Drift-diffusion model for organic solar cells

The presented model is used in simulation of organic and perovskite solar cells for the PhD thesis. The following software is also shown for the sake of paper titled:

D. Głowienka and J. Szmytkowski 
Influence of excitons interaction with charge carriers on photovoltaic parameters in organic solar cells
Chemical Physics 503 (2018) 31–38

D. Głowienka, J. Szmytkowski 
Modeling of Transient Photocurrent in Organic Semiconductors Incorporating the Annihilation of Excitons on Charge Carriers
Acta Physica Polonica A 132 (2017) 397–400

D. Głowienka, J. Szmytkowski
Numerical Modeling of Exciton Impact in Two Crystalographic Phases of the Organo–lead
Halide Perovskite (CH 3 NH 3 PbI 3 ) solar cell,
Semiconductor Science and Technology 34 (2019) 035018

D. Głowienka, D. Zhang, F. D. Giacomo, M. Najafi, S. Veenstra, J. Szmytkowski, Y. Galagan
Role of Surface Recombination in the Interface of Cu:NiO x /CH 3 NH 3 PbI 3 Perovskite Solar
Cells,
submitted (2019)

D. Głowienka, F. D. Giacomo, M. Najafi, J. Szmytkowski, Y. Galagan
The Effect of Different Source of Bromide on Perovskite Cs 0.18 Fa 0.82 Pb(I 0.94 Br 0.06 ) 3 Solar Cells,
in preparation

##First use

The software is written in C++ language and tested on Linux Ubuntu 16.04.3. Compilation of the software is done in terminal with *make* command. As a result the *runner* exec file is created in */bin* folder. To run the code, put *./runner* command and use the provided commands to proceed wih calculations. The input parameters are in *config.xml* file.

One exemplary simulation is attached in */bin/001_default* folder. To calculate it one more time, you can use *./runner < inut.dat* command in terminal.

The detailed description of equations used in the numerical model can be found in *README.pdf*.

**Contact:**
Damian Głowienka
dglowienka@mif.pg.gda.pl
Gdańsk University of Technology
