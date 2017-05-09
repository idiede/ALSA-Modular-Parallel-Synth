# ALSA-Modular-Parallel-Synth [AMPS]
- Parallel Version of the ALSA Modular Synth [AMS]
- The AMS is a emeulation of a 70's style modular synth: http://alsamodular.sourceforge.net/ Author. M. Nagorni 

- This version parallelizes the callgraph of each individual voice of the AMS.
The C++ std::async library has been used to execute the parallel paths. 
Initial tests show that this version out performs the other versions. See: http://spectrum.library.concordia.ca/980575/ 
- Future work will be to implement a thread-pool solution and to use a lock-free queue and add more modules
For now however proof of concept has been completed.
- Installation is the same as the AMS but the C++ 2011 Library is required. Follow the README in the source_code folder and the INSTALL file for instructions on how to install.
