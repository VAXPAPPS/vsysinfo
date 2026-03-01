# VAXP Hardware Info (vsysinfo)

VAXP Hardware Info is an advanced, lightweight system information tool designed specifically for the VAXP-OS ecosystem. It provides comprehensive, real-time details about your computer's hardware and operating system in a beautifully designed, modern GTK3 interface featuring transparent glassmorphism effects and custom window decorations.

## Features

- **System Information:** View your OS distribution, Kernel version, Architecture, Computer Model, and current Shell.
- **Processor (CPU):** Displays CPU Model, Vendor, Cores/Threads, Cache sizes, and real-time CPU frequency updates.
- **Memory (RAM):** Accurately tracks total installed RAM and Swap space with live usage statistics.
- **Motherboard:** Fetches Motherboard manufacturer, product name, and BIOS details.
- **Graphics (GPU):** Automatically detects available Graphics Adapters and displays their exact model names and total Video RAM (VRAM) sizes.
- **Sensors:** Live temperature monitoring for various hardware components utilizing the `hwmon` sensor interface.
- **Storage:** Lists all active storage mounts, file systems, and their respective total, used, and free capacities.
- **Modern User Interface:** Built with GTK3, it comes with a custom VAXP ecosystem headerbar, rounded corners, and a beautifully tailored semi-transparent dark theme.

## Dependencies

Before compiling or running `vsysinfo`, ensure you have the following packages installed on your Linux system:

- **GTK3 Development Libraries**: `libgtk-3-dev`
- **Compiler Options**: `gcc`, `make`, `pkg-config`
- **Hardware Utilities**: `pciutils` (for `lspci` to detect GPU/VRAM)
- Standard system tools (usually pre-installed like `sysfs` access and `udev`).

## Building from Source

To compile the application from the source, simply navigate to the project directory and run `make`:

```bash
cd /path/to/vsysinfo
make clean && make
```
The executable will be placed in the `bin/` directory.

```bash
./bin/vsysinfo
```

## Packaging

To create a Debian package (`.deb`) for easy distribution and installation across VAXP-OS:

```bash
./build_deb.sh
```
This will generate `vsysinfo_1.0-1_amd64.deb` which can be installed via `dpkg -i`.

---

This project is part of the VAXP ecosystem.
