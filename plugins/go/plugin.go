package main

import (
	"fmt"
	"nanomsg.org/go/mangos/v2"
	"nanomsg.org/go/mangos/v2/protocol/pair"
	"os"
	"time"

	// register transports
	_ "nanomsg.org/go/mangos/v2/transport/all"
)

/// Exit this application, while printing something.
func die(format string, v ...interface{}) {
	fmt.Println("Beep boop. Shutting down")
	fmt.Fprintln(os.Stderr, fmt.Sprintf(format, v...))
	os.Exit(1)
}

/// Send a string via this socket
func sendString(sock mangos.Socket, name string) {
	fmt.Printf("SENDING \"%s\"\n", name)
	if err := sock.Send([]byte(name)); err != nil {
		die("failed sending: %s", err)
	}
}

func createClientNode(url string) (sock mangos.Socket) {
	var err error
	if sock, err = pair.NewSocket(); err != nil {
		die("can't get new pair socket: %s", err)
	}
	if err = sock.Dial(url); err != nil {
		die("can't dial on pair socket: %s", err.Error())
	}
	return
}

func main() {

	quotes := [3]string{
		"I will be phenomenal to the women. I mean, I want to help women.",
		"If Obama resigns from office NOW, thereby doing a great service to the country—I will give him free lifetime golf at any one of my courses!",
		"The U.S. cannot allow EBOLA infected people back. People that go to far away places to help out are great-but must suffer the consequences!",
	}

	var addr string = "ipc:///tmp/registry_endpoint.ipc"
	node := createClientNode(addr)

	for _, quote := range quotes[:] {
		sendString(node, quote)
		time.Sleep(1 * time.Second)
	}

	os.Exit(0)
}
