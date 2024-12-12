# ft_ping 🌐

A custom implementation of the ping command for macOS, focusing on ICMP (Internet Control Message Protocol) echo requests.

## Overview 🔍

ft_ping is a network diagnostic tool that tests the reachability of a host on an Internet Protocol (IP) network. It measures the round-trip time for messages sent from the originating host to a destination computer and back.

## Features ✨

- **ICMP Echo Requests**: Send and receive ICMP packets
- **Real-time Statistics**: Track packet loss and round-trip times
- **Multiple Options**: Various flags to customize ping behavior
- **macOS Compatible**: Specifically designed for macOS systems

## Usage 🚀
sudo ./ft_ping [options] <destination>


### Options 🛠️

- `-v` Verbose output
- `-h` Show help message
- `-f` Flood ping (root only)
- `-q` Quiet output
- `-c count` Stop after sending count packets
- `-i interval` Wait interval seconds between sending each packet
- `-s size` Specify the number of data bytes to be sent
- `-t ttl` Specify the IP Time To Live


## Technical Details 🔧

- Raw socket implementation
- ICMP packet construction
- Checksum calculation
- Signal handling
- Time precision measurements

## Requirements 📋

- macOS operating system
- Root privileges (for raw socket operations)
- C compiler (clang/gcc)

## Building 🏗️

```bash
make
```

## Running 🏃

```bash
sudo ./ft_ping -v -c 5 -i 1 -s 100 -t 64 192.168.1.1
```


## Notes 📝

- Requires root privileges due to raw socket usage
- Designed specifically for macOS network stack
- Follows RFC 792 ICMP specifications

## Author 👨‍💻

[Armorine86](https://github.com/Armorine86)

## License 📄

This project is licensed under the MIT License - see the LICENSE file for details.
