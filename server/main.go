package main

import (
	"bytes"
	"compress/flate"
	"compress/gzip"
	"crypto/md5"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"math/rand"
	"net/http"
	"os"
	"time"

	"github.com/gin-gonic/gin"
	"github.com/gomodule/redigo/redis"
)

var (
	cache *RedisCache
)

func handleReverseProxy(c *gin.Context, cacheKey string, targetURL string, method string, body io.Reader) {
	acceptHeader := c.GetHeader("Accept")
	if cache.IsExist(cacheKey) {
		content, err := redis.Bytes(cache.Get(cacheKey))
		if err == nil {
			c.Data(http.StatusOK, acceptHeader, content)
			return
		}
	}

	req, err := http.NewRequest(method, targetURL, body)
	if err != nil {
		log.Println("error creating http request", targetURL)
		c.Status(http.StatusInternalServerError)
		return
	}

	req.Header = c.Request.Header
	req.Header.Set("Accept-Encoding", "gzip,deflate")

	client := &http.Client{
		Timeout: 300 * time.Second,
	}
	resp, err := client.Do(req)
	if err != nil || resp.StatusCode != http.StatusOK {
		log.Println("error http requesting", targetURL)
		c.Status(http.StatusServiceUnavailable)
		return
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
			c.Status(http.StatusNoContent)
			return
		}
		reader = flate.NewReader(bytes.NewReader(content[2:]))
		defer reader.Close()
	default:
		reader = resp.Body
	}

	content, err := ioutil.ReadAll(reader)
	if err != nil {
		log.Println("error reading content", targetURL)
		c.Status(http.StatusNoContent)
		return
	}
	cache.PutWithTimeout(cacheKey, content, 7*24*time.Hour)
	contentType := resp.Header.Get("Content-Type")
	c.Data(http.StatusOK, contentType, content)
}

func handleGetLanguagesList(c *gin.Context) {
	acceptHeader := c.GetHeader("Accept")
	h := md5.New()
	h.Write([]byte(acceptHeader))
	handleReverseProxy(c, fmt.Sprintf("languagesList:%x", h.Sum(nil)), "https://godbolt.org/api/languages", "GET", nil)
}

func handleGetCompilersList(c *gin.Context) {
	acceptHeader := c.GetHeader("Accept")
	h := md5.New()
	h.Write([]byte(acceptHeader))
	handleReverseProxy(c, fmt.Sprintf("compilersList:%x", h.Sum(nil)), "https://godbolt.org/api/compilers", "GET", nil)
}

func handleGetCompilersListEx(c *gin.Context) {
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
	handleReverseProxy(c, fmt.Sprintf("compilersList:%s:%x", id, h.Sum(nil)), "https://godbolt.org/api/compilers/"+id, "GET", nil)
}

func handleGetLibrariesList(c *gin.Context) {
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
	handleReverseProxy(c, fmt.Sprintf("librariesList:%s:%x", id, h.Sum(nil)), "https://godbolt.org/api/libraries/"+id, "GET", nil)
}

func handleGetShortLinkInfo(c *gin.Context) {
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
	handleReverseProxy(c, fmt.Sprintf("shortlinkinfo:%s:%x", id, h.Sum(nil)), "https://godbolt.org/api/shortlinkinfo/"+id, "GET", nil)
}

func handleCompile(c *gin.Context) {
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
	cacheKey := fmt.Sprintf("compile:%s:%x", id, h.Sum(nil))

	handleReverseProxy(c, cacheKey, fmt.Sprintf("https://godbolt.org/api/compiler/%s/compile", id), "POST", bytes.NewReader(body))
}

func main() {
	bindAddr := "127.0.0.1:8093"
	redisServer := "127.0.0.1:6379"
	flag.StringVar(&bindAddr, "bindAddr", bindAddr, "set binding address")
	flag.StringVar(&redisServer, "redis", redisServer, "set redis server address")
	flag.Parse()

	envVal := map[string]*string{
		"BIND_ADDR":  &bindAddr,
		"REDIS_ADDR": &redisServer,
	}

	for env, variable := range envVal {
		if v, ok := os.LookupEnv(env); ok {
			*variable = v
		}
	}

	cache = RedisInit(redisServer)
	rand.Seed(time.Now().Unix())

	r := gin.Default()

	r.GET("/", func(c *gin.Context) {
		c.Redirect(http.StatusFound, "https://ci.minidump.info/dl/")
	})
	r.GET("/api/languages", handleGetLanguagesList)
	r.GET("/api/compilers", handleGetCompilersList)
	r.GET("/api/compilers/:id", handleGetCompilersListEx)
	r.GET("/api/libraries/:id", handleGetLibrariesList)
	r.GET("/api/shortlinkinfo/:id", handleGetShortLinkInfo)
	r.POST("/api/compiler/:id/compile", handleCompile)
	r.StaticFile("/configurations", "./configurations.json")
	r.StaticFile("/configurations.json", "./configurations.json")

	log.Fatal(r.Run(bindAddr))
}
