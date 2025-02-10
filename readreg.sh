# This small bash script reads the value of a register on an MCU and prints it to terminal
# note for use: make sure another terminal is open that has already ran the `make load` command

# edit reg to be address of register to be read
reg=0x10000000

# open client, read value of reg, print value out to terminal, then quit
make client 2>&1 <<EOF | grep $reg
start
monitor mdw $reg 1
q
EOF

