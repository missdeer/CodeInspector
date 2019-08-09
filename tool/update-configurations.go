package main

import (
	"bytes"
	"crypto/md5"
	"encoding/json"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"os"
)

const (
	clientOptionsURL = `https://godbolt.org/client-options.js`
	localFileName    = `configurations.json`
)

func main() {
	// download client-options.js
	resp, err := http.Get(clientOptionsURL)
	if err != nil {
		log.Fatal(err)
	}

	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		log.Fatal(err)
	}
	// preprocess the content
	index := bytes.IndexByte(body, byte('{'))
	if index < 0 {
		log.Fatal("unexpected content body")
	}
	body = body[index:]
	for body[len(body)-1] != byte('}') {
		body = body[:len(body)-1]
	}
	// unmarshal as JSON
	var v map[string]interface{}
	err = json.Unmarshal(body, &v)
	if err != nil {
		log.Fatal(err)
	}

	// check md5 sum
	needToSave := false
	h := md5.New()
	io.Copy(h, bytes.NewBuffer(body))
	newSum := h.Sum(nil)

	f, err := os.Open(localFileName)
	if err == nil {
		h := md5.New()
		if _, err := io.Copy(h, f); err != nil {
			needToSave = true
		}

		oldSum := h.Sum(nil)
		needToSave = !bytes.Equal(newSum, oldSum)
		f.Close()
	}
	if !needToSave {
		log.Println("configurations not changed")
		return
	}
	// save to local file
	f, err = os.OpenFile(localFileName, os.O_TRUNC|os.O_WRONLY, 0644)
	if err != nil {
		log.Fatal(err)
	}
	f.Write(body)
	f.Close()
	log.Println("configurations.json has been updated")
}
