package main

import (
	"bytes"
	"crypto/md5"
	"encoding/json"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"path/filepath"
	"time"

	"github.com/klauspost/compress/flate"
	"github.com/klauspost/compress/gzip"

	"github.com/gin-gonic/gin"
	"github.com/gomodule/redigo/redis"
)

func handleGetInspectorLanguagesList(c *gin.Context) {
	acceptHeader := c.GetHeader("Accept")
	h := md5.New()
	h.Write([]byte(acceptHeader))
	handleReverseProxy(c, fmt.Sprintf("inspectorLanguagesList:%x", h.Sum(nil)), "https://godbolt.org/api/languages", "GET", nil)
}

func handleGetInspectorCompilersList(c *gin.Context) {
	acceptHeader := c.GetHeader("Accept")
	h := md5.New()
	h.Write([]byte(acceptHeader))
	handleReverseProxy(c, fmt.Sprintf("inspectorCompilersList:%x", h.Sum(nil)), "https://godbolt.org/api/compilers", "GET", nil)
}

func handleGetInspectorCompilersListEx(c *gin.Context) {
	id := c.Param("id")
	if id == "" {
		c.JSON(http.StatusOK, gin.H{
			"result": "error",
			"msg":    "id expected"})
		return
	}
	acceptHeader := c.GetHeader("Accept")
	h := md5.New()
	h.Write([]byte(acceptHeader))
	handleReverseProxy(c, fmt.Sprintf("inspectorCompilersList:%s:%x", id, h.Sum(nil)), "https://godbolt.org/api/compilers/"+id, "GET", nil)
}

func handleGetInspectorLibrariesList(c *gin.Context) {
	id := c.Param("id")
	if id == "" {
		c.JSON(http.StatusOK, gin.H{
			"result": "error",
			"msg":    "id expected"})
		return
	}
	acceptHeader := c.GetHeader("Accept")
	h := md5.New()
	h.Write([]byte(acceptHeader))
	handleReverseProxy(c, fmt.Sprintf("inspectorLibrariesList:%s:%x", id, h.Sum(nil)), "https://godbolt.org/api/libraries/"+id, "GET", nil)
}

func handleGetInspectorShortLinkInfo(c *gin.Context) {
	id := c.Param("id")
	if id == "" {
		c.JSON(http.StatusOK, gin.H{
			"result": "error",
			"msg":    "id expected"})
		return
	}
	acceptHeader := c.GetHeader("Accept")
	h := md5.New()
	h.Write([]byte(acceptHeader))
	handleReverseProxy(c, fmt.Sprintf("inspectorShortlinkinfo:%s:%x", id, h.Sum(nil)), "https://godbolt.org/api/shortlinkinfo/"+id, "GET", nil)
}

func handleInspectorCompile(c *gin.Context) {
	id := c.Param("id")
	if id == "" {
		c.JSON(http.StatusOK, gin.H{
			"result": "error",
			"msg":    "id expected"})
		return
	}
	body, err := c.GetRawData()
	if err != nil {
		c.JSON(http.StatusOK, gin.H{
			"result": "error",
			"msg":    err.Error()})
		return
	}

	h := md5.New()
	acceptHeader := c.GetHeader("Accept")
	h.Write([]byte(acceptHeader))
	h.Write(body)
	cacheKey := fmt.Sprintf("inspectorCompile:%s:%x", id, h.Sum(nil))

	handleReverseProxy(c, cacheKey, fmt.Sprintf("https://godbolt.org/api/compiler/%s/compile", id), "POST", bytes.NewReader(body))
}

func updateCompilerExploreConfiguration() []byte {
	// download client-options.js
	const targetURL = `https://godbolt.org/client-options.js`
	req, err := http.NewRequest("GET", targetURL, nil)
	if err != nil {
		log.Println("error creating http request", targetURL)
		return nil
	}

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

	body, err := ioutil.ReadAll(reader)
	if err != nil {
		log.Println(err)
		return nil
	}
	// preprocess the content
	index := bytes.IndexByte(body, byte('{'))
	if index < 0 {
		log.Println("unexpected content body")
		return nil
	}
	body = body[index:]
	for body[len(body)-1] != byte('}') {
		body = body[:len(body)-1]
	}
	// unmarshal as JSON
	var v map[string]interface{}
	err = json.Unmarshal(body, &v)
	if err != nil {
		log.Println(err)
		return nil
	}

	const cacheKey = `inspectorConfigurations`
	cache.PutWithTimeout(cacheKey, body, 24*time.Hour)
	return body
}

func handleGetInspectorConfigurations(c *gin.Context) {
	const cacheKey = `inspectorConfigurations`
	const acceptHeader = `application/json`
	if cache.IsExist(cacheKey) {
		content, err := redis.Bytes(cache.Get(cacheKey))
		if err == nil {
			c.Data(http.StatusOK, acceptHeader, content)
			return
		}
	}
	content := updateCompilerExploreConfiguration()
	c.Data(http.StatusOK, acceptHeader, content)
}

func handleGetInspectorSamples(c *gin.Context) {
	language := c.Param("language")
	if language == "" {
		c.JSON(http.StatusOK, gin.H{
			"result": "error",
			"msg":    "language expected"})
		return
	}

	res := samples.GetSamples(language)
	c.JSON(http.StatusOK, gin.H{
		"language": language,
		"list":     res})
}

func handleGetInspectorSample(c *gin.Context) {
	language := c.Param("language")
	if language == "" {
		c.JSON(http.StatusOK, gin.H{
			"result": "error",
			"msg":    "language expected"})
		return
	}
	file := c.Param("file")
	if file == "" {
		c.JSON(http.StatusOK, gin.H{
			"result": "error",
			"msg":    "file expected"})
		return
	}

	filePath := filepath.Join(sampleDir, language, file)
	_, err := os.Stat(filePath)
	if os.IsNotExist(err) {
		c.AbortWithStatus(http.StatusNotFound)
		return
	}

	c.File(filePath)
}
