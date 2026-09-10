# pscan

*pscan is a libpcap\libnet based Port Scanner which uses TCP-SYN scans*
*It is written in c++*

# Build 

1. **Clone repository**

```bash
git clone https://github.com/sagarxjoshi/pscan.git
cd pscan
```

2. **Configure the project**

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
```

3. **Compile the project**

```bash
cmake --build build --config Release
```

# Usage

```bash
pscan [OPTIONS]

 -t or --target <IP address> 

 -tn or --target-network <CIDR - example: 192.168.1.0/24>

 -p or --ports <start port-end port> 

 -o or --out-file <ouput file name> 

 -h or --help <help menu> 
```

# Output

~[Terminal Demo](./assets/output.gif)
