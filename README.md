<div align="center">
  <img src="assets/nexora_main_bg.png" alt="Nexora Dolby Cinema Logo" width="300"/>
  <h1>🍿 Nexora Dolby Cinema</h1>
  <p><i>A Next-Generation Cinema Ticketing & Management System built with C++, MFC, and Supabase.</i></p>
</div>

---

## 📖 Overview
**Nexora Dolby Cinema** is a premium desktop application built for managing cinema ticket bookings, browsing movie catalogs, and handling secure transactions. Designed with a stunning **glassmorphism UI**, dynamic TMDB poster integrations, and a robust real-time cloud backend, it demonstrates advanced C++ concepts and Object-Oriented Architecture.

## ✨ Key Features
- **🎬 Dynamic Movie Browser**: Fetches movie details and downloads high-quality posters on-the-fly using the TMDB API.
- **🛋️ Interactive Seat Selection**: A visually color-coded grid for reserving seats (Available, Selected, Booked).
- **🔒 Secure Checkout System**: Form validates emails, Mastercard/Visa cards, and CVVs using polymorphic payment processors.
- **🎟️ Digital Ticket Generation**: Instantly generates booking receipts complete with scannable **QR Codes** that can be saved to your local machine.
- **🎥 Embedded Trailer Player**: Watch movie trailers directly inside the application using an embedded player (`mpv`).
- **🛡️ Admin Dashboard**: Secure login for cinema administrators to add, edit, and remove movies and showtimes.
- **🔄 Auto-Reset Scheduler**: Automatically resets and clears past bookings when a new day begins based on real-time clock data.

## 🛠️ Technology Stack
- **Language**: C++ (C++17/20)
- **Framework**: MFC (Microsoft Foundation Classes)
- **UI Rendering**: GDI+ (Custom drawing, glass overlays, alpha blending)
- **Database Backend**: [Supabase](https://supabase.com/) (PostgreSQL via REST API)
- **Networking**: `WinHTTP` for secure HTTPS REST requests
- **JSON Parsing**: `nlohmann/json`
- **Build System**: CMake

## 🏗️ Architecture & Design Patterns
Nexora is built with a clean, scalable architectural pattern:
- **Repository Pattern**: Clean separation of database logic (`MovieRepository`, `ShowtimeRepository`, `BookingRepository`, `SeatRepository`).
- **Singleton Pattern**: Ensures a single, thread-safe instance for the `DatabaseManager`.
- **Polymorphism & Abstraction**: Applied in the `PaymentProcessor` classes to handle varying payment gateways (e.g., `CardPayment`).
- **Multithreading**: Uses background threads for downloading heavy assets (like TMDB posters) to ensure the UI remains buttery smooth.

## 🚀 Getting Started

### Prerequisites
1. **Visual Studio 2022+** (with "Desktop development with C++" and MFC components installed).
2. **CMake** (v3.20+).
3. **Supabase Account**: You will need a Supabase project and an Anon Key.

### Building the Project
1. Clone the repository:
   ```bash
   git clone https://github.com/Pabasaralakmal2/Cinama-Ticket-Booking-System-.git
   cd Cinama-Ticket-Booking-System-
   ```
2. Open the Solution in Visual Studio:
   - Open `CinemaMFC/CinemaMFC.slnx` or `CinemaMFC/CinemaMFC/CinemaMFC.vcxproj` in Visual Studio 2022.
   - Select configuration **Debug** or **Release** with platform **x64**.
3. Build & Run:
   - Press `F5` or `Ctrl+F5` to build and launch the application.

## 📸 Screenshots & Architecture
<div align="center">
  <img src="CinemaMFC/CinemaMFC/uml_class_diagram.png" alt="UML Class Diagram" width="850"/>
  <br/><br/>
  <img src="assets/bg.png" alt="Nexora Dolby Cinema" width="850"/>
</div>

## 👨‍💻 Admin Access
To access the Admin Dashboard to manage movies and showtimes:
- **Email**: `pabasaralakmal275@gmail.com`
- **Password**: `2004`

## 📄 License
This project is licensed under the MIT License.

---
<div align="center">
  <i>Developed with ❤️ for Cinema Enthusiasts.</i>
</div>
