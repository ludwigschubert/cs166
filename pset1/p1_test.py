n = 20



k_lookup = [0] * n
k = 0
two_to_k = 1
for i in range(n):
    if two_to_k <= i:
        k += 1
        two_to_k = 2**k
    k_lookup[i] = k - 1

print k_lookup