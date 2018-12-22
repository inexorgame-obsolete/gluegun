package main

import (
	"fmt"
	"os"
	"os/signal"
	"syscall"
	"time"

	"nanomsg.org/go/mangos/v2"
	"nanomsg.org/go/mangos/v2/protocol/pair"

	// register transports
	_ "nanomsg.org/go/mangos/v2/transport/all"
)

func die(format string, v ...interface{}) {
	fmt.Fprintln(os.Stderr, fmt.Sprintf(format, v...))
	os.Exit(1)
}

func receiveString(sock mangos.Socket) {
	var msg []byte
	var err error
	if msg, err = sock.Recv(); err == nil {
		fmt.Printf("RECEIVED: \"%s\"\n", string(msg))
	}
}

func createServerNode(url string) (sock mangos.Socket) {
	var err error
	if sock, err = pair.NewSocket(); err != nil {
		die("can't get new pair socket: %s", err)
	}
	if err = sock.Listen(url); err != nil {
		die("can't listen on pair socket: %s", err.Error())
	}
	return
}

func main() {
	addr := "ipc:///tmp/registry_endpoint.ipc"
	// Create a nanomsg endpoint
	var serverSock = createServerNode(addr)

	gracefulStop := make(chan os.Signal)

	signal.Notify(gracefulStop, syscall.SIGINT, syscall.SIGTERM)

	go func() {
		sig := <-gracefulStop
		fmt.Printf("caught sig: %+v", sig)

		serverSock.Close()
		os.Exit(0)
	}()

	// Send and Receive some data on that address

	ticker := time.NewTicker(500 * time.Millisecond)
	func() {
		for _ = range ticker.C {
			// alle 0.5s:
			// checke ob neue nachricht da ist
			// sofort:
			// checke ob signal kill gekommen ist.
			go receiveString(serverSock)
		}
	}()
}
