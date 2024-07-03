\page rpi_setup_tag RPi Setup RPi to use with Friendly LWM2M client

This section contains a detailed description of setting up the RPi 4 and running the Friendly LWM2M client on it.

- [Environment](@ref rpi_env)
- [Flashing RPi 4 with the Ubuntu Server](@ref rpi_flash)
- [Setup SSH connection](@ref rpi_ssh)
- [Set up demo board](@ref rpi_demo_board)
- [Running WppRPiExample](@ref rpi_running_demo)

### Environment {#rpi_env}

**Raspberry Pi 4 Model B.**\n
**Ubuntu Server 22.04.**

### Flashing RPi 4 with the Ubuntu Server {#rpi_flash}

Use **[this](https://ubuntu.com/tutorials/how-to-install-ubuntu-on-your-raspberry-pi#1-overview)** guide, to install Ubuntu Server 22.04, at RPi 4 and set up an ssh connection.

Configurations of the required platform.
\image html images/rpi_cfg6.png width=550px

To simplify future usage we need to set custom settings, and press **EDIT SETTINGS**.
\image html images/rpi_cfg7.png width=550px

General settings, it is recommended to configure wireless LAN and enable SSH in the **SERVICE** tab.
\image html images/rpi_cfg1.png width=550px

### Setup SSH connection {#rpi_ssh}

After flashing, you need to insert the **SD** card into the **RPi 4** and connect the power. Once the system boots up, connect via SSH. To do this, you need to be on the same **subnet** as the RPi and find its IP address through the router's settings, or connect a display and keyboard to the RPi, then enter the command **ifconfig** in the terminal. To connect, execute the following command: **ssh wpp@<rpi_ip>**.

If you need to add support for an Ethernet connection, you need to modify the configuration file **etc/netplan/50-cloud-init.yaml**. You can do this by using a keyboard and display or by removing the **SD** card from the **RPi**, inserting it into a PC, and modifying it there. The file with support for both Ethernet and WiFi looks like this, just replace the parameters with your WiFi access point details.
\image html images/rpi_cfg2.png width=850px

### Setup demo board {#rpi_demo_board}

\image html images/rpi_demo_board.jpg width=750px

For the hardware setup, you need 4 resistors of 10 kΩ, 3 resistors of 60 Ω, 3 LEDs, two buttons, and a board with sensors STEVAL-MKI217V1. 
Connection scheme.

\image html images/rpi_circuit.png width=750px

### Running WppRPiExample {#rpi_running_demo}

Using SSH, you need to transfer the **WppRPiExample** file to the **RPi 4**. First, you need to build the corresponding example for **wpp** lib. The output file will be located at the following path: **<wpp_repo>/_build/examples/raspberry_pi/WppRPiExample**. To move the executable file to the **RPi**, execute the following command: **scp WppRPiExample wpp@<rpi_ip>:~/**. After this, the corresponding file should appear on the **RPi**.
\image html images/rpi_cfg3.png width=550px

The next step is to install the **libgpiod** library to access the output pins on the board. The command is: **sudo apt-get install libgpiod2**./n
Now, you need to configure the automatic startup of **WppRPiExample** after the **RPi** is turned on. To do this, you need to create a service:
    - **sudo nano /etc/systemd/system/wpp_demo.service**
    - Fill with the following content: \image html images/rpi_cfg4.png width=750px
    - **sudo systemctl daemon-reload**
    - **sudo systemctl enable wpp_demo.service**
    - **sudo systemctl start wpp_demo.service**
    - And check the status: **sudo systemctl status wpp_demo.service** \image html images/rpi_cfg5.png width=750px
  
During operation, **WppRPiExample** generates a detailed log which is stored in the file **~/wpp_logs.txt**.