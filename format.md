# Format .bpe

| Offset   | Size  | Field           | Type           | Description                              |
| -------- | ----- | --------------- | -------------- | ---------------------------------------- |
| 0        | 3     | Magic           | `char[3]`      | File signature: `"BPE"`                  |
| 3        | 2     | Rule count      | `uint16`       | Number of BPE rules (N)                  |
| 5        | N × 3 | Rules           | `uint8[3]` × N | Each rule: symbol, pair[0], pair[1]      |
| 5 + N×3  | 8     | Original size   | `uint64`       | Size of the original uncompressed data   |
| 13 + N×3 | 8     | Compressed size | `uint64`       | Size of the compressed data that follows |
| 21 + N×3 | ...   | Compressed data | `uint8[]`      | The BPE-compressed byte stream           |
