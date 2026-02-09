# OPiL - an Open Processor-in-the-Loop framework

OPiL is an open processor-in-the-loop (PiL) framework aiming at defining the behavior of simulation and controller for processor-in-the-loop testing.  

PiL testing is extremely useful to verify the implementation of controllers on the actual embedded hardware, while simulating the plant. Because there are no real-time requirements, the implementation can be iterated in closed-loop to achieve an acceptable execution time. With PiL, it is possible to safely and easily test different implementation strategies, compiler settings, execution from flash/RAM, while verifying the controller in closed-loop.

This is the documentation of a C implementation of the OPiL framework. This implementation allows using PiL with any software and embedded controller capable of running C.

For a complete introduction to PiL and OPiL, as well as its implementation and an example, check [our online documentation](https://marcoguerreiro.gitlab.io/opil)!

# Introduction

The general PiL concept is shown in the figue below. On one side, the system, measurements and actuators are simulated by a simulation software. On the other side, the control algorithm is executed by the target embedded controller.

![PiL testing](/docs/images/intro/pil.png "PiL testing."){width=35%}

The general concept of the OPiL framework is based the figure below. The framework proposes blocks that are responsible for managing the data flow between simulation and controller. These blocks are made modular, so that it can easily support different simulation software and embedded controllers.

![OPiL scheme](/docs/images/intro/pil-opil.png "OPiL scheme."){width=35%}

# Example

This repository contains an example that can be run on Windows. The system is a DC-DC buck converter, and is simulated with the PLECS software. A state-feedback controller is implemented in C. PLECS and the controller exchange data through a local TCP/IP socket. 

Check our [quickstart guide](https://marcoguerreiro.gitlab.io/opil) for instructions on how to run the example.

# Documentation

For more information and discussion on OPiL, visit [our online documentation](https://marcoguerreiro.gitlab.io/opil).

# Citing
You can make a reference to this project by citing the related OPiL publication: 

```
@inproceedings{guerreiro2023,
  author    = {Marco {Guerreiro} and Wesley {Becker} and Pedro {dos Santos} and Steven {Liu}},
  booktitle = {Proc. IEEE 49th Annu. Conf. Ind. Electron. Soc.},
  title     = {An Open Processor-in-the-Loop Framework for Power Converter Control},
  year      = {2023},
  publisher = {IEEE},
}
```
