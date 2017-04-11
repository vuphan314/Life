# Life

```
n: order
```

```
space(n) == 2 ^ (n ^ 2)
prespace(n) == space(n + 2)
preimage(n) <= prespace(n)
proportion(n) == preimage(n) / prespace(n)
```

```
preimage(k * n) <= (preimage(n)) ^ (k ^ 2)
space(k * n) == (space(n)) ^ (k ^ 2)
proportion(k * n) <= (preimage(n)) ^ (k ^ 2) / prespace(k * n)
proportion(k * n) == preimage(k * n) / prespace(k * n)
proportion(k * n) <= ?
```
