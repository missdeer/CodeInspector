package main

import (
	"path"
	"path/filepath"
	"strings"
	"sync"
)

type SampleList struct {
	filePath []string
	sync.RWMutex
}

func (s *SampleList) Append(f string) {
	s.Lock()
	s.filePath = append(s.filePath, f)
	s.Unlock()
}

func (s *SampleList) Clear() {
	s.Lock()
	s.filePath = []string{}
	s.Unlock()
}

func (s *SampleList) Count() int {
	s.RLock()
	defer s.RUnlock()
	return len(s.filePath)
}

func (s *SampleList) GetSamples(language string) (res []string) {
	s.RLock()
	for _, f := range s.filePath {
		if strings.HasPrefix(f, path.Join(sampleDir, language)) {
			res = append(res, filepath.Base(f))
		}
	}
	s.RUnlock()
	return
}
