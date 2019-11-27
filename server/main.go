package main

import (
	"bytes"
	"crypto/md5"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"math/rand"
	"net/http"
	"os"
	"strings"
	"time"

	"github.com/gin-gonic/gin"
	"github.com/gomodule/redigo/redis"
)

var (
	cache *RedisCache
)

func reverseProxyGet(c *gin.Context, cacheKey string, targetURL string, method string, body io.Reader) {
	acceptHeader := c.GetHeader("Accept")
	if strings.Contains(acceptHeader, "application/json") && cache.IsExist(cacheKey) {
		languages, err := redis.Bytes(cache.Get(cacheKey))
		if err == nil {
			c.Data(http.StatusOK, "application/json", languages)
			return
		}
	}

	req, err := http.NewRequest(method, targetURL, body)
	if err != nil {
		c.Status(http.StatusServiceUnavailable)
		return
	}

	req.Header.Set("Accept", acceptHeader)

	client := &http.Client{
		Timeout: 300 * time.Second,
	}
	resp, err := client.Do(req)
	if err != nil || resp.StatusCode != http.StatusOK {
		c.Status(http.StatusServiceUnavailable)
		return
	}
	defer resp.Body.Close()

	contentType := resp.Header.Get("Content-Type")
	content, err := ioutil.ReadAll(resp.Body)
	if strings.Contains(contentType, "application/json") {
		cache.PutWithTimeout(cacheKey, content, 7*24*time.Hour)
	}
	c.Data(http.StatusOK, contentType, content)
}

func handleGetLanguagesList(c *gin.Context) {
	reverseProxyGet(c, "languagesList", "https://godbolt.org/api/languages", "GET", nil)
}

func handleGetCompilersList(c *gin.Context) {
	reverseProxyGet(c, "compilersList", "https://godbolt.org/api/compilers", "GET", nil)
}

func handleGetCompilersListEx(c *gin.Context) {
	id := c.Param("id")
	if id == "" {
		c.JSON(http.StatusOK, gin.H{
			"result": "error",
			"msg":    "id expected"})
		return
	}
	reverseProxyGet(c, "compilersList:"+id, "https://godbolt.org/api/compilers/"+id, "GET", nil)
}

func handleGetLibrariesList(c *gin.Context) {
	id := c.Param("id")
	if id == "" {
		c.JSON(http.StatusOK, gin.H{
			"result": "error",
			"msg":    "id expected"})
		return
	}
	reverseProxyGet(c, "librariesList:"+id, "https://godbolt.org/api/libraries/"+id, "GET", nil)
}

func handleGetShortLinkInfo(c *gin.Context) {
	id := c.Param("id")
	if id == "" {
		c.JSON(http.StatusOK, gin.H{
			"result": "error",
			"msg":    "id expected"})
		return
	}
	reverseProxyGet(c, "shortlinkinfo:"+id, "https://godbolt.org/api/shortlinkinfo/"+id, "GET", nil)
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
	h.Write(body)
	cacheKey := fmt.Sprintf("compile:%s:%x", id, h.Sum(nil))

	reverseProxyGet(c, cacheKey, fmt.Sprintf("https://godbolt.org/api/compiler/%s/compile", id), "POST", bytes.NewBuffer(body))
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

	log.Fatal(r.Run(bindAddr))
}
