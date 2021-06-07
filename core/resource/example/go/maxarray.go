// Type your code here, or load an example.
package main

func MaxArray(x, y []float64) {
	for i, c := range x {
		if y[i] > c {
			x[i] = y[i]
		}
	}
}

func main() {}
