# OneOS

OneOS is a small operating system written in assembly language (x86) and C, created purely for fun.
The project leverages Docker to provide a standardized, reproducible Linux build environment, and QEMU for emulation.


## Features
* **Global Descriptor Table (GDT):** Custom memory segment management configuration.
* **Interrupt Descriptor Table (IDT):** Complete setup handling hardware and software interrupts.
* **Keyboard Driver:** Integrated AZERTY layout mapping hardware scancodes to ASCII characters.
* **Interactive Terminal:** Dynamic prompt mechanism with backspace safety boundaries to protect the interface.



## Prerequisites
Before running the commands, make sure you have the following installed on your machine:
* **Docker Desktop** (ensure the application is running in the background)
* **QEMU** (`qemu-system-i386`)



## Build and Run Commands
### Build 
```bash
make
```

### Run
```bash
make run
```

### Clean Workspace
```bash
make clean
```
