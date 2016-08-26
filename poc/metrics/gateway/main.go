package main

import (
	"fmt"
	"net"
)

func main() {
	addr := net.UDPAddr{
		Port: 9876,
		IP:   net.ParseIP("0.0.0.0"),
	}
	conn, err := net.ListenUDP("udp", &addr)
	defer conn.Close()
	if err != nil {
		panic(err)
	}
	var buf [1024]byte
	for {
		fmt.Printf("waiting\n")
		n, _, err := conn.ReadFromUDP(buf[:])
		if err != nil {
			panic(err)
		}
		fmt.Printf("%s\n", buf[0:n])
	}
}
