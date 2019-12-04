package main

import (
	"os"
	"os/signal"
	"syscall"
)

func handleSigHup(sigHup chan os.Signal) {
	signal.Notify(sigHup, syscall.SIGHUP)
}

func handleSigUsr1(sigHup chan os.Signal) {
}

func handleSigUsr2(sigHup chan os.Signal) {
}
