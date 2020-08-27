# Sigfox Protocol Physical Layer for S2-LP - Demo for HT Micron HT32SX

`renard-phy-s2lp-demo-ht32sx` is demo application for the open source Sigfox PHY layer [`renard-phy-s2lp`](https://github.com/Jeija/renard-phy-s2lp) for HT Micron's HT32SX, which is a system-in-package (SiP) that integrates both STMicroelectronics' S2-LP ultra-low power transceiver chip and an STM32L0 microcontroller.

It combines [`renard-phy-s2lp`](https://github.com/Jeija/renard-phy-s2lp) and renard-phy-s2lp's STM32L0 hardware abstraction layer [`renard-phy-s2lp-hal-stm32`](https://github.com/Jeija/renard-phy-s2lp-hal-stm32).

## Building
### Clone
Make sure to clone this repository recursively so that all submodules get downloaded as well:
```
git clone --recursive https://github.com/Jeija/renard-phy-s2lp-demo-ht32sx/
```

If you forgot `--recursive` while cloning, you can also use the following command to initialize all submodules:
```
git submodule update --init --recursive
```

### Configure
See [`renard-phy-s2lp-hal-stm32`](https://github.com/Jeija/renard-phy-s2lp-hal-stm32) for information on the default pin configuration and on how to change it.

### Compile
Make sure you have [`stlink`](https://github.com/texane/stlink) and the GNU ARM Toolchain (`arm-none-eabi`) installed. To compile and flash this demo application, use:

```
make
make stlink-flash
```

## Usage
`renard-phy-s2lp-demo-ht32sx` assumes a button connected to GPIOB 5 and an LED connected to GPIOB 0. It transmits a Sigfox uplink whenever that button is pressed (modify `src/button.c` or `src/main.c` to change button / LED pinouts). The application then waits for a downlink and outputs the received downlink frame on the serial console UART2 (GPIOA 2 is TX, GPIOA 3 is RX, default baudrate is 115200 Bd). Watch the serial console for debug output!

`renard-phy-s2lp-demo-ht32sx` loads the factory-provisioned Sigfox credentials from  the STM32L0's internal EEPROM, so make sure your EEPROM is not corrupted!
It currently does *not* support Sigfox's NVM format, which means that Sigfox sequence numbers will start from 0 after flashing and you will have to disengage the sequence number in Sigfox's backend.

KiCad schematics and a PCB reference design for my development board can be found in the [ht32sx-nano](https://github.com/Jeija/ht32sx-nano) repository.

## HT32SX Internal Pinout
#### STM32L0 to S2-LP
S2-LP Pin Name | STM32L0 Pin
---:|:---
MISO (SPI) | GPIOB 4
MOSI (SPI) | GPIOA 7
SCLK (SPI) | GPIOB 3
CS (SPI) | GPIOA 15
SDN | GPIOB 8
GPIO3 | GPIOB 2

#### S2-LP to SKY66420-11 Front-End Module (FEM)
SKY66420-11 | S2-LP Pin
---:|:---
CSD | GPIO2
CTX | GPIO0
CPS | GPIO1

## Licensing
`renard-phy-s2lp-demo-ht32sx` is licensed under the MIT License. See `LICENSE` for details.



