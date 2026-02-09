.. _index:

Welcome to OPiL's documentation!
================================

OPiL is an open processor-in-the-loop (PiL) framework aiming at defining the behavior of simulation and controller for processor-in-the-loop testing.  

This is the documentation of a C implementation of the OPiL framework. This implementation allows using PiL with any simulation software supporting DLLs and embedded controllers running C. Check out the `project's official repository <https://gitlab.rhrk.uni-kl.de/lrs/opil>`_ for the source code.

The documentation includes:

- An introduction to PiL and OPiL (see :ref:`sec-intro`)
- A quickstart guide on how to use this C implementation (see :ref:`sec-quickstart`)
- A description of its C implementation (see :ref:`sec-c-implementation`)
- An example (see :ref:`sec-plecs-example`)

About PiL
---------

Processor-in-the-loop (PiL) testing is extremely useful to verify the implementation of controllers on the actual embedded hardware, while simulating the plant. Some of the advantages and possibilites of PiL testing are:

- Verify the closed-loop system without real-time requirements. After the controller is verified, the implementation can be iterated in closed-loop to achieve real-time capabilities. This is specially important when trying to implement advanced control strategies, such as model predictive control. At first, the correct implementation is verified, and then the implementation is optimized.

- Safely and easily test different implementation strategies, compiler settings, execution from flash/RAM, while verifying the controller in closed-loop.

- Debug the control algorithm at each sampling instant, while the system is halted.  This allows debugging the control algorithm line by line, without losing the states of the system. 

- Verify integration of complex embedded targets. For example, in a system  consisting of a DSP and an FPGA, where the control algorithm is executed partially on the DSP and partially on the FPGA, it is possible to verify their correct integration, while also verifying timing requirements.

- Decouples control from hardware and leads to a more modular controller design. A disadvantage of PiL testing is that the input and output peripherals of the embedded target are not tested. However, PiL creates clear boundaries between hardware and software, which actually allows executing the control algorithm in any embedded hardware, as long as the proper interfaces are implemented for the target hardware. 

- Cheaper when compared to hardware-in-the-loop (HiL). Although HiL is much faster than PiL (since HiL is real-time), PiL is much cheaper, since only a simulation software and the embedded target are required. 

About OPiL
----------

PiL is only partially supported by simulation software, such as PLECS and Matlab. OPiL is an open processor-in-the-loop framework that defines the behavior of simulation and controller for PiL testing, aiming at extending PiL support to a wider range of simulation software and targets.

This C implementation allows PiL testing with virtually any software supporting DLLs and controllers running C code.

.. toctree::
	:maxdepth: 2
	:caption: Contents
	
	intro
	quickstart
	c_implementation
	plecs_example

Support OPiL
------------

You can support OPiL by citing the related OPiL publication:

.. code:: latex

   @inproceedings{guerreiro2023,
     author    = {Marco {Guerreiro} and Wesley {Becker} and Pedro {dos Santos} and Steven {Liu}},
     booktitle = {Proc. IEEE 49th Annu. Conf. Ind. Electron. Soc.},
     title     = {An Open Processor-in-the-Loop Framework for Power Converter Control},
     year      = {2023},
     publisher = {IEEE},
   }

