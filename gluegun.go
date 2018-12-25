package main

import (
	"fmt"
	"os"
	"os/signal"
	"syscall"
	"time"

	"nanomsg.org/go/mangos/v2"
	"nanomsg.org/go/mangos/v2/protocol/bus"

	// register transports
	_ "nanomsg.org/go/mangos/v2/transport/all"
	// gluegun
	"github.com/inexorgame/plugin"
)

func die(format string, v ...interface{}) {
	fmt.Fprintln(os.Stderr, fmt.Sprintf(format, v...))
	os.Exit(1)
}

func receivePluginMessage(sock mangos.Socket) *plugin.Message{
	var buf[]byte
	var err error
	var msg *plugin.Message

	if buf, err = sock.Recv(); err == nil {
		msg = plugin.GetRootAsMessage(buf, 0)
	}

	return msg
}

func createServerNode(url string) (sock mangos.Socket) {
	var err error
	if sock, err = bus.NewSocket(); err != nil {
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
		_ = <-gracefulStop
		fmt.Println("Closing server. Beep bop.")

		if err := serverSock.Close(); err != nil {
			die("can't close socket: %s", err.Error())
		}
		os.Exit(0)
	}()

	// Send and Receive some data on that address
	fmt.Println("Started server successfully. Waiting for instructions.")

	ticker := time.NewTicker(500 * time.Millisecond)
	func() {
		for _ = range ticker.C {
			// check every 0.5 for new RegistryMessages
			go receivePluginMessage(serverSock)
		}
	}()
}
