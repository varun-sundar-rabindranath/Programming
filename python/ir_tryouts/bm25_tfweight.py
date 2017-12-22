
# the tf weight in bm25 is (k * tf) / (k + tf).. lets see how tf weight changes
# based on k

k = 1000

for tf in range(1, 100):
    print tf, " ", float((k + 1) * tf) / float(k + tf)
