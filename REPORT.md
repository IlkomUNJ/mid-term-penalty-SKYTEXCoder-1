After careful consideration, i've established that the ideal matrix window size for analyzeation is 3x3, which consists of 9 pixels. Because if it's smaller or bigger than 3x3, it'll be very hard to analyze
because 2x2 only consists of 4 pixels (not enough pixels to examine) and if it's 4x4 or bigger, the total pixel count is 16 pixels and that'll be very hard to analyze. If we opt to use a non-symmetrical window size
like (3x4, or 4x3). It's obviously non-symmetrical so that'll be very hard to analyze. 

basically there are four appropriate window criterias:

1: endpoint pattern
2: body/middle pattern
3: corner pattern
4: intersection pattern


https://gemini.google.com/share/e26271c2fd5f