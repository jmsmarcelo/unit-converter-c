# Unit Converter

This is a simple and efficient unit converter written in pure C. It was developed as part of the [Roadmap.sh Unit Converter Project](https://roadmap.sh/projects/unit-converter), and can be used as a standalone API or alongside the included frontend.

## 🚀 Features

- Lightweight C backend with minimal dependencies
- Cross-platform support (Linux/Windows)
- Modular and extensible architecture
- Supports conversion of:
  - 📏 Length
  - ⚖️ Weight
  - 🌡️ Temperature
- REST-style communication over raw sockets
- Optional frontend client (HTML/JS/CSS)

## 🛠️ Build & Run

### Requirements

- **Compiler**
  - Linux: **GCC** (GNU Compiler Collection)
  - Windows: **MSVC** (via Developer Command Prompt)
- **Run:**
  - Any terminal (Bash, CMD, or PowerShell)

### Steps

1. **Clone the repository:**
   ```bash
   git clone https://github.com/jmsmarcelo/unit-converter-c
   cd unit-converter-c
   ```
2. **Compile the project:**
   ```bash
   mkdir build
   cd build

   # On Linux:
   gcc -o unit-converter-server ../src/*.c

   # Windows (MSVC):
   cl /Fe:unit-converter-server.exe ..\src\*.c
   ```
3. **Run the application:**
   ```bash
   # Optional: Set server port (default is 8080)
   export HTTP_PORT="80"    # Linux
   set HTTP_PORT=80         # Windows CMD
   $env:HTTP_PORT="80"      # Windows PowerShell

   # Start the server
   ./unit-converter-server  # Linux
   .\unit-converter-server  # Windows
   ```
4. **Access the frontend (optional):**

   Open in your browser:
   ```bash
   http://localhost:8080
   ```

## 📡 API Usage

You can test the API directly using tools like `curl`, Postman, or any HTTP client.

### Endpoint

```bash
POST /api/unit-converter
```

### Supported Units

| Category | Units |
| --- | --- |
| Lenght | mm, cm, m, km, in, ft, yd, mi |
| Weight | µg, g, kg, t, oz, lb |
| Temperature | °C, °F, K |


### Request Example

```json
{
    "category": "Length",
    "value": 100,
    "unit_from": "m",
    "unit_to": "km"
}
```
⚠️ Use PascalCase for the `category` field (`Length`, `Weight`, `Temperature`).

### Response Example

```json
{
    "value": 0.1
}
```

### Error Example

```json
{
    "message": "Malformed JSON in request body"
}
```

## 🖼️ Screenshots


## 📜 License

This project is licensed under the **MIT License**. See the [LICENSE](https://github.com/jmsmarcelo/unit-converter-c/blob/main/LICENSE) file for details.

## 👨‍💻 About

Created by [Jose Marcelo](https://jmsmarcelo.github.io) as a hands-on project to practice:
- Modular C programming
- Network programming with raw sockets
- Cross-platform development in C
- Frontend-backend integration

---
✨ Contributions, suggestions, or issues are welcome!

---