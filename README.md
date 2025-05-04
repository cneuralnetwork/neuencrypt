# neuencrypt
## Has
### Ciphers
1. **Caesar Cipher** - Simple substitution with fixed shift
2. **Monoalphabetic Cipher** - One-to-one character substitution
3. **Homophonic Cipher** - One-to-many character substitution
4. **Polygram Cipher** - Block substitution (digraphs)
5. **Polyalphabetic Cipher** - Multiple substitution alphabets (Vigenère)
6. **Playfair Cipher** - Digraph substitution using 5x5 matrix
7. **Hill Cipher** - Matrix-based block cipher

### Encoding Methods
- Base64 Encoding/Decoding
- Base32 Encoding/Decoding
- Hexadecimal Encoding/Decoding
- Hash Functions (MD5, SHA-1, SHA-256)

## Prerequisites
Ubuntu : `sudo apt-get install build-essential libssl-dev`
Fedora : `sudo dnf install gcc make openssl-devel`

## Building it
1. `git clone https://github.com/cneuralnetwork/neuencrypt`
2. `cd neuencrypt`
3. `make clean`
4. `make`
5. `./neuencrypt`

### MIT LICENCE
