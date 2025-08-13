
# 🧠 Networking Lab Assignments (PCC-CS-692) - Socket Programming in C

This repository includes complete C implementations using **Socket Programming** for simulating data transmission and error detection techniques as required in the **Networking Lab (PCC-CS-692)** course.

Each protocol is implemented with:
- A **server** program to encode and send data
- A **client** program to receive, verify, and extract/correct data

---

## 📌 Assignments Covered

| Protocol           | Supports                 | Error Handling     |
|-------------------|--------------------------|--------------------|
| CRC (Cyclic Redundancy Check) | Encoding, Decoding, Remainder Check | ✅ Detects single-bit error |
| Checksum          | Encoding, Decoding       | ✅ Detects corruption |
| Hamming Code      | Encoding, Decoding       | ✅ Corrects single-bit error |
| Parity (Even/Odd) | Generator and Checker    | ✅ Detects single-bit error |

---

## 📁 File Structure

```
.
├── crc_server.c
├── crc_client.c
├── checksum_server.c
├── checksum_client.c
├── hamming_server.c
├── hamming_client.c
├── parity_server.c
├── parity_client.c
└── README.md
```

---

## 🛠️ How to Run the Projects

> Run each protocol in **two terminals**: one for server, one for client.

### ✅ 1. Compile All Programs

```bash
# CRC
gcc crc_server.c -o crc_server
gcc crc_client.c -o crc_client

# Checksum
gcc checksum_server.c -o checksum_server -lm
gcc checksum_client.c -o checksum_client -lm

# Hamming
gcc hamming_server.c -o hamming_server -lm
gcc hamming_client.c -o hamming_client -lm

# Parity
gcc parity_server.c -o parity_server
gcc parity_client.c -o parity_client
```

### ✅ 2. Run Each Protocol

- **Terminal 1 (Server)**:
  ```bash
  ./crc_server
  ./checksum_server
  ./hamming_server
  ./parity_server
  ```

- **Terminal 2 (Client)**:
  ```bash
  ./crc_client
  ./checksum_client
  ./hamming_client
  ./parity_client
  ```

---

## 🧪 Sample Test Cases

### 🔹 CRC Example

- **Dataword**: `100100`  
- **Divisor**: `1101`  
- → Codeword sent: `100100001`  
- ✅ Client receives and validates

---

### 🔹 Checksum Example

- **Dataword**: `11001100101010101111000011000011`  
- **Segment Length**: `8`  
- → Checksum: `11010011`  
- → Codeword: `1100110010101010111100001100001111010011`  
- ✅ Client validates sum, extracts data

---

### 🔹 Hamming Code Example

- **Dataword**: `1011010`  
- → Codeword: `10101010000`  
- Flip 5th bit → Client detects **error at position 5**, corrects it  
- ✅ Corrected codeword: `10101010000`  
- ✅ Extracted dataword: `1011010`

---

### 🔹 Parity Example

- **Dataword**: `110101`  
- **Mode**: Even  
- → Codeword: `1101010`  
- Flip a bit manually → Client detects parity mismatch  
- ✅ Parity check passed/failed accordingly

---

## 🔧 Optional Features Included

- ✅ Manual Error Injection (for CRC, Checksum, Hamming, Parity)
- ✅ Single-bit Error Correction (Hamming only)
- ✅ Real-time sender-receiver simulation
- ✅ Data extraction from verified/corrected codeword

---

## 📜 License

This project is free to use for academic and learning purposes.

---

## ⭐ GitHub Tip

If you found this helpful, consider starring 🌟 the repo and contributing improvements!
