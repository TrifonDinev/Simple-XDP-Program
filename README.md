# A simple XDP program
A simple XDP program that is aimed at people who are NEW to XDP and will mostly help them to easily start their first XDP program and continue to develop and learn.
\
The main and only idea/purpose of this simple xdp program is to help people who are starting out and trying to learn xdp or compile their first xdp program, so there will be no complicated or extra features added to this program.
\
The program includes 5 simple examples with comments, so you will understand them easily from the first time.

## Requirements:
- A distribution with recommended Linux Kernel >= 5.15
- Ensure the following packages are installed:
These packages are installed via `apt` (Ubuntu, Debian, etc.), or similar package names in other package managers.
```bash
# Install dependencies.
sudo apt install -y clang llvm build-essential libconfig-dev libelf-dev libpcap-dev m4 gcc-multilib

# We need tools for our kernel since we need BPFTool.
# If there are no available, try to build BPFTool from source (https://github.com/libbpf/bpftool)
# For Debian 12 (which I mainly use) I build it from source
sudo apt install -y linux-tools-$(uname -r)
```

## Building:
Use `make` command to build.

## Load/Unload:
> [!CAUTION]
> Do not forget to replace `ens3` with your interface name.
\
> You can also use `xdp` instead of `xdpgeneric` to load the program in driver mode/native mode if your NIC has driver support. [NIC driver XDP support list](https://github.com/iovisor/bcc/blob/master/docs/kernel-versions.md#xdp)
- Loading the program: `sudo ip link set ens3 xdpgeneric obj simple_xdp_program.o sec simple_xdp_program`
- Unloading the program: `sudo ip link set ens3 xdpgeneric off`

## Additional:
- You can use `ip link` to see if the program is attached.
- You can use `cat /sys/kernel/debug/tracing/trace_pipe` to view the bpf_printk messages.

## License:
Licensed under the [MIT License](LICENSE).
