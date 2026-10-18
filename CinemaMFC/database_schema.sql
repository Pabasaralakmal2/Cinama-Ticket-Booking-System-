-- Nexora Dolby Cinema - Database Schema & Initial Data

-- 1. Users Table (For Admin login and Customer reference)
CREATE TABLE users (
    user_id SERIAL PRIMARY KEY,
    username VARCHAR(100) UNIQUE,
    password_hash VARCHAR(255),
    role VARCHAR(20) NOT NULL DEFAULT 'customer' -- 'admin' or 'customer'
);

-- 2. Customers Table
CREATE TABLE customers (
    customer_id SERIAL PRIMARY KEY,
    user_id INT REFERENCES users(user_id) ON DELETE SET NULL,
    full_name VARCHAR(150) NOT NULL,
    phone VARCHAR(20) NOT NULL,
    email VARCHAR(150) NOT NULL
);

-- 3. Movies Table
CREATE TABLE movies (
    movie_id SERIAL PRIMARY KEY,
    title VARCHAR(150) NOT NULL,
    genre VARCHAR(100) NOT NULL,
    duration VARCHAR(20) NOT NULL, -- e.g., '2h 49m'
    description TEXT,
    poster_url TEXT,
    trailer_url TEXT,
    format VARCHAR(10) NOT NULL, -- '2D', '3D'
    release_date DATE,
    status VARCHAR(50) NOT NULL -- 'Now Showing', 'Coming Soon'
);

-- 4. Halls Table
CREATE TABLE halls (
    hall_id VARCHAR(10) PRIMARY KEY, -- 'C1', 'C2', 'C3'
    hall_name VARCHAR(50) NOT NULL,
    projection_type VARCHAR(50) NOT NULL,
    sound_system VARCHAR(50) NOT NULL,
    supports_3d BOOLEAN NOT NULL,
    capacity INT NOT NULL
);

-- 5. Seats Table
CREATE TABLE seats (
    seat_id SERIAL PRIMARY KEY,
    hall_id VARCHAR(10) REFERENCES halls(hall_id) ON DELETE CASCADE,
    row_label VARCHAR(5) NOT NULL,
    seat_number INT NOT NULL,
    seat_code VARCHAR(10) NOT NULL -- e.g., 'A1', 'C3'
);

-- 6. Showtimes Table
CREATE TABLE showtimes (
    showtime_id SERIAL PRIMARY KEY,
    movie_id INT REFERENCES movies(movie_id) ON DELETE CASCADE,
    hall_id VARCHAR(10) REFERENCES halls(hall_id) ON DELETE CASCADE,
    show_date DATE NOT NULL,
    start_time TIME NOT NULL,
    format VARCHAR(10) NOT NULL, -- '2D' or '3D' depending on the movie/hall combination
    ticket_price DECIMAL(10, 2) NOT NULL
);

-- 7. Bookings Table
CREATE TABLE bookings (
    booking_id SERIAL PRIMARY KEY,
    booking_reference VARCHAR(50) UNIQUE NOT NULL, -- e.g., NEX-20260928-000183
    customer_id INT REFERENCES customers(customer_id) ON DELETE CASCADE,
    showtime_id INT REFERENCES showtimes(showtime_id) ON DELETE CASCADE,
    booking_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    total_amount DECIMAL(10, 2) NOT NULL,
    payment_status VARCHAR(50) NOT NULL DEFAULT 'Pending',
    booking_status VARCHAR(50) NOT NULL DEFAULT 'Active'
);

-- 8. BookingSeats Table (Mapping table to track booked seats per booking)
CREATE TABLE booking_seats (
    booking_seat_id SERIAL PRIMARY KEY,
    booking_id INT REFERENCES bookings(booking_id) ON DELETE CASCADE,
    seat_id INT REFERENCES seats(seat_id) ON DELETE CASCADE,
    UNIQUE (booking_id, seat_id)
);

-- 9. Payments Table
CREATE TABLE payments (
    payment_id SERIAL PRIMARY KEY,
    booking_id INT REFERENCES bookings(booking_id) ON DELETE CASCADE,
    amount DECIMAL(10, 2) NOT NULL,
    payment_method VARCHAR(50) NOT NULL, -- 'Card'
    payment_status VARCHAR(50) NOT NULL, -- 'Success', 'Failed'
    transaction_reference VARCHAR(100),
    payment_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Initial Data Insertion

-- Admin User
INSERT INTO users (username, password_hash, role) VALUES ('admin', 'admin123', 'admin');

-- Halls
INSERT INTO halls (hall_id, hall_name, projection_type, sound_system, supports_3d, capacity) VALUES 
('C1', 'NEXORA Dolby Cinema', '8K Laser', 'Dolby Atmos', TRUE, 140),
('C2', 'Premium 2K', '2K Projector', '7.1 Surround', TRUE, 140),
('C3', 'Standard 2K', '2K Projector', '7.1 Surround', FALSE, 140);

-- Generate Seats for C1, C2, C3 (A1-A20 to G1-G20) - 140 seats per hall
DO $$
DECLARE
    h RECORD;
    r VARCHAR[] := ARRAY['A','B','C','D','E','F','G'];
    s INT;
    i INT;
BEGIN
    FOR h IN SELECT hall_id FROM halls LOOP
        FOR i IN 1..7 LOOP
            FOR s IN 1..20 LOOP
                INSERT INTO seats (hall_id, row_label, seat_number, seat_code) 
                VALUES (h.hall_id, r[i], s, r[i] || s::VARCHAR);
            END LOOP;
        END LOOP;
    END LOOP;
END $$;

-- Movies
INSERT INTO movies (title, genre, duration, description, poster_url, trailer_url, format, release_date, status) VALUES 
('Interstellar', 'Sci-Fi', '2h 49m', 'A team of explorers travel through a wormhole in space in an attempt to ensure humanity''s survival.', 'https://image.tmdb.org/t/p/w500/gEU2QniE6E77NI6lCU6MxlNBvIx.jpg', 'https://www.youtube.com/embed/zSWdZVtXT7E', '2D', '2014-11-05', 'Now Showing'),
('Avatar: The Way of Water', 'Action, Sci-Fi', '3h 12m', 'Jake Sully lives with his newfound family formed on the extrasolar moon Pandora.', 'https://image.tmdb.org/t/p/w500/t6HIqrHeCPJ7qXWCEh31F0TqD36.jpg', 'https://www.youtube.com/embed/a8Gx8wiNbs8', '3D', '2022-12-14', 'Now Showing'),
('The Dark Knight', 'Action, Crime', '2h 32m', 'When the menace known as the Joker wreaks havoc and chaos on the people of Gotham, Batman must accept one of the greatest psychological and physical tests of his ability to fight injustice.', 'https://image.tmdb.org/t/p/w500/qJ2tW6WMUDux911r6m7haRef0WH.jpg', 'https://www.youtube.com/embed/EXeTwQWrcwY', '2D', '2008-07-16', 'Now Showing'),
('Dune: Part Two', 'Sci-Fi, Adventure', '2h 46m', 'Paul Atreides unites with Chani and the Fremen while on a warpath of revenge against the conspirators who destroyed his family.', 'https://image.tmdb.org/t/p/w500/1pdfLvkbY9ohJlCjQH2JGqqC2T.jpg', 'https://www.youtube.com/embed/U2Qp5pL3ovA', '3D', '2024-02-28', 'Coming Soon'),
('Oppenheimer', 'Biography, Drama', '3h 0m', 'The story of American scientist, J. Robert Oppenheimer, and his role in the development of the atomic bomb.', 'https://image.tmdb.org/t/p/w500/8Gxv8gSFCU0XGDykEGv7zR1n2ua.jpg', 'https://www.youtube.com/embed/uYPbbksJxIg', '2D', '2023-07-19', 'Now Showing');

-- Additional movies (added for testing/demo)
INSERT INTO movies (title, genre, duration, description, poster_url, trailer_url, format, release_date, status) VALUES
('Top Gun: Maverick', 'Action, Drama', '2h 11m', 'Pete Mitchell returns to train the next generation of Top Gun graduates for a specialized mission.', 'https://image.tmdb.org/t/p/w500/62HCnUTziyWcpDaBO2i1DX17ljH.jpg', 'https://www.youtube.com/embed/giXco2jaZ_4', '2D', '2022-05-27', 'Now Showing'),
('Spider-Man: Across the Spider-Verse', 'Animation, Action', '2h 20m', 'Miles Morales returns for a new adventure across the Spider-Verse.', 'https://image.tmdb.org/t/p/w500/8Vt6mWEReuy4Of61Lnj5Xj704m8.jpg', 'https://www.youtube.com/embed/3V4plMlhg7I', '2D', '2023-06-02', 'Now Showing'),
('Mission: Impossible - Dead Reckoning Part One', 'Action, Thriller', '2h 43m', 'Ethan Hunt and his team face their most impossible mission yet.', 'https://image.tmdb.org/t/p/w500/62HCnUTziyWcpDaBO2i1DX17ljH.jpg', 'https://www.youtube.com/embed/Dm2tYFUVK_4', '2D', '2023-07-12', 'Now Showing'),
('Guardians of the Galaxy Vol. 3', 'Action, Adventure', '2h 30m', 'The Guardians embark on a mission to protect one of their own while facing new threats.', 'https://image.tmdb.org/t/p/w500/azTxQ0fZ9bE0Yw1Y3Jl8k3D2g9S.jpg', 'https://www.youtube.com/embed/J0MNxT0m4oQ', '2D', '2023-05-03', 'Now Showing'),
('John Wick: Chapter 4', 'Action, Thriller', '2h 49m', 'John Wick uncovers a path to defeating The High Table.', 'https://image.tmdb.org/t/p/w500/zb6fM4o3k4d2bC8QkqQpZ5VOw3U.jpg', 'https://www.youtube.com/embed/0OFb9e0mQf0', '2D', '2023-03-24', 'Now Showing');

-- Showtimes (Assuming today is 2026-09-27)
-- C1 (Dolby Cinema)
INSERT INTO showtimes (movie_id, hall_id, show_date, start_time, format, ticket_price) VALUES 
(1, 'C1', CURRENT_DATE, '10:00:00', '2D', 1500.00), -- Interstellar 2D
(2, 'C1', CURRENT_DATE, '13:30:00', '3D', 1800.00), -- Avatar 3D
(5, 'C1', CURRENT_DATE, '17:00:00', '2D', 1500.00), -- Oppenheimer 2D
(2, 'C1', CURRENT_DATE, '20:30:00', '3D', 1800.00); -- Avatar 3D

-- Additional showtimes for the newly added movies
INSERT INTO showtimes (movie_id, hall_id, show_date, start_time, format, ticket_price) VALUES
(6, 'C1', CURRENT_DATE, '12:30:00', '2D', 1600.00), -- Top Gun: Maverick
(7, 'C1', CURRENT_DATE, '15:45:00', '2D', 1600.00), -- Spider-Man: Across the Spider-Verse
(9, 'C1', CURRENT_DATE, '19:00:00', '2D', 1600.00); -- Guardians Vol.3

-- C2 (Premium 2K)
INSERT INTO showtimes (movie_id, hall_id, show_date, start_time, format, ticket_price) VALUES 
(3, 'C2', CURRENT_DATE, '11:00:00', '2D', 1300.00), -- Dark Knight 2D
(1, 'C2', CURRENT_DATE, '14:00:00', '2D', 1300.00), -- Interstellar 2D
(2, 'C2', CURRENT_DATE, '17:30:00', '3D', 1500.00), -- Avatar 3D
(3, 'C2', CURRENT_DATE, '21:00:00', '2D', 1300.00); -- Dark Knight 2D

-- Additional showtimes in C2
INSERT INTO showtimes (movie_id, hall_id, show_date, start_time, format, ticket_price) VALUES
(8, 'C2', CURRENT_DATE, '13:15:00', '2D', 1400.00), -- Mission: Impossible
(10, 'C2', CURRENT_DATE, '16:45:00', '2D', 1400.00); -- John Wick 4

-- C3 (Standard 2K)
INSERT INTO showtimes (movie_id, hall_id, show_date, start_time, format, ticket_price) VALUES 
(5, 'C3', CURRENT_DATE, '10:30:00', '2D', 1300.00), -- Oppenheimer 2D
(3, 'C3', CURRENT_DATE, '14:30:00', '2D', 1300.00), -- Dark Knight 2D
(1, 'C3', CURRENT_DATE, '17:30:00', '2D', 1300.00), -- Interstellar 2D
(5, 'C3', CURRENT_DATE, '21:30:00', '2D', 1300.00); -- Oppenheimer 2D

-- Additional showtimes in C3
INSERT INTO showtimes (movie_id, hall_id, show_date, start_time, format, ticket_price) VALUES
(7, 'C3', CURRENT_DATE, '12:00:00', '2D', 1200.00), -- Spider-Man
(6, 'C3', CURRENT_DATE, '15:00:00', '2D', 1200.00), -- Top Gun
(10, 'C3', CURRENT_DATE, '18:30:00', '2D', 1200.00); -- John Wick
