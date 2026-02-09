.. _sec-c-implementation:

C implementation
================

This section discusses a C implementation of the main core of the OPiL framework, as well as implementation of the additional modules required by OPiL. The source code is available at the `project's official repository <https://gitlab.rhrk.uni-kl.de/lrs/opil>`_. For an example on how to use OPiL with a simulation software and a controller, see :ref:`sec-plecs-example`.

Communication with sockets
--------------------------

Network sockets provide many primitives that are very useful when implementing a PiL scheme. Sockets work in a client-server model. In this model, the server opens a socket and waits for incoming connections. The client then connects to the server and once the connection is established, both sides can exchange data. At any moment, either side can close the connection, while notifying the other side. Everything is handled by the socket protocol.

This type of communication fits PiL very well. The target creates a server socket, and waits for connections. The host (simulation) creates a client socket, and connects to the server socket created by the target. While the simulation is running, both host and target can exchange data through this connection. When the simulation ends, it can simply close its client socket, and notify the target that the connections is closed.

When discussing implementation of the communication interface, it is assumed that sockets are used, where the target creates a server socket, and the host creates a client socket.

OPiL host
-------------

OPiL host is implemented in four main functions, which are documented in the |opilhost_h| file. 

The ``opilhostInitialize`` function initializes the framework on the host side. Internally, all callbacks that are related to the communication and simulation interfaces are set. Thus, to initialize the framework, these callbacks need to be supplied to the initialization function. In an application, this function needs to be called just once, when the simulation starts.

The initalize function takes as parameters two structures that need to be filled with all required callbacks. A description of these functions, the callbacks and the signatures can be found in the |opilhost_h| file. :ref:`sec-c-implementation-running-opil-host` shows how these structures can be filled, along with some implementation examples. 

Once initialized, the host is ready to be used. The ``opilhostConnectToTarget`` is used to connect to the target. This function returns 0 if the connection was successful. Any other value indicates that connecting to the target  was not possible. In an application, this function would be called jut once when the simulation starts. If connection to the client fails, the simulation can be terminated, or the control signals can be set to some default value throughout the simulation.

Once the connection is established, ``opilhostExchangeDataTarget`` is used to execute the state machine that loads data from the simulation, exchanges data with the target, and finally updates data in the simulation. This function should be executed only once per control period, and it represents one execution period of the controller.

When the simulation ends, the function ``opilhostDisconnectFromTarget`` needs to be called, in order to signal the target that the simulation is over.

.. _sec-c-implementation-example-comm:

Communication interface
^^^^^^^^^^^^^^^^^^^^^^^

Implementation of the communication interface requires implementing four functions: open connection, close connection, send, and receive. The signatures and behavior of these functions are documented in |opilhost_h|. 

An implementation of the communication interface assuming network sockets can be found in the ``comm`` folder inside the ``OPiL`` project. The documentation for each function is available in the header file, and their implementations for different platforms are in the subfolders inside the ``comm`` folder. A brief description of their behavior is given here.

The ``hostCommSockOpenConnection`` function creates a client socket on the host side, and attempts to connect to a  server socket (expected to be already createad by the target). 

The ``hostCommSockSendData`` and ``hostCommSockReceiveData`` functions are used to send and receive data. All data is sent and received through the socket previously created when ``hostCommSockOpenConnection`` is called.

Last, ``hostCommSockCloseConnection`` closes the communication while notifying the target. The client socket is simply closed and shut down.

These functions can be given as the communication interface to ``opilhostInitialize``.

.. _sec-c-implementation-example-simif:

Simulation interface
^^^^^^^^^^^^^^^^^^^^

The simulation interface is implemented through buffers. The main idea is to have internal buffers to allocate measurements, simulation data, control signals, and controller data. At each sampling instant, data from the simulation is copied into these internal buffers. When ``OPiL Host`` needs to send data to the target, these buffers are used as the source of data. Likewise, whenever there are control signals and data available, they are copied to these internal buffers and, when ``OPiL Host`` needs to update the simulation with these signals, they are copied from these buffers. 

Although there are more efficient ways to achieve this simulation interface, this is one that is implemented only as C code without any direct dependency on the simulation software. In this way, this interface does not need to be changed when moving to a different simulation tool.

The simulation interface is implemented in the |simif_h| and |simif_c| files, and the documentation of the implemented functions is available in the header file. The interface needs to be initialized by providing pointers to where the data lie in the actual simulation. See :ref:`sec-c-implementation-running-opil-host` for an example of how the simulation interface can be initialized.

.. _sec-c-implementation-example-plecs-interface:

Configuration file
^^^^^^^^^^^^^^^^^^

The four data structures defining the data that is exchanged between simulation and target need to be defined in a header file called ``stypes.h``. This include throughout ``OPiL`` and each project should have is own ``stypes.h`` file. For reference, see the |buck_example_stypes_h| provided in our example.


.. _sec-c-implementation-running-opil-host:

Running OPiL host
---------------------

The simulation software and ``OPiL host`` interact through a DLL. Usually, there are at least three events where the software interacts with the DLL: when the simulation starts, at the sample time of the DLL block inside the simulation, and when the simulation terminates. These events are used to configure, run and terminate ``OPiL host``.


Start
^^^^^

During initialization, ``OPiL host`` needs to be configured with the callbacks providing access to the  simulation and communication interfaces. In this section, the following code can be executed:

.. code-block:: c
    :linenos:

    #define CONFIG_MEAS_SIZE        (sizeof(stypesMeasurements_t) / 4)
    #define CONFIG_SIM_DATA_SIZE    (sizeof(stypesSimData_t) / 4)
    #define CONFIG_CONTROL_SIZE     (sizeof(stypesControl_t) / 4)
    #define CONFIG_CONTROLLER_SIZE  (sizeof(stypesControllerData_t) / 4)

    int32_t conn;
    
    float measurements[CONFIG_MEAS_SIZE];
    float simdata[CONFIG_SIM_DATA_SIZE];
    float control[CONFIG_CONTROL_SIZE];
    float controllerdata[CONFIG_CONTROLLER_SIZE];

    simifInitialize((void *)( &measurements ), (void *)( &simdata ), (void *)( &control ), (void *)( &controllerdata ));

    opilhostCommConfig_t comm;
    opilhostSimConfig_t sim;

    comm.openConn = hostCommOpen;
    comm.closeConn = hostCommClose;
    comm.sendData = hostCommSend;
    comm.receiveData = hostCommReceive;

    sim.updateSim = simifUpdateSimulation;
    sim.getMeas = simifGetMeasurements;
    sim.getSimData = simifGetSimData;
    sim.updateControl = simifUpdateControl;
    sim.updateControllerData = simifUpdateControllerData;
    sim.applyControl = simifApplyControl;

    opilhostInitialize(&comm, &sim);

    conn = opilhostConnectToTarget(0);

First, buffers to hold data that will be exchanged between ``OPiL host`` and simulation are created (lines 8-11).

Next, the simulation interface is initialized, with pointers to the buffers used to interface with the simulation software (line 13).

``OPiL host`` is initialized in lines  15-30, where the required callbacks are set. The simulation interface callbacks are defined in |simif_h| and |simif_c|, and should not require any changes.

After ``OPiL host`` is initialized, it is necessary to connect to the target (line 32). The variable ``conn`` can be used to detect whether connecting to the target was possible or not. Its value will be zero only if  connection was successful.

Run
^^^

During each sampling step of the DLL block in the simulation, ``OPiL host`` must be executed. An example of this routine is shown below.

.. code-block:: c
    :linenos:
    
    if( conn == 0 )
    {
        int k;

        for(k = 0; k < CONFIG_MEAS_SIZE; k++) 
            measurements[k] = (float)InputSignal(0, k);

        for(k = CONFIG_MEAS_SIZE; k < (CONFIG_MEAS_SIZE + CONFIG_SIM_DATA_SIZE); k++)
            simdata[k-CONFIG_MEAS_SIZE] = (float)InputSignal(0, k);

        opilhostExchangeDataTarget();

        for(k = 0; k < CONFIG_CONTROL_SIZE; k++)
            OutputSignal(0, k) = control[k];

        for(k = CONFIG_CONTROL_SIZE; k < (CONFIG_CONTROL_SIZE + CONFIG_CONTROLLER_SIZE); k++)
            OutputSignal(0, k) = controllerdata[k - CONFIG_CONTROL_SIZE];
    }

If there is a connection between host and target (``conn`` is zero), measurements and simulation data are copied to the buffers that are used to interface simulation and ``OPiL host`` (lines 5-9). In this example,  ``InputSignal`` represents a software-specific function that reads data from the simulation. After data is copied to the interface buffers, ``OPiL host`` state machine is executed with the ``opilhostExchangeDataTarget`` function (line 11). After execution, new control and controller data should be available in the interface buffers, and are copied into the simulation by using a software-specific ``OutputSignal`` function (lines 13-17).

Terminate
^^^^^^^^^

When the simulation ends, ``OPiL host`` also needs to be terminated. In this case, the following code should be executed.

.. code-block:: c
    :linenos:
    
    if( conn == 0 ) opilhostDisconnectFromTarget(0);

OPiL target
---------------

Implementation of OPiL target is similar to that of the host, and is documented in the |opiltarget_h| file.

The framework on the target side is initialized with  ``opiltargetInitialize``, where the framework is configured with the required callbacks.

After initialization, the target is ready to wait for connections from the target. The ``opiltargetConnectToHost`` is used to wait for a connection. This function returns 0 when a new connection is established, and any other value can be used to represent an error or timeout. 

Once a connection is established, ``opiltargetExchangeDataHost`` is used to exchange data with the host. Internally, the target waits for incoming data. After receiving data from the host, the control algorithm is executed, and the results are sent back to the host. Then the target waits again for incoming data.

This is repeated until it is detected that the host closed the connection. Then, the function ``opiltargetDisconnectFromHost`` is used to terminate the connection on the target side.

See :ref:`sec-c-implementation-running-opil-target`  for an example on how ``OPiL target`` can be initialized and executed on the target. 

Communication interface
^^^^^^^^^^^^^^^^^^^^^^^

The communication interface on the target-side is very similar to that of the host.
Its implementation requires implementing four functions: open connection, close connection, send, and receive. The signatures and behavior of these functions are documented in |opiltarget_h|. 

Implementation examples can be found in the ``comm`` folder of the ``OPiL`` project. Only a brief description of how these functions behave is given here.

The ``targetCommSockOpenConnection`` function creates a server socket and waits for a connection. The functions ``targetCommSockSendData`` and ``targetCommSockReceiveData`` are used to exchange data with the client socket. The ``targetCommSockCloseConnection`` function closes the server socket, and no further connections will be accepted, until a new server socket is created again.


.. _sec-c-implementation-ctlrif:

Controller interface
^^^^^^^^^^^^^^^^^^^^

The controller interface, along with its documentation, is implemented in the |ctlrif_h| and |ctlrif_c| files.  Similar to implementation of the :ref:`sec-c-implementation-example-simif`, the controller interface is implemented through buffers.

When new measurements and simulation data arrive from the host (simulation), they are copied to buffers inside the controller interface. The control function is executed based on data stored in these buffers, and it is expected that the control function writes data to the control and controller data buffers inside the controller interface. ``OPiL target`` uses these buffers to send the control signals and controller data back to the simulation.

The controller interface requires proper initialization before it can be executed. This initialization consists of setting two callbacks: control ``initialize`` and control ``run``. These functions are the interface to the control algorithm. 

The ``inititalize`` function is called whenever a new connection is established. This function can be used to initialize the controller (for example, resetting controller states). This function does not take any argument, and does not return any value.

The ``run`` function is executed whenever new measurements and simulation data are available. This function takes four pointers as arguments (see the definition of ``ctlrifControlRun_t`` in |ctlrif_h| - line 23):

- ``meas`` points to the buffer holding measurements received from the host.
- ``simData`` points to the buffer holding simulation data received from the host.
- ``control`` points to the buffer holding the control signals that will be sent to the host.
- ``controllerData`` points to the buffer holding the controller data that will be sent to the host.

The ``run`` function can cast these pointers to the types defined in the ``stypes.h`` configuration file to access the buffers. 

For an example of how the ``init`` and ``run`` function can be defined, see |buckcontrol_h| and |buckcontrol_c|.

.. _sec-c-implementation-running-opil-target:

Running OPiL target
-----------------------

``OPiL target`` is executed by the embedded target. The simplest way to execute ``OPiL target`` is in a loop, where data is exchanged with ``OPiL host`` as long as the simulation runs. The following example can be used to run ``OPiL target`` (given the proper implementation of the controller and communication interfaces):

.. code-block:: c
    :linenos:
    
    void main(void){
        
        int32_t conn;
        
        embdtargetInitialize();

        /* Initializes controller interface */
        ctlrifInitialize(controlInit, controlRun);

        /* Initializes opil */
        opiltargetCommConfig_t comm;
        opiltargetControlConfig_t control;

        comm.openConn = targetCommOpen;
        comm.closeConn = targetCommClose;
        comm.sendData = targetCommSend;
        comm.receiveData = targetCommReceive;

        control.updateMeas = ctlrifUpdateMeasurements;
        control.updateSimData = ctlrifUpdateSimData;

        control.initControl = ctlrifInitializeControl;
        control.runControl = ctlrifRunControl;

        control.getControl = ctlrifGetControl;
        control.getControllerData = ctlrifGetControllerData;

        opiltargetInitialize(&comm, &control);
        
        while(1){
            
            conn = opiltargetConnectToHost(0);
            if( conn != ) continue;
            
            while( opiltargetExchangeDataHost() == 0 );

            opiltargetDisconnectFromHost(0);
        }
    }

First, the controller interface needs to be initialized with ``init`` and ``run`` functions (line 8). For an example on how ``init`` and ``run`` can be implemented, see |buckcontrol_h| and |buckcontrol_c|.

After initializing the controller interface, ``OPiL target`` is initialized (lines 11-26), where the proper callbacks are set. These callbacks provide interface to the controller and to the communication interface.

The functions registered as controller interface (lines 19-26) are defined in |ctlrif_h| and don't have to be modified.

The functions registered as communication interface (lines 14-17) need to be defined according to the target's hardware, and the communication protocol used. See the ``comm`` folder inside the ``OPiL`` project for application examples.

The ``while(1)`` loop executes ``OPiL target``. A new connection can be detected when ``opiltargetConnectToHost`` returns a value that is different than zero (lines 32-33). When a connection is detected, ``opiltargetExchangeDataHost`` can be executed, which is responsible for executing the state machine of ``OPiL target``, where data is received from the host, the controller is executed, and data is sent back (line 35). This is executed until a disconnection is detected (``opiltargetExchangeDataHost`` returns a value that is different than zero). After disconnection, the connection is closed on the target side, by calling ``opiltargetDisconnectFromHost`` (line 37).

.. raw:: html

    <hr>

This section discussed implementation of the OPiL framework in C, along with examples of how to interface the framework with the simulation and with a controller. For a more concrete example of using OPiL with a simulation software and an (emulated) embedded controller, see :ref:`sec-plecs-example`. 
