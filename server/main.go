package main

import (
	"bytes"
	"compress/flate"
	"compress/gzip"
	"flag"
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

	r.NoRoute(func(c *gin.Context) {
		c.Redirect(http.StatusFound, "https://ci.minidump.info/dl/")
	})

	v1 := r.Group("/v1")
	{
		v1.GET("/api/inspector/languages", handleGetInspectorLanguagesList)
		v1.GET("/api/inspector/compilers", handleGetInspectorCompilersList)
		v1.GET("/api/inspector/compilers/:id", handleGetInspectorCompilersListEx)
		v1.GET("/api/inspector/libraries/:id", handleGetInspectorLibrariesList)
		v1.GET("/api/inspector/shortlinkinfo/:id", handleGetInspectorShortLinkInfo)
		v1.POST("/api/inspector/compiler/:id/compile", handleInspectorCompile)
		v1.GET("/api/inspector/configurations", handleGetInspectorConfigurations)

		v1.GET("/api/runner/compilers", handleGetRunnerCompilersList)
		v1.POST("/api/runner/compile", handleRunnerCompile)
		v1.POST("/api/runner/permlink", handleRunnerPermlink)
		v1.GET("/api/runner/permlink/:link", handleGetRunnerPermlink)
		v1.GET("/api/runner/template/:name", handleGetRunnerTemplate)
	}

	done := make(chan bool)

	go func() {
		dailyTicker := time.NewTicker(24 * time.Hour)
		for {
			select {
			case <-done:
				return
			case <-dailyTicker.C:
				updateCompilerExploreConfiguration()
				updateWandboxCompilersList()
			}
		}
	}()

	log.Fatal(r.Run(bindAddr))
}
