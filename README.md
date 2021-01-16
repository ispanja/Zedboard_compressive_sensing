# Zedboard_compressive_sensing
Compressive sensing implemented on Zedboard and STM discovery board. Vivado design studio is used for block design of Zedboard. It enables SPI communication (.hdf file). Bit file is generated after successful syntesis and implementation. Petalinux is set on Zedboard. BOOT.bin file are executive programs, image.ub is OS image, whereas device tree is Linux filesystem. Linux kernel is set on zedboard. In order to activate SPI, device tree must be modified (.dtsi file). 
Discovery develompment board has data ready for compressive sensing. Data is sent using SPI protocol. Zedboard is set as master, while Discovery is set as slave. 
Reconstruction algorithm is based on C++ and is set on Linux filesystem on Zedboard. Reconstruction is based on matrix and vector operations.  
