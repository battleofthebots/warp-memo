# Warp Memo

For our intrepid cosmic wanderers, we've engineered a stellar notes application to capture every wild thought and celestial revelation. Brace yourselves, fellow explorers, as this application rides the experimental wave, and we suspect it harbors interstellar bugs waiting to be exposed. As our bounty hunter, can you uncover any bugs? Remember to clean up after use. 

# Vulnerablity Explanation: Use after free

WIP

# Solution

`Local solve: python3 exploit.py`

`Local debugging: python3 exploit.py GDB`

`Remote solve: python3 exploit REMOTE <IP> <PORT>`


## SETUP

```
# Build docker image
docker build -t awarp-memo .

# Run image
docker run -p 1337:1337 warp-memo

```
