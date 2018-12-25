package main

import (
	  "fmt"
	  "os"
	  "time"

  // required for spawning Core
    "os/exec"

  // required for connecting Core with Flex
	  "nanomsg.org/go/mangos/v2"
	  "nanomsg.org/go/mangos/v2/protocol/pair"

	  // register transports
	  _ "nanomsg.org/go/mangos/v2/transport/ipc"

  // required for flatbuffers
	  // sample "MyGame/Sample"
	  // flatbuffers "github.com/google/flatbuffers/go"
	 // "strconv"
)

/// Execute a process and forward its stdout and stdin to this process
func exec_command(program string, args ...string) {
    cmd := exec.Command(program, args...)
    cmd.Stdin = os.Stdin;
    cmd.Stdout = os.Stdout;
    cmd.Stderr = os.Stderr;
    err := cmd.Run() 
    if err != nil {
        fmt.Printf("%v\n", err)
    }
}

func die(format string, v ...interface{}) {
	fmt.Fprintln(os.Stderr, fmt.Sprintf(format, v...))
	os.Exit(1)
}

func sendName(sock mangos.Socket, name string) {
	fmt.Printf("%s: SENDING \"%s\"\n", name, name)
	if err := sock.Send([]byte(name)); err != nil {
		die("failed sending: %s", err)
	}
}

func recvName(sock mangos.Socket, name string) {
	var msg []byte
	var err error
	if msg, err = sock.Recv(); err == nil {
		fmt.Printf("%s: RECEIVED: \"%s\"\n", name, string(msg))
	}
}

func sendRecv(sock mangos.Socket, name string) {
	for {
		sock.SetOption(mangos.OptionRecvDeadline, 100*time.Millisecond)
		recvName(sock, name)
		time.Sleep(time.Second)
		sendName(sock, name)
	}
}

func create_server_node(url string) (sock mangos.Socket) {
	var err error
	if sock, err = pair.NewSocket(); err != nil {
		die("can't get new pair socket: %s", err)
	}
	if err = sock.Listen(url); err != nil {
		die("can't listen on pair socket: %s", err.Error())
	}
	return
}

func client_node(url string) {
	var sock mangos.Socket
	var err error

	if sock, err = pair.NewSocket(); err != nil {
		die("can't get new pair socket: %s", err.Error())
	}
	if err = sock.Dial(url); err != nil {
		die("can't dial on pair socket: %s", err.Error())
	}
	sendRecv(sock, "node1")
}

func main() {
    var addr string = "ipc:///tmp/synchrotest7.ipc"
    // Create a nanomsg endpoint
    var server_sock = create_server_node(addr)

    // Send and Receive some data on that address
	go sendRecv(server_sock, "node0")
    // spawn core to connect to the endpoint
    var core_executable_path string = "build/core"
    go exec_command(core_executable_path, addr)
    time.Sleep(time.Second)

    server_sock.Close()
    os.Exit(12)
}

/*
// Example how to use Flatbuffers to create and read binary buffers.
func mainold() {
	  builder := flatbuffers.NewBuilder(0)
    weaponOne := builder.CreateString("Sword")
    sample.WeaponStart(builder)
    sample.WeaponAddName(builder, weaponOne)
    sample.WeaponAddDamage(builder, 3)
    sword := sample.WeaponEnd(builder)
    builder.Finish(orc)
    buf := builder.FinishedBytes()

    monster := sample.GetRootAsMonster(buf, 0)
    assert(monster.Mana() == 150, "`monster.Mana()`", strconv.Itoa(int(monster.Mana())), "150")
}*/

/*
A: whole graph in go, only subsets (using ECS) in C++, updates via events
-> flexibility in go, performance in C++ (?)

go hat entity-graphen (concurrency given) (gamemode code darf länger dauern)
c++ has systems with a component registry for caching object pointers into graph, which listeners updating the relevant components on graph change -> reads are cached (+ in a cpu cache friendly way)
system specific event -> cache may be required (go side)

syncro:
c++ read: iterate over components, and components' entities
c++ write: component updated -> event to flex
go read: traverse go-graph
go write:

Q: Is it possible to cache a graph in an ecs? Was it done for the rendering system? How to translate the systems?
Q: Is ECS so much faster to justify the hassle (??)

strömungssimulation:
"federsystem" zwischen strömungslinien
federn können hinzugefügt werden um besseren detailgrad zu erreichen ->
erste grobe Richtung finden -> dann feineinstellung?
genetische algorithmen oder suchalgorithmen um optimale federrichtung zu erreichen.
federn sind relations? mit Beschleunigungs-vektoren (F ~ a).
randbedingungen der kontur sollen verändert werden -> In welche Richtung und wie stark?
an welchen stellen? netz an randfedern (durch reibung)

+  ~ dichte ~ dicke der strömungsröhre bzw. länge des vektors wenn nicht orthogonal)-attribut und )
*/
