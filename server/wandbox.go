package main

import (
	"bytes"
	"crypto/md5"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"time"

	"github.com/klauspost/compress/flate"
	"github.com/klauspost/compress/gzip"

	"github.com/gin-gonic/gin"
	"github.com/gomodule/redigo/redis"
)

func updateWandboxCompilersList() []byte {
	const targetURL = `https://wandbox.org/api/list.json`
	req, err := http.NewRequest("GET", targetURL, nil)
	if err != nil {
		log.Println("error creating http request", targetURL)
		return nil
	}

	req.Header.Set("Accept", "application/json")
	req.Header.Set("Accept-Encoding", "gzip,deflate")

	client := &http.Client{
		Timeout: 300 * time.Second,
	}
	resp, err := client.Do(req)
	if err != nil || resp.StatusCode != http.StatusOK {
		log.Println("error http requesting", targetURL)
		return nil
	}
	defer resp.Body.Close()

	var reader io.ReadCloser
	switch resp.Header.Get("Content-Encoding") {
	case "gzip":
		reader, err = gzip.NewReader(resp.Body)
		defer reader.Close()
	case "deflate":
		content, err := ioutil.ReadAll(resp.Body)
		if err != nil {
			log.Println("error reading deflate content", targetURL)
			return nil
		}
		reader = flate.NewReader(bytes.NewReader(content[2:]))
		defer reader.Close()
	default:
		reader = resp.Body
	}

	content, err := ioutil.ReadAll(reader)
	if err != nil {
		log.Println("error reading content", targetURL)
		return nil
	}
	const cacheKey = `runnerCompilersList`
	cache.PutWithTimeout(cacheKey, content, 24*time.Hour)
	return content
}

func handleGetRunnerCompilersList(c *gin.Context) {
	const cacheKey = `runnerCompilersList`
	const acceptHeader = `application/json`
	if cache.IsExist(cacheKey) {
		content, err := redis.Bytes(cache.Get(cacheKey))
		if err == nil {
			c.Data(http.StatusOK, acceptHeader, content)
			return
		}
	}

	content := updateWandboxCompilersList()
	c.Data(http.StatusOK, acceptHeader, content)
}

func handleRunnerCompile(c *gin.Context) {
	body, err := c.GetRawData()
	if err != nil {
		c.JSON(http.StatusOK, gin.H{
			"result": "error",
			"msg":    err.Error()})
		return
	}

	h := md5.New()
	h.Write(body)
	cacheKey := fmt.Sprintf("runnerCompile:%x", h.Sum(nil))
	handleReverseProxy(c, cacheKey, "https://wandbox.org/compile.json", "POST", bytes.NewReader(body))
}

func handleRunnerPermlink(c *gin.Context) {
	body, err := c.GetRawData()
	if err != nil {
		c.JSON(http.StatusOK, gin.H{
			"result": "error",
			"msg":    err.Error()})
		return
	}

	h := md5.New()
	h.Write(body)
	cacheKey := fmt.Sprintf("runnerPermlink:%x", h.Sum(nil))
	handleReverseProxy(c, cacheKey, "https://wandbox.org/permlink", "POST", bytes.NewReader(body))
}

func handleGetRunnerPermlink(c *gin.Context) {
	link := c.Param("link")
	if link == "" {
		c.JSON(http.StatusOK, gin.H{
			"result": "error",
			"msg":    "link expected"})
		return
	}
	handleReverseProxy(c, fmt.Sprintf("permlink:%s", link), "https://wandbox.org/permlink/"+link, "GET", nil)
}

func handleGetRunnerTemplate(c *gin.Context) {
	name := c.Param("name")
	if name == "" {
		c.JSON(http.StatusOK, gin.H{
			"result": "error",
			"msg":    "template name expected"})
		return
	}
	handleReverseProxy(c, fmt.Sprintf("runnerTemplate:%s", name), "https://wandbox.org/template/"+name, "GET", nil)
}
